/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Author
    Zeljko Tukovic, FSB Zagreb.  All rights reserved

\*----------------------------------------------------------------------------*/

#include "pointHistory.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "pointFields.H"
#include "boundBox.H"
#include "fluidStructureInterface.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(pointHistory, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        pointHistory,
        dictionary
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::pointHistory::writeData()
{
    const fvMesh& mesh =
        time_.lookupObject<fvMesh>(polyMesh::defaultRegion);

    vector deflection = vector::zero;
    vector velocity = vector::zero;

    if (processor_ == Pstream::myProcNo())
    {
        if (mesh.foundObject<fluidStructureInterface>("fsiProperties"))
        {
            const fluidStructureInterface& fsi = 
                mesh.lookupObject<fluidStructureInterface>("fsiProperties");

            deflection = fsi.stress().pointD()[historyPointID_];

            if (writeVelocity_)
            {
                velocity = fsi.stress().pointU(historyPointID_);
            }
        }
        else
        {
            deflection = mesh.points()[historyPointID_] - refHistoryPoint_;

            if (mesh.foundObject<pointVectorField>("pointD"))
            {
                const pointVectorField& pointU = 
                    mesh.lookupObject<pointVectorField>("pointD");
            
                deflection = pointU[historyPointID_];
            }
            else if (mesh.foundObject<pointVectorField>("pointU"))
            {
                const pointVectorField& pointU = 
                    mesh.lookupObject<pointVectorField>("pointU");
                
                deflection = pointU[historyPointID_];
            }
        }
    }


    reduce(deflection, sumOp<vector>());
    reduce(velocity, sumOp<vector>());

    if (Pstream::master())
    {
        historyFilePtr_() 
            << time_.time().value() << tab
            << deflection.x() << tab
            << deflection.y() << tab
            << deflection.z();

        if (writeVelocity_)
        {
            historyFilePtr_()
                << tab << velocity.x() << tab 
                    << velocity.y() << tab << velocity.z();
        }

        historyFilePtr_() << endl;
    }

    return true;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointHistory::pointHistory
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    functionObject(name),
    name_(name),
    time_(t),
    regionName_(polyMesh::defaultRegion),
    historyPointID_(-1),
    refHistoryPoint_(dict.lookup("refHistoryPoint")),
    writeVelocity_(false),
    processor_(-1),
    historyFilePtr_(NULL)
{
    Info << "Creating " << this->name() << " function object." << endl;

//     if (Pstream::parRun())
//     {
//         FatalErrorIn("pointHistory::pointHistory(...)")
//             << "pointHistory objec function "
//                 << "is not implemented for parallel run"
//                 << abort(FatalError);
//     }

    if (dict.found("region"))
    {
        dict.lookup("region") >> regionName_;
    }

    if (dict.found("historyPointID"))
    {
        dict.lookup("historyPointID") >> historyPointID_;
    }

    if (dict.found("writeVelocity"))
    {
        dict.lookup("writeVelocity") >> writeVelocity_;
    }

    const fvMesh& mesh =
        time_.lookupObject<fvMesh>(regionName_);

    const vectorField& points = mesh.points();

    List<scalar> minDist(Pstream::nProcs(), GREAT);
    
    if (historyPointID_ == -1)
    {
        forAll(points, pointI)
        {
            scalar dist = mag(refHistoryPoint_ - points[pointI]);

            if (dist < minDist[Pstream::myProcNo()])
            {
                minDist[Pstream::myProcNo()] = dist;
                historyPointID_ = pointI;
            }
        }
    }

    Pstream::gatherList(minDist);
    Pstream::scatterList(minDist);

    processor_ = -1;
    scalar min = GREAT;

    forAll(minDist, procI)
    {
        if (minDist[procI] < min)
        {
            min = minDist[procI];
            processor_ = procI;
        }
    }

    if (processor_ == Pstream::myProcNo())
    {
        Pout << "History point ID: " << historyPointID_ << endl;
        Pout << "History point coordinates: " 
            << points[historyPointID_] << endl;
        Pout << "Reference point coordinates: " << refHistoryPoint_ << endl;
    }

    // Create history file if not already created
    if (historyFilePtr_.empty())
    {
        // File update
        if (Pstream::master())
        {
            fileName historyDir;

            word startTimeName =
                time_.timeName(mesh.time().startTime().value());


            if (Pstream::parRun())
            {
                // Put in undecomposed case (Note: gives problems for
                // distributed data running)
                historyDir = time_.path()/".."/"history"/startTimeName;
            }
            else
            {
                historyDir = time_.path()/"history"/startTimeName;
            }

            // Create directory if does not exist.
            mkDir(historyDir);

            // Open new file at start up
            historyFilePtr_.reset(new OFstream(historyDir/"point.dat"));

            // Add headers to output data
            if (historyFilePtr_.valid())
            {
                historyFilePtr_()
                    << "# Time" << tab << "X" << tab 
                        << "Y" << tab << "Z";

                if (writeVelocity_)
                {
                    historyFilePtr_()
                        << tab << "Vx" << tab 
                            << "Vy" << tab << "Vz";
                }

                historyFilePtr_() << endl;
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::pointHistory::start()
{
    return writeData();
}


bool Foam::pointHistory::execute()
{
    return writeData();
}


bool Foam::pointHistory::read(const dictionary& dict)
{
    if (dict.found("region"))
    {
        dict.lookup("region") >> regionName_;
    }

    return true;
}

// ************************************************************************* //
