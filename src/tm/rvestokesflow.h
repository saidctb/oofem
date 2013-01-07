/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2012   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef RVESTOKESFLOW_H_
#define RVESTOKESFLOW_H_

#include "rvematerial.h"
#include "flotarry.h"
#include "flotmtrx.h"
#include "oofem_limits.h"
#include "transportmaterial.h"

namespace oofem {
/**
 * Material status class for the rveStokesFlow class.
 *
 * @author Carl Sandström
 */
class rvestokesflowMaterialStatus : public TransportMaterialStatus
{
protected:

    FloatArray gradPVector, temp_gradPVector;
    FloatArray velocityVector, temp_velocityVector;
    FloatMatrix temp_TangentMatrix, tangentMatrix;
    FloatArray *solutionVector;

public:

    rvestokesflowMaterialStatus(int n, Domain *d, GaussPoint *g);

    ~rvestokesflowMaterialStatus();

    virtual void initTempStatus();

    virtual void updateYourself(TimeStep *);

    contextIOResultType    saveContext(DataStream *stream, ContextMode mode, void *obj = NULL);
    contextIOResultType    restoreContext(DataStream *stream, ContextMode mode, void *obj = NULL);

    const FloatArray &giveGradP()                                       { return gradPVector; }
    const FloatArray &giveTempGradP()                                   { return temp_gradPVector; }
    const FloatArray &giveVelocityVector()                              { return velocityVector; }
    const FloatArray &giveTempVelocityVector()                          { return temp_velocityVector; }
    const FloatMatrix &giveTangentMatrix()                              { return tangentMatrix; }
    const FloatMatrix &giveTempTangentMatrix()                          { return temp_TangentMatrix; }

    void letTempGradPVectorBe(const FloatArray &v)                      { temp_gradPVector = v; }
    void letTempVelocityVectorBe(const FloatArray &v)                   { temp_velocityVector = v; }
    void letTempTangentMatrixBe(const FloatMatrix &K)                   { temp_TangentMatrix = K; }

    /** Export this RVE. The files produced is named ./[.in-file].rve/Rve_[ID]_[GP number] where is is the global element number anf GP number is
     * the number of the Gausspoint where the RVE is evaluated
     */
    void exportFilter(EngngModel *E, GaussPoint *gp, TimeStep *tStep);

    const char *giveClassName() const { return "rvestokesflowMaterialStatus"; }
    classType giveClassID() const { return RVEStokesFlowMaterialStatusClass; }
};


/**
 * Material class using an external .in file as a description of the substructure of a transport problem (in this case seepage). The external .in
 * file must be of the stokesflowvelocityhomogenization type.
 *
 * The response is computed by applying a pressure gradient over the domain and after solving, computing the mean velocity (which is the seepage
 * velocity).
 *
 * @author Carl Sandström
 */
class rvestokesflow : public RVEMaterial, public TransportMaterial
{
private:
    void exportFilter(EngngModel *E, GaussPoint *gp, TimeStep *tStep);

public:
    rvestokesflow(int n, Domain *d);

    ~rvestokesflow() { };

    IRResultType initializeFrom(InputRecord *ir);

    virtual void giveFluxVector(FloatArray &answer, GaussPoint *gp, const FloatArray &eps, TimeStep *tStep);
    virtual void giveCharacteristicMatrix(FloatMatrix & answer,  MatResponseForm form, MatResponseMode, GaussPoint * gp, TimeStep * atTime);
    virtual double giveCharacteristicValue(MatResponseMode mode, GaussPoint *gp, TimeStep *atTime) {return 0.0;};

    virtual MaterialStatus *CreateStatus(GaussPoint *gp) const;

    virtual int giveIPValue(FloatArray &answer, GaussPoint *aGaussPoint, InternalStateType type, TimeStep *atTime);

    const char *giveClassName() const { return "rvestokesflow"; }
    classType  giveClassID() const { return RVEStokesFlowClass; }
};
}

#endif /* RVEMATERIAL_H_ */