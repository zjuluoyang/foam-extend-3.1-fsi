/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     |
    \\  /    A nd           | For copyright notice see file Copyright
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "cellSetBoundaryToFace.H"
#include "polyMesh.H"
#include "cellSet.H"
#include "Time.H"
#include "syncTools.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(cellSetBoundaryToFace, 0);

addToRunTimeSelectionTable(topoSetSource, cellSetBoundaryToFace, word);

addToRunTimeSelectionTable(topoSetSource, cellSetBoundaryToFace, istream);

}


Foam::topoSetSource::addToUsageTable Foam::cellSetBoundaryToFace::usage_
(
    cellSetBoundaryToFace::typeName,
    "\n    Usage: cellSetBoundaryToFace <cellSet> all|both\n\n"
    "    Select -all : all faces of cells in the cellSet\n"
    "           -both: faces where both neighbours are in the cellSet\n\n"
);

template<>
const char* Foam::NamedEnum<Foam::cellSetBoundaryToFace::cellAction, 3>::names[] =
{
    "all",
    "both",
    "one"
};

const Foam::NamedEnum<Foam::cellSetBoundaryToFace::cellAction, 3>
    Foam::cellSetBoundaryToFace::cellActionNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::cellSetBoundaryToFace::combine(topoSet& set, const bool add) const
{
    // Load the set
    if (!exists(mesh_.time().path()/topoSet::localPath(mesh_, setName_)))
    {
        SeriousError<< "Cannot load set "
            << setName_ << endl;
    }

    cellSet loadedSet(mesh_, setName_);

    if (option_ == ALL)
    {
        // Add all faces from cell
        for
        (
            cellSet::const_iterator iter = loadedSet.begin();
            iter != loadedSet.end();
            ++iter
        )
        {
            label cellI = iter.key();

            const labelList& cFaces = mesh_.cells()[cellI];

            forAll(cFaces, cFaceI)
            {
                addOrDelete(set, cFaces[cFaceI], add);
            }
        }
    }
    else if (option_ == BOTH)
    {
        // Add all faces whose both neighbours are in set.

        label nInt = mesh_.nInternalFaces();
        const labelList& own = mesh_.faceOwner();
        const labelList& nei = mesh_.faceNeighbour();
        const polyBoundaryMesh& patches = mesh_.boundaryMesh();


        // Check all internal faces
        for (label faceI = 0; faceI < nInt; faceI++)
        {
            if (loadedSet.found(own[faceI]) && loadedSet.found(nei[faceI]))
            {
                addOrDelete(set, faceI, add);
            }
        }


        // Get coupled cell status
        boolList neiInSet(mesh_.nFaces()-nInt, false);

        forAll(patches, patchI)
        {
            const polyPatch& pp = patches[patchI];

            if (pp.coupled())
            {
                label faceI = pp.start();
                forAll(pp, i)
                {
                    neiInSet[faceI-nInt] = loadedSet.found(own[faceI]);
                    faceI++;
                }
            }
        }
        syncTools::swapBoundaryFaceList(mesh_, neiInSet, false);


        // Check all boundary faces
        forAll(patches, patchI)
        {
            const polyPatch& pp = patches[patchI];

            if (pp.coupled())
            {
                label faceI = pp.start();
                forAll(pp, i)
                {
                    if (loadedSet.found(own[faceI]) && neiInSet[faceI-nInt])
                    {
                        addOrDelete(set, faceI, add);
                    }
                    faceI++;
                }
            }
        }
    }
    else if (option_ == ONE)
    {
        // Add all faces whose only one neighbour is in set.

        label nInt = mesh_.nInternalFaces();
        const labelList& own = mesh_.faceOwner();
        const labelList& nei = mesh_.faceNeighbour();


        // Check all internal faces
        for (label faceI = 0; faceI < nInt; faceI++)
        {
            if (loadedSet.found(own[faceI]) && !loadedSet.found(nei[faceI]))
            {
                addOrDelete(set, faceI, add);
            }
            else if
            (
                !loadedSet.found(own[faceI]) && loadedSet.found(nei[faceI])
            )
            {
                addOrDelete(set, faceI, add);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from componenta
Foam::cellSetBoundaryToFace::cellSetBoundaryToFace
(
    const polyMesh& mesh,
    const word& setName,
    const cellAction option
)
:
    topoSetSource(mesh),
    setName_(setName),
    option_(option)
{}


// Construct from dictionary
Foam::cellSetBoundaryToFace::cellSetBoundaryToFace
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    setName_(dict.lookup("set")),
    option_(cellActionNames_.read(dict.lookup("option")))
{}


// Construct from Istream
Foam::cellSetBoundaryToFace::cellSetBoundaryToFace
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    setName_(checkIs(is)),
    option_(cellActionNames_.read(checkIs(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::cellSetBoundaryToFace::~cellSetBoundaryToFace()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::cellSetBoundaryToFace::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding faces according to cellSet " << setName_
            << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing faces according to cellSet " << setName_
            << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
