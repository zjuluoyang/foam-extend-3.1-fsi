/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2005 OpenCFD Ltd.
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
    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Class
    ITLMaterialInterface

Description

SourceFiles
    ITLMaterialInterface.C

\*---------------------------------------------------------------------------*/

#ifndef ITLMaterialInterface_H
#define ITLMaterialInterface_H

#include "fvMesh.H"
// #include "fvMeshSubset.H"
#include "materialInterface.H"

#include "fvMatrices.H"
// #include "leastSquaresVolPointInterpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class freeSurface Declaration
\*---------------------------------------------------------------------------*/

class ITLMaterialInterface
:
    public materialInterface
{
    // Private data
    
        //- Displacement increment field
        const volVectorField& DD_;

        //- Point displacement increment field
        const pointVectorField& pointDD_;

        //- Displacement field
        const volVectorField& D_;

        //- Current time index
        label curTimeIndex_;

    // Demand-driven data

        //- Displacement increment at the interface
        mutable vectorField* displacementIncrementPtr_;

        //- Old displacement at the interface
        mutable vectorField* displacementPtr_;

        //- Cauchy traction increment at the interface
        mutable vectorField* tractionIncrementPtr_;

        //- Old Cauchy traction at the interface
        mutable vectorField* tractionPtr_;

        //- Second Piola-Kirchhoff stress increment at the interface
        mutable symmTensorField* ownDSigmafPtr_;

        //- Second Piola-Kirchhoff stress increment at the interface
        mutable symmTensorField* ngbDSigmafPtr_;

        //- Old second Piola-Kirchhoff stress at the interface
        mutable symmTensorField* ownSigmafPtr_;

        //- Old second Piola-Kirchhoff stress at the interface
        mutable symmTensorField* ngbSigmafPtr_;

        //- Point displacement increment for each sub-mesh
        mutable PtrList<volVectorField> subMeshDD_;

        //- Point displacement increment for each sub-mesh
        mutable PtrList<pointVectorField> subMeshPointDD_;

    // Private Member Functions

        // Make demand-driven data
    
        //- Make interface displacement field
        void makeDisplacementIncrement() const;

        //- Make interface displacement field
        void makeDisplacement() const;

        //- Make interface displacement field
        void makeTractionIncrement() const;

        //- Make interface displacement field
        void makeTraction() const;

        //- Make second Piola-Kirchhoff stress increment at the interface
        void makeDSigmaf() const;

        //- Make old second Piola-Kirchhoff stress at the interface
        void makeSigmaf() const;

        //- Make displacements field
        void makeSubMeshDD() const;

        //- Make point displacements field
        void makeSubMeshPointDD() const;

        //- Clear all demand-driven data
        void clearOut();

        //- Disallow default bitwise copy construct
        ITLMaterialInterface(const ITLMaterialInterface&);

        //- Disallow default bitwise assignment
        void operator=(const ITLMaterialInterface&);

public:

    //- Runtime type information
    TypeName("ITLMaterialInterface");

    // Constructors

        ITLMaterialInterface
        (
            const volVectorField& DD,
            const pointVectorField& pointDD,
            const volVectorField& D
        );

    // Destructor

        virtual ~ITLMaterialInterface();  


    // Member Functions

        // Access

            //- Return interface displacement increment
            vectorField& displacementIncrement();

            //- Return interface displacement increment
            const vectorField& displacementIncrement() const;

            //- Return interface displacement
            vectorField& displacement();

            //- Return interface displacement
            const vectorField& displacement() const;

            //- Return interface traction increment
            vectorField& tractionIncrement();

            //- Return interface traction increment
            const vectorField& tractionIncrement() const;

            //- Return interface traction
            vectorField& traction();

            //- Return interface traction
            const vectorField& traction() const;

            //- Return old second Piola-Kirchhoff stress
            symmTensorField& ownSigmaf();

            //- Return old second Piola-Kirchhoff stress
            const symmTensorField& ownSigmaf() const;

            //- Return old second Piola-Kirchhoff stress
            symmTensorField& ngbSigmaf();

            //- Return old second Piola-Kirchhoff stress
            const symmTensorField& ngbSigmaf() const;

            //- Return old second Piola-Kirchhoff stress
            symmTensorField& ownDSigmaf();

            //- Return old second Piola-Kirchhoff stress
            const symmTensorField& ownDSigmaf() const;

            //- Return old second Piola-Kirchhoff stress
            symmTensorField& ngbDSigmaf();

            //- Return old second Piola-Kirchhoff stress
            const symmTensorField& ngbDSigmaf() const;

            //- Return list of point displacement fields
            const PtrList<volVectorField>& subMeshDD() const;

            //- Return list of point displacement fields
            PtrList<volVectorField>& subMeshDD();

            //- Return list of point displacement fields
            const PtrList<pointVectorField>& subMeshPointDD() const;

            //- Return list of point displacement fields
            PtrList<pointVectorField>& subMeshPointDD();

        // Edit

            //- Correct discretization at the interface
            void correct(fvVectorMatrix& DDEqn);

            //- Correct traction at the interface
            void correct(surfaceVectorField& t);

            //- Update displacement fields
            void updateDisplacementIncrement(pointVectorField& pointDD);

            //- Calculate displacement gradients
            void updateDisplacementIncrementGradient
            (
                volTensorField& gradDD, 
                surfaceTensorField& gradDDf
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
