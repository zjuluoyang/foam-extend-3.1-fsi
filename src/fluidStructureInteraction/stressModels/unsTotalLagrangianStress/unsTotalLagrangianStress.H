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

Class
    unsTotalLagrangianStress

Description
    

Author
    Zeljko Tukovic, FSB Zagreb.  All rights reserved.

SourceFiles
    unsTotalLagrangianStress.C

\*---------------------------------------------------------------------------*/

#ifndef unsTotalLagrangianStress_H
#define unsTotalLagrangianStress_H

#include "stressModel.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "pointFields.H"
#include "constitutiveModel.H"
#include "thermalModel.H"
#include "leastSquaresVolPointInterpolation.H"
#include "TLMaterialInterface.H"
#include "TLMaterialInterface.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace stressModels
{

/*---------------------------------------------------------------------------*\
                        Class unsTotalLagrangianStress Declaration
\*---------------------------------------------------------------------------*/

class unsTotalLagrangianStress
:
    public stressModel
{
    // Private data

        //- Displacement field
        volVectorField D_;

        //- Velocity field
        volVectorField U_;

        //- Point mesh
        pointMesh pMesh_;

        //- Point displacement field
        pointVectorField pointD_;

        //- Stress field
        volSymmTensorField sigma_;

        //- Strain field
        volSymmTensorField epsilon_;

        //- Rheology model
        constitutiveModel rheology_;

        //- Cell-to-point interpolation
        leastSquaresVolPointInterpolation volToPoint_;

        //- Face displacement gradient
        surfaceTensorField gradDf_;

        //- Cell displacement gradient
        volTensorField gradD_;

        //- Density
        volScalarField rho_;

        //- First Lame's coefficient
        volScalarField mu_;

        //- First Lame's coefficient
        surfaceScalarField muf_;

        //- Second Lame's coefficient
        volScalarField lambda_;

        //- Second Lame's coefficient
        surfaceScalarField lambdaf_;

        //- Solid interface
        autoPtr<TLMaterialInterface> interface_;

        //- Pointer to threeK field
        mutable volScalarField* threeKPtr_;

        //- Pointer to alpha field
        mutable volScalarField* alphaPtr_;

        //- Pointer to threeK field
        mutable surfaceScalarField* threeKfPtr_;

        //- Pointer to alpha field
        mutable surfaceScalarField* alphafPtr_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        unsTotalLagrangianStress(const unsTotalLagrangianStress&);

        //- Disallow default bitwise assignment
        void operator=(const unsTotalLagrangianStress&);


public:

    //- Runtime type information
    TypeName("unsTotalLagrangianStress");

    // Constructors

        //- Construct from components
        unsTotalLagrangianStress(const fvMesh& mesh);


    // Destructor

        virtual ~unsTotalLagrangianStress()
        {
            deleteDemandDrivenData(threeKPtr_);
            deleteDemandDrivenData(alphaPtr_);
            deleteDemandDrivenData(threeKfPtr_);
            deleteDemandDrivenData(alphafPtr_);
        }


    // Member Functions

        // Access

            //- Return cell displacement field
            const volVectorField& D() const
            {
                return D_;
            }

            //- Return cell displacement field
            volVectorField& D()
            {
                return D_;
            }

            //- Return point mesh
            const pointMesh& pMesh() const
            {
                return pMesh_;
            }

            const volSymmTensorField& sigma() const
            {
                return sigma_;
            }

            const volSymmTensorField& epsilon() const
            {
                return epsilon_;
            }

            virtual const constitutiveModel& rheology() const
            {
                return rheology_;
            }

            const volScalarField& mu() const
            {
                return mu_;
            }

            const volScalarField& lambda() const
            {
                return lambda_;
            }

            //- Return point displacement field
            virtual const pointVectorField& pointD() const
            {
                return pointD_;
            }

            //- Return multi-material interface pointer
            const autoPtr<TLMaterialInterface>& interface() const
            {
                return interface_;
            }

            //- Return multi-material interface pointer
            autoPtr<TLMaterialInterface>& interface()
            {
                return interface_;
            }

            //- Return multi-material interface pointer
            const leastSquaresVolPointInterpolation& volToPoint() const
            {
                return volToPoint_;
            }

            //- Return point displacement field
            virtual vector pointU(label pointID) const;

            //- Patch point displacement increment
            virtual tmp<vectorField> patchPointDisplacementIncrement
            (
                const label patchID
            ) const;

            //- Face zone point displacement increment
            virtual tmp<vectorField> faceZonePointDisplacementIncrement
            (
                const label zoneID
            ) const;

            //- Face zone velocity surface (tangential) gradient
            virtual tmp<tensorField> faceZoneSurfaceGradientOfVelocity
            (
                const label zoneID,
                const label patchID
            ) const;

            //- Face zone current normal
            virtual tmp<vectorField> faceZoneNormal
            (
                const label zoneID,
                const label patchID
            ) const;

            //- Face zone primitive patch in current configuration
            virtual tmp<vectorField> 
            currentFaceZonePoints(const label zoneID) const;

            //- Reference to temperature difference field
            const surfaceScalarField& DTf() const;

            //- Reference to ref temperature difference
            const volScalarField& DT() const;

            //- Reference to 3K propertie field
            const volScalarField& threeK() const;

            //- Reference to alpha propertie field
            const volScalarField& alpha() const;

            //- Reference to 3K propertie field
            const surfaceScalarField& threeKf() const;

            //- Reference to alpha propertie field
            const surfaceScalarField& alphaf() const;

        // Edit

            //- Set traction at specified patch
            virtual void setTraction
            (
                const label patchID, 
                const vectorField& traction
            );

            //- Set pressure at specified patch
            virtual void setPressure
            (
                const label patchID, 
                const scalarField& pressure
            );

            //- Set traction at specified patch
            virtual void setTraction
            (
                const label patchID, 
                const label zoneID, 
                const vectorField& zoneTraction
            );

            //- Set pressure at specified patch
            virtual void setPressure
            (
                const label patchID, 
                const label zoneID, 
                const scalarField& zonePressure
            );

            //- Predict traction at specified zone
            virtual tmp<vectorField> predictTraction
            (
                const label patchID,
                const label zoneID
            );

            //- Predict pressure at specified zone
            virtual tmp<scalarField> predictPressure
            (
                const label patchID, 
                const label zoneID
            );

            //- Evolve the flow model
            virtual bool evolve();

            //- Evolve the flow model
            virtual void predict();

            //- Smooth solution
            virtual scalar smooth
            (
                const volVectorField& source,
                label nCorrectors = 0
            );

            //- Smooth solution
            virtual scalar smooth
            (
                const volVectorField& source,
                const volVectorField& refSolution,
                label nCorrectors = 0
            );

            //- Return solution (displacement field)
            virtual const volVectorField& solution() const
            {
                return D_;
            }

            //- Return solution (displacement field)
            virtual volVectorField& solution()
            {
                return D_;
            }

            //- Initialise solution
            virtual void initialise
            (
                const volVectorField& sol,
                bool consistentBoundaryField = false
            );

            //- Return current residual
            virtual tmp<volVectorField> residual
            (
                const volVectorField& source
            );

            //- Return current relative momentum resudual
            scalar residual() const;

            //- Write using given format, version and compression
            virtual bool writeObject
            (
                IOstream::streamFormat,
                IOstream::versionNumber,
                IOstream::compressionType
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace flowModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
