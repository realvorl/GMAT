//$Id: MOEEqAxes.hpp 10889 2012-09-19 16:51:59Z wendys-dev $
//------------------------------------------------------------------------------
//                                  MOEEqAxes
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under 
// MOMS Task order 124.
//
// Author: Wendy C. Shoan/GSFC/MAB
// Created: 2005/04/28
//
/**
 * Definition of the MOEEqAxes class.
 *
 */
//------------------------------------------------------------------------------

#ifndef MOEEqAxes_hpp
#define MOEEqAxes_hpp

#include "gmatdefs.hpp"
#include "GmatBase.hpp"
#include "AxisSystem.hpp"
#include "InertialAxes.hpp"

class GMAT_API MOEEqAxes : public InertialAxes
{
public:

   // default constructor
   MOEEqAxes(const std::string &itsName = "");
   // copy constructor
   MOEEqAxes(const MOEEqAxes &moe);
   // operator = for assignment
   const MOEEqAxes& operator=(const MOEEqAxes &moe);
   // destructor
   virtual ~MOEEqAxes();
   
   // method to initialize the data
   virtual bool Initialize();

   virtual GmatCoordinate::ParameterUsage UsesEpoch() const;
   
   // all classes derived from GmatBase must supply this Clone method;
   // this must be implemented in the 'leaf' classes
   virtual GmatBase*       Clone(void) const;
   
protected:

   enum
   {
      MOEEqAxesParamCount = InertialAxesParamCount,
   };
   
   virtual void CalculateRotationMatrix(const A1Mjd &atEpoch,
                                        bool forceComputation = false);
};
#endif // MOEEqAxes_hpp
