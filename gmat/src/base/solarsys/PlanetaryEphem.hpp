//$Id: PlanetaryEphem.hpp 11016 2012-10-24 18:32:36Z wendys-dev $
//------------------------------------------------------------------------------
//                                  PlanetaryEphem
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
// Created: 2004/02/18
//
/**
 * This is the base class that defines the interface to planetary ephemeris
 * files.
 */
//------------------------------------------------------------------------------
#ifndef PlanetaryEphem_hpp
#define PlanetaryEphem_hpp

#include "gmatdefs.hpp"
#include "A1Mjd.hpp"

namespace Gmat
{
   /// different types of DE files
   enum DeFileType
   {
      DE_DE405,
      DE_DE421,
      DE_DE424,
   };

   /// DE file can be ASCII or BINARY
   enum DeFileFormat
   {
      DE_ASCII = 0,
      DE_BINARY
   };
};

class GMAT_API PlanetaryEphem
{
public:

   // default constructor
   PlanetaryEphem(std::string withFileName);
   // copy constructor
   PlanetaryEphem(const PlanetaryEphem& pef);
   // operator=
   PlanetaryEphem& operator=(const PlanetaryEphem& pef);
   // destructor
   virtual ~PlanetaryEphem();

   // method to return the full path name of the planetary ephem file.
   std::string GetName() const;

   //------------------------------------------------------------------------------
   //  Integer GetBodyID(std::string bodyName)
   //------------------------------------------------------------------------------
   /**
    * Returns the body ID number for the specified body.
    *
    * @param <bodyName> body whose ID is requested.
    *
    * @return the body ID number for the specified body.
    */
   //------------------------------------------------------------------------------
   virtual Integer GetBodyID(std::string bodyName) = 0;

   //------------------------------------------------------------------------------
   //  Real* GetPosVel(Integer forBody, A1Mjd atTime)
   //------------------------------------------------------------------------------
   /**
    * Returns the the position and velocity of the specified body
    * at the specified time.
    *
    * @param forBody            body number for which to return the state
    * @param atTime             time at which to get the state for the body
    * @param overrideTimeSystem override the TDB time used, with TT time
    *
    * @return the position and velocity of the specified body at the requested
    *         time.
    */
   //------------------------------------------------------------------------------
   virtual Real* GetPosVel(Integer forBody, A1Mjd atTime,
                           bool overrideTimeSystem = false) = 0;

   //------------------------------------------------------------------------------
   //  Integer* GetStartDayAndYear()
   //------------------------------------------------------------------------------
   /**
    * Returns the day-of-year and year of the start time of the file.
    *
    * @return the day-of-year and year of the start time of the file..
    */
   //------------------------------------------------------------------------------
   virtual Integer* GetStartDayAndYear() = 0;


protected:
   // file type (from Swingby - added constructors, operator=, destructor)
   struct GMAT_API dcb_type
   {
      dcb_type()
      {
         strcpy(full_path, "\0");
         recl  = 0;
         fptr  = NULL;
      }
      dcb_type(const dcb_type& dcb)
      {
         strcpy(full_path, dcb.full_path);
         recl  = dcb.recl;
         fptr  = dcb.fptr;
      }
      dcb_type& operator=(const dcb_type dcb)
      {
         strcpy(full_path, dcb.full_path);
         recl  = dcb.recl;
         fptr  = dcb.fptr;
         return *this;
      }
      char  full_path[261];
      long   recl;
      FILE  *fptr;
   };

   double      jdMjdOffset;

   std::string itsName;

   // file data type
   dcb_type g_pef_dcb;

private:

};
#endif // PlanetaryEphem_hpp
