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
    unsIncrTotalLagrangianStress

Description
    

Author
    Zeljko Tukovic, FSB Zagreb.  All rights reserved.

SourceFiles
    unsIncrTotalLagrangianStress.C

\*---------------------------------------------------------------------------*/

#ifndef unsIncrTotalLagrangianStress_H
#define unsIncrTotalLagrangianStress_H

#include "stressModel.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "pointFields.H"
#include "constitutiveModel.H"
#include "leastSquaresVolPointInterpolation.H"
// #include "volPointEnhancedInterpolation.H"
#include "ITLMaterialInterface.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace stressModels
{

/*---------------------------------------------------------------------------*\
                        Class unsIncrTotalLagrangianStress Declaration
\*---------------------------------------------------------------------------*/

class unsIncrTotalLagrangianStress
:
    public stressModel
{
    // Private data

        //- Displacement increment field
        volVectorField DD_;

        //- Displacement field
        volVectorField D_;

//         //- Velocity increment field
//         volVectorField DU_;

        //- Velocity field
        volVectorField U_;

        //- Point mesh
        pointMesh pMesh_;

        //- Point displacement increment field
        pointVectorField pointDD_;

        //- Point displacement field
        pointVectorField pointD_;

        //- Stress increment field
        volSymmTensorField DSigma_;

        //- Stress field
        volSymmTensorField sigma_;

        //- Strain increment field
        volSymmTensorField DEpsilon_;

        //- Strain increment field
        surfaceSymmTensorField DEpsilonf_;

        //- Strain field
        volSymmTensorField epsilon_;

        //- Strain field
        surfaceSymmTensorField epsilonf_;

        //- Plastic strain field
        volSymmTensorField epsilonP_;

        //- Plastic strain field
        surfaceSymmTensorField epsilonPf_;

        //- Stress increment field
        surfaceSymmTensorField DSigmaf_;

        //- Stress field
        surfaceSymmTensorField sigmaf_;

        //- Rheology model
        constitutiveModel rheology_;

        //- Cell-to-point interpolation
        leastSquaresVolPointInterpolation volToPoint_;

        //- Face displacement increment gradient
        surfaceTensorField gradDDf_;

        //- Face displacement gradient
        surfaceTensorField gradDf_;

        //- Cell displacement increment gradient
        volTensorField gradDD_;

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
        autoPtr<ITLMaterialInterface> interface_;

        //- Current time index
        label curTimeIndex_;

    // Private Member Functions
    
        //- Disallow default bitwise copy construct
        unsIncrTotalLagrangianStress(const unsIncrTotalLagrangianStress&);

        //- Disallow default bitwise assignment
        void operator=(const unsIncrTotalLagrangianStress&);

public:

    //- Runtime type information
    TypeName("unsIncrTotalLagrangianStress");

    // Constructors

        //- Construct from components
        unsIncrTotalLagrangianStress(const fvMesh& mesh);


    // Destructor

        virtual ~unsIncrTotalLagrangianStress()
        {}


    // Member Functions

        // Access

            const pointMesh& pMesh() const
            {
                return pMesh_;
            }

//             volVectorField& DU()
//             {
//                 return DU_;
//             }

//             const volVectorField& DU() const
//             {
//                 return DU_;
//             }

            leastSquaresVolPointInterpolation& volToPoint()
            {
                return volToPoint_;
            }

            const leastSquaresVolPointInterpolation& volToPoint() const
            {
                return volToPoint_;
            }

            pointVectorField& pointDD()
            {
                return pointDD_;
            }

            const volSymmTensorField& DSigma() const
            {
                return DSigma_;
            }

            volSymmTensorField& DSigma()
            {
                return DSigma_;
            }

            const volSymmTensorField& sigma() const
            {
                return sigma_;
            }

            volSymmTensorField& sigma()
            {
                return sigma_;
            }

            surfaceSymmTensorField& DSigmaf()
            {
                return DSigmaf_;
            }

            const surfaceSymmTensorField& DSigmaf() const
            {
                return DSigmaf_;
            }

            const volSymmTensorField& DEpsilon() const
            {
                return DEpsilon_;
            }

            volSymmTensorField& DEpsilon()
            {
                return DEpsilon_;
            }

            const surfaceSymmTensorField& DEpsilonf() const
            {
                return DEpsilonf_;
            }

            surfaceSymmTensorField& DEpsilonf()
            {
                return DEpsilonf_;
            }

            const volScalarField& rho() const
            {
                return rho_;
            }

            const volScalarField& mu() const
            {
                return mu_;
            }

            const surfaceScalarField& muf() const
            {
                return muf_;
            }

            const volScalarField& lambda() const
            {
                return lambda_;
            }

            const surfaceScalarField& lambdaf() const
            {
                return lambdaf_;
            }

            const volVectorField& DD() const
            {
                return DD_;
            }

            volVectorField& DD()
            {
                return DD_;
            }

            const volVectorField& D() const
            {
                return D_;
            }

            volVectorField& D()
            {
                return D_;
            }

            const surfaceSymmTensorField& sigmaf() const
            {
                return sigmaf_;
            }

            surfaceSymmTensorField& sigmaf()
            {
                return sigmaf_;
            }

            const volSymmTensorField& epsilon() const
            {
                return epsilon_;
            }

            volSymmTensorField& epsilon()
            {
                return epsilon_;
            }

            const surfaceSymmTensorField& epsilonf() const
            {
                return epsilonf_;
            }

            surfaceSymmTensorField& epsilonf()
            {
                return epsilonf_;
            }

            virtual const constitutiveModel& rheology() const
            {
                return rheology_;
            }

            constitutiveModel& rheology()
            {
                return rheology_;
            }

            //- Return point displacement field
            virtual const pointVectorField& pointDD() const
            {
                return pointDD_;
            }

            //- Return point displacement field
            virtual const pointVectorField& pointD() const
            {
                return pointD_;
            }

            //- Return point displacement field
            pointVectorField& pointD()
            {
                return pointD_;
            }

            surfaceTensorField& gradDDf()
            {
                return gradDDf_;
            }

            const surfaceTensorField& gradDDf() const
            {
                return gradDDf_;
            }

            surfaceTensorField& gradDf()
            {
                return gradDf_;
            }

            const surfaceTensorField& gradDf() const
            {
                return gradDf_;
            }

            const volTensorField& gradDD() const
            {
                return gradDD_;
            }

            volTensorField& gradDD()
            {
                return gradDD_;
            }

            const volTensorField& gradD() const
            {
                return gradD_;
            }

            volTensorField& gradD()
            {
                return gradD_;
            }

            //- Return multi-material interface pointer
            const autoPtr<ITLMaterialInterface>& interface() const
            {
                return interface_;
            }

            //- Return multi-material interface pointer
            autoPtr<ITLMaterialInterface>& interface()
            {
                return interface_;
            }

            //- Return point velocity field
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

            //- Return displacement field (current solution)
            virtual const volVectorField& solution() const
            {
                return DD_;
            }

            //- Return displacement field (current solution)
            virtual volVectorField& solution()
            {
                return DD_;
            }

            //- Initialise solution
            virtual void initialise
            (
                const volVectorField& sol,
                bool consistentBoundaryField
            );

            //- Return current residual
            virtual tmp<volVectorField> residual
            (
                const volVectorField& source
            );

            //- Update total fields
            virtual void updateTotalFields();

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
