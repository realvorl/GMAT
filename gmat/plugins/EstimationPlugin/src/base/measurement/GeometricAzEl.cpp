//$Id: GeometricAzEl.cpp 1398 2011-04-21 20:39:37Z ljun@NDC $
//------------------------------------------------------------------------------
//                         GeometricAzEl
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CA54C
//
// Author: Darrel J. Conway, Thinking Systems, Inc./Wendy Shoan, GSFC/GSSB
// Created: 2009.08.24
//
/**
 * Implementation of the geometric range rate measurement.
 */
//------------------------------------------------------------------------------


#include "GeometricAzEl.hpp"
#include "MessageInterface.hpp"
#include "MeasurementException.hpp"

#include "Rvector3.hpp"
#include "RealUtilities.hpp"

using namespace GmatMathUtil;
//#define DEBUG_AZEL_CALC
//#define SHOW_AZEL_CALC


GeometricAzEl::GeometricAzEl(const std::string &name) :
   GeometricMeasurement          ("GeometricAzEl", name)
{
   objectTypeNames.push_back("GeometricAzEl");

   // Prep value array in measurement
   currentMeasurement.value.push_back(0.0);
   currentMeasurement.typeName = "GeometricAzEl";
   currentMeasurement.type = Gmat::GEOMETRIC_AZ_EL;
//   parameterCount = GeometricGeometricAzElParamCount;
   measurementSize = 2;

   covariance.SetDimension(2);
   covariance(0,0) = covariance(1,1) = 1.0;
}


GeometricAzEl::~GeometricAzEl()
{
}


GeometricAzEl::GeometricAzEl(const GeometricAzEl &meas) :
   GeometricMeasurement          (meas)
{
   currentMeasurement.value.push_back(0.0);
   currentMeasurement.value.push_back(0.0);
   currentMeasurement.typeName = "GeometricAzEl";
   currentMeasurement.type = Gmat::GEOMETRIC_AZ_EL;
   currentMeasurement.participantIDs.push_back("NotSet");
   currentMeasurement.participantIDs.push_back("NotSet");
   currentMeasurement.uniqueID = meas.currentMeasurement.uniqueID;

   covariance = meas.covariance;
}


GeometricAzEl& GeometricAzEl::operator=(const GeometricAzEl &meas)
{
   if (&meas != this)
   {
      GeometricMeasurement::operator=(meas);

      // Allocate exactly one value in current measurement for range
      currentMeasurement.value.clear();
      currentMeasurement.value.push_back(0.0);
      currentMeasurement.value.push_back(0.0);
      currentMeasurement.typeName = "GeometricAzEl";
      currentMeasurement.type = Gmat::GEOMETRIC_AZ_EL;
      currentMeasurement.uniqueID = meas.currentMeasurement.uniqueID;

      covariance = meas.covariance;
   }

   return *this;
}


GmatBase* GeometricAzEl::Clone() const
{
   #ifdef DEBUG_AZEL_CALC
      MessageInterface::ShowMessage("Entered GeometricAzEl::Clone() "
            "with %d participants; this = %p\n", participants.size(), this);
   #endif
   GmatBase *retval =  new GeometricAzEl(*this);
   #ifdef DEBUG_AZEL_CALC
      MessageInterface::ShowMessage("   clone address is %p\n", retval);
   #endif
   return retval;
}


bool GeometricAzEl::Initialize()
{
   #ifdef DEBUG_AZ_EL_CALC
      MessageInterface::ShowMessage("Entered GeometricAzEl::Initialize()"
            "; this = %p\n", this);
   #endif
   
   bool retval = false;
   
   if (GeometricMeasurement::Initialize())
      retval = true;
   
   #ifdef DEBUG_AZ_EL_CALC
      MessageInterface::ShowMessage("   Initialization %s with %d "
            "participants\n", (retval ? "succeeded" : "failed"),
            participants.size());
   #endif
   
      return retval;
}


bool GeometricAzEl::Evaluate(bool withEvents)
{

   #ifdef DEBUG_AZ_EL_CALC
      MessageInterface::ShowMessage("Entered GeometricAzEl::Evaluate()\n");
      MessageInterface::ShowMessage("  ParticipantCount: %d\n",
            participants.size());
   #endif
        
   if (!initialized) InitializeMeasurement();
   
   CalculateRangeRateVectorObs();
   
   currentMeasurement.feasibilityValue = rangeVecInertial * p1Loc; 
   #ifdef DEBUG_AZ_EL_CALC
      MessageInterface::ShowMessage("   feasibility = %12.10f\n",
            currentMeasurement.feasibilityValue);
      for (unsigned int ii = 0; ii < 3; ii++)
         MessageInterface::ShowMessage(" range vector (obs) [%d] = %12.10f\n",
               (Integer) ii, rangeVecObs[ii]);
   #endif
   if ((currentMeasurement.feasibilityValue > 0.0) && Abs(rangeVecObs[0]) >= 1.0e-8)
   {
      currentMeasurement.isFeasible = true;
      // Compute elevation first
      Real     range     = rangeVecObs.GetMagnitude();
      currentMeasurement.value[1] = ASin(rangeVecObs[2] / range);
      // Then compute the azimuth
      if (IsEqual(currentMeasurement.value[1], GmatMathConstants::PI_OVER_TWO))
      {
         std::string errmsg = "Error computing azimuth - elevation is 90 degrees\n";
         throw MeasurementException(errmsg);
      }
      currentMeasurement.value[0] = ATan(rangeVecObs[1], -rangeVecObs[0]);
   }
   else
   {
      currentMeasurement.isFeasible = false;
      currentMeasurement.value[0] = 0.0; 
      currentMeasurement.value[1] = 0.0; 
   }
   
   #ifdef DEBUG_AZ_EL_CALC
      MessageInterface::ShowMessage("Calculating Az/El at epoch %.12lf\n",
            currentMeasurement.epoch);
      MessageInterface::ShowMessage("   Location of %s, id = '%s':  %s",
            participants[0]->GetName().c_str(),
            currentMeasurement.participantIDs[0].c_str(),
            p1Loc.ToString().c_str());
      MessageInterface::ShowMessage("   Location of %s, id = '%s':  %s",
            participants[1]->GetName().c_str(),
            currentMeasurement.participantIDs[1].c_str(),
            p2Loc.ToString().c_str());
      MessageInterface::ShowMessage("   Range Vector (Inertial):  %s\n",
            rangeVecInertial.ToString().c_str());
      MessageInterface::ShowMessage("   Range Vector (Observation):  %s\n",
            rangeVecObs.ToString().c_str());
      MessageInterface::ShowMessage("   Range Rate ector (Observation):  %s\n",
            rangeRateVecObs.ToString().c_str());
      MessageInterface::ShowMessage("   R(Groundstation) dot RangeVec =  %lf\n", // ******** ??
            currentMeasurement.feasibilityValue);
      MessageInterface::ShowMessage("   Feasibility:  %s\n",
            (currentMeasurement.isFeasible ? "true" : "false"));
      MessageInterface::ShowMessage("   Azimuth is %.12lf\n",
            currentMeasurement.value[0]);
      MessageInterface::ShowMessage("   Elevation is %.12lf\n",
            currentMeasurement.value[1]);
   #endif
   
   #ifdef SHOW_AZ_EL_CALC
      MessageInterface::ShowMessage("Az/El at epoch %.12lf is ",
            currentMeasurement.epoch);
      if (currentMeasurement.isFeasible)
         MessageInterface::ShowMessage("feasible, azimuth = %.12lf, elevation = %.12lf\n",
            currentMeasurement.value[0],currentMeasurement.value[1]);
      else
         MessageInterface::ShowMessage("not feasible\n");
   #endif

   return true;
}

const std::vector<RealArray>& GeometricAzEl::CalculateMeasurementDerivatives(GmatBase *obj, Integer id) // *****TBD******
{
   #ifdef DEBUG_DERIVATIVES
      MessageInterface::ShowMessage("GeometricAzEl::CalculateMeasurement"
            "Derivatives(%s, %d) called\n", obj->GetName().c_str(), id);
   #endif
      
   if (!initialized) InitializeMeasurement();
   
   GmatBase *objPtr = NULL;
   
   Integer size = obj->GetEstimationParameterSize(id);
   Integer objNumber = -1;
   #ifdef DEBUG_DERIVATIVES
      MessageInterface::ShowMessage("   ParameterSize = %d\n", size);
   #endif
   
   if (size <= 0)
      throw MeasurementException("The derivative parameter on derivative "
            "object " + obj->GetName() + "is not recognized");
   
   // Check to see if obj is a participant
   for (UnsignedInt i = 0; i < this->participants.size(); ++i)
   {
      if (participants[i] == obj)
      {
         objPtr = participants[i];
         objNumber = i + 1;
         #ifdef DEBUG_DERIVATIVES
            MessageInterface::ShowMessage("   Participant %s found\n",
                  objPtr->GetName().c_str());
         #endif
         break;
      }
   }
   
   // Or if it is the measurement model for this object
   if (obj->IsOfType(Gmat::MEASUREMENT_MODEL))
      if (obj->GetRefObject(Gmat::CORE_MEASUREMENT, "") == this)
      {
         objPtr = obj;
         objNumber = 0;
         #ifdef DEBUG_DERIVATIVES
            MessageInterface::ShowMessage("   The measurement is the object\n",
                  objPtr->GetName().c_str());
         #endif
      }
   if (objNumber == -1)
      throw MeasurementException(
            "GeometricAzEl error - object is neither participant nor measurement model.");
   
   RealArray oneRow;
   oneRow.assign(size, 0.0);
   currentDerivatives.clear();
   // Push 2 rows onto the derivative structure, one per angle
   currentDerivatives.push_back(oneRow);
   currentDerivatives.push_back(oneRow);
   
   Integer parameterID = GetParmIdFromEstID(id, obj);
   
   #ifdef DEBUG_DERIVATIVES
      MessageInterface::ShowMessage("   Looking up id %d\n", parameterID);
   #endif

   if (objPtr != NULL)
   {
      Rvector3  tmp, tmpVel;
      Rvector6  result;  
      if (objNumber == 1) // participant 1, either a GroundStation or a Spacecraft
      {
         if (objPtr->GetParameterText(parameterID) == "Position")
         {
            CalculateRangeVectorObs();
            Real      elevation     = currentMeasurement.value[1];
            Real      azimuth       = currentMeasurement.value[0];
            Rvector3  rangeUnit     = rangeVecObs.GetUnitVector();
            Real      range         = rangeVecObs.GetMagnitude();
            Rmatrix   po( 1,3,rangeUnit[0], rangeUnit[1], rangeUnit[2]); 
            Rmatrix   poT(3,1,rangeUnit[0], rangeUnit[1], rangeUnit[2]);
            Rmatrix   xT (3,1,1.0,0.0,0.0);
            
            if (stationParticipant)
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        ((I33 - po * poT) * R_o_1);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = - tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))) * R_o_1);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = tmp[ii];
            }
            else
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        (I33 - po * poT);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = - tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))));
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = tmp[ii];
            }
         }
         else if (objPtr->GetParameterText(parameterID) == "Velocity")
         {
               for (UnsignedInt ii = 0; ii < 3; ii++)
               {
                  currentDerivatives[0][ii]   = zeroVector[ii];
                  currentDerivatives[1][ii]   = zeroVector[ii];
               }
          }
         else if (objPtr->GetParameterText(parameterID) == "CartesianX")
         {
            CalculateRangeVectorObs();
            Real      elevation     = currentMeasurement.value[1];
            Real      azimuth       = currentMeasurement.value[0];
            Rvector3  rangeUnit     = rangeVecObs.GetUnitVector();
            Real      range         = rangeVecObs.GetMagnitude();
            Rmatrix   po( 1,3,rangeUnit[0], rangeUnit[1], rangeUnit[2]); 
            Rmatrix   poT(3,1,rangeUnit[0], rangeUnit[1], rangeUnit[2]);
            Rmatrix   xT (3,1,1.0,0.0,0.0);
            
            if (stationParticipant)
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        ((I33 - po * poT) * R_o_1);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = - tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))) * R_o_1);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = tmp[ii];
            }
            else
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        (I33 - po * poT);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = - tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))));
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = tmp[ii];
            }
            // velocity is zeroVector
            for (UnsignedInt ii = 0; ii < 3; ii++)
            {
               currentDerivatives[0][ii+3]   = zeroVector[ii];
               currentDerivatives[1][ii+3]   = zeroVector[ii];
            }
          }
         else if (objPtr->GetParameterText(parameterID) == "Bias")
         {
            for (Integer i = 0; i < size; ++i) currentDerivatives[0][i] = 1.0;
         }
         else
         {
            for (UnsignedInt i = 0; i < 3; ++i)  currentDerivatives[0][i] = 0.0;
         }
      }
      else if (objNumber == 2) // participant 2, always a Spacecraft
      {
         if (objPtr->GetParameterText(parameterID) == "Position")
         {
            CalculateRangeVectorObs();
            Real      elevation     = currentMeasurement.value[1];
            Real      azimuth       = currentMeasurement.value[0];
            Rvector3  rangeUnit     = rangeVecObs.GetUnitVector();
            Real      range         = rangeVecObs.GetMagnitude();
            Rmatrix   po( 1,3,rangeUnit[0], rangeUnit[1], rangeUnit[2]); 
            Rmatrix   poT(3,1,rangeUnit[0], rangeUnit[1], rangeUnit[2]);
            Rmatrix   xT (3,1,1.0,0.0,0.0);
            
            if (stationParticipant)
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        ((I33 - po * poT) * R_o_2);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))) * R_o_2);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = - tmp[ii];
            }
            else
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        (I33 - po * poT);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))));
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = - tmp[ii];
            }
         }
         else if (objPtr->GetParameterText(parameterID) == "Velocity")
         {
               for (UnsignedInt ii = 0; ii < 3; ii++)
               {
                  currentDerivatives[0][ii]   = zeroVector[ii];
                  currentDerivatives[1][ii]   = zeroVector[ii];
               }
          }
         else if (objPtr->GetParameterText(parameterID) == "CartesianX")
         {


            //            // Temporary: GTDS 7-21 ...
            //            Real xysum = obsRange[0]*obsRange[0] + obsRange[1]*obsRange[1];
            //            currentDerivatives[0][0] =  obsRange[1] / xysum;
            //            currentDerivatives[0][1] = -obsRange[0] / xysum;
            //            currentDerivatives[0][2] = 0.0;
            //
            //            // ... and 7-22
            //            Real sqrtxysum = GmatMathUtil::Sqrt(xysum);
            //            Real overRho = 1.0 / (range*range*sqrtxysum);
            //            currentDerivatives[1][0] = -obsRange[0] * obsRange[2] * overRho;
            //            currentDerivatives[1][1] = -obsRange[0] * obsRange[2] * overRho;
            //            currentDerivatives[1][2] = sqrtxysum / (range*range);

            CalculateRangeVectorObs();
            Real      elevation     = currentMeasurement.value[1];
            Real      azimuth       = currentMeasurement.value[0];
            Rvector3  rangeUnit     = rangeVecObs.GetUnitVector();
            Real      range         = rangeVecObs.GetMagnitude();
            Rmatrix   po( 1,3,rangeUnit[0], rangeUnit[1], rangeUnit[2]); 
            Rmatrix   poT(3,1,rangeUnit[0], rangeUnit[1], rangeUnit[2]);
            Rmatrix   xT (3,1,1.0,0.0,0.0);
            
            if (stationParticipant)
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        ((I33 - po * poT) * R_o_2);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))) * R_o_2);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = - tmp[ii];
            }
            else
            {
               // Elevation
               tmp    = (1 / Cos(elevation)) * (zUnit / range) * 
                        (I33 - po * poT);
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[1][ii]   = tmp[ii];
               // Azimuth
               tmp    = (1 * Cos(azimuth) * Cos(azimuth)) * (yUnit / (xT * po)) *
                        ((I33 + ((po * xT) / (xT * po))));
               for (UnsignedInt ii = 0; ii < 3; ii++)
                  currentDerivatives[0][ii]   = - tmp[ii];
            }
            // velocity is zeroVector
            for (UnsignedInt ii = 0; ii < 3; ii++)
            {
               currentDerivatives[0][ii+3]   = zeroVector[ii];
               currentDerivatives[1][ii+3]   = zeroVector[ii];
            }
          }
         else if (objPtr->GetParameterText(parameterID) == "Bias")
         {
            for (Integer i = 0; i < size; ++i) currentDerivatives[0][i] = 1.0;
         }
         else
         {
            for (UnsignedInt i = 0; i < 3; ++i)  currentDerivatives[0][i] = 0.0;
         }
      }
      else if (objNumber == 0) // measurement model
      {
         if (objPtr->GetParameterText(parameterID) == "Bias")
         {
            for (Integer i = 0; i < size; ++i) currentDerivatives[0][i] = 1.0;
         }
      }
      else
      {
         for (UnsignedInt i = 0; i < 3; ++i)  currentDerivatives[0][i] = 0.0;
      }

   }
   return currentDerivatives;

}
