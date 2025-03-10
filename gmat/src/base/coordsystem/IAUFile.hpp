//$Id: IAUFile.hpp 9513 2012-02-24 21:23:06Z tuandangnguyen $
//------------------------------------------------------------------------------
//                                  IAUFile.hpp
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
// Author: Tuan Nguyen (NASA/GSFC)
// Created: 2012/02/24
//
/**
 * Definition of the IAUFile class.  This is the code that provides
 * IAU2000-2006 data.  It is a singleton.
 *
 */
//------------------------------------------------------------------------------

#ifndef IAUFile_hpp
#define IAUFile_hpp

#include "gmatdefs.hpp"

class GMAT_API IAUFile
{
public:
   // Returns the instance of this singleton
   static IAUFile* Instance();
   // Initializes the IAUFile (reads it and stores the data)
   virtual void Initialize();
   // Finalizes the IAUFile
   void Finalize();

   // Get IAU2000 data for a given epoch
   bool GetIAUData(Real epoch, Real* iau, Integer dim, Integer order);

protected:

   static const Integer MAX_TABLE_SIZE;

   /// name of data file (It contains the path to the file)
   std::string      iauFileName;
   /// a table containing values of independent variables
   Real*            independence;
   /// a table containing dependent vectors
   Real**           dependences;
   /// dimension of dependent vector
   Integer          dimension;
   /// the size of the table
   Integer          tableSz;
   /// number of data points
   Integer          pointsCount;

   /// specify whether the object is initialized or not
   bool isInitialized;

   void AllocateArrays();
   void CleanupArrays();

private:
   // default constructor
   IAUFile(const std::string &fileName = "IAU_SOFA.DAT", const Integer dim = 1);
   // copy constructor - NOT IMPLEMENTED
   IAUFile(const IAUFile &iau);
   // operator = - NOT IMPLEMENTED
   const IAUFile& operator=(const IAUFile &iau);

   // destructor
   virtual ~IAUFile();
   
   static IAUFile *instance;
};
#endif // ICRFFile_hpp
