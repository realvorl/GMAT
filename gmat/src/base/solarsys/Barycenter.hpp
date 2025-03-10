//$Id: Barycenter.hpp 10819 2012-08-29 13:57:31Z wendys-dev $
//------------------------------------------------------------------------------
//                                  Barycenter
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: Wendy C. Shoan
// Created: 2005.04.05
//
/**
 * Definition of the Barycenter class.
 *
 */
//------------------------------------------------------------------------------

#ifndef Barycenter_hpp
#define Barycenter_hpp

#include "gmatdefs.hpp"
#include "GmatBase.hpp"
#include "CalculatedPoint.hpp"
#include "A1Mjd.hpp"
#include "Rmatrix.hpp"
#include "Rvector6.hpp"
#include "TimeTypes.hpp"

/**
 * Barycenter class, derived from CalculatedPoint class.
 *
 * @note Bodies are sent to an object of this class via the SetRefObject
 *       method.
 */
class GMAT_API Barycenter : public CalculatedPoint
{
public:
   // default constructor, specifying calculated point type and name
   Barycenter(const std::string &itsName = "");
   // copy constructor
   Barycenter(const Barycenter &bary);
   // operator=
   Barycenter& operator=(const Barycenter &bary);
   // destructor
   virtual ~Barycenter();
   
   virtual GmatBase*       Clone() const;
   virtual void            Copy(const GmatBase* orig);
   
   // methods inherited from SpacePoint, that must be implemented here
   virtual const Rvector6  GetMJ2000State(const A1Mjd &atTime);
   virtual const Rvector3  GetMJ2000Position(const A1Mjd &atTime);
   virtual const Rvector3  GetMJ2000Velocity(const A1Mjd &atTime);
   
   virtual bool            SetRefObject(GmatBase *obj,
                                        const Gmat::ObjectType type,
                                        const std::string &name = "");

   virtual Real            GetMass();
   virtual bool            Initialize();

   virtual StringArray     GetBuiltInNames();

   // Parameter access methods - overridden from GmatBase - not needed (no additional parameters)

protected:

   enum
   {
      BarycenterParamCount = CalculatedPointParamCount
   };

   /// If this is a built-in point, this is the SpacePoint for it
   SpacePoint                  *builtInSP;
    
private:
   // check that all bodies in the list are CelestialBody objects
   virtual void CheckBodies();

};
#endif // Barycenter_hpp

