//$Id: RepeatGroundTrack.hpp 11140 2012-12-05 14:28:48Z wendys-dev $
//------------------------------------------------------------------------------
//                           RepeatGroundTrack     
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
// Author:  Evelyn Hull
// Created: 2011/07/28
//
/**
 * Definition of the Repeat Ground Track Orbit Class
 */
//------------------------------------------------------------------------------

#ifndef RepeatGroundTrack_hpp
#define RepeatGroundTrack_hpp

#include "StringUtil.hpp"

class GMAT_API RepeatGroundTrack
{
public:
   RepeatGroundTrack();
   ~RepeatGroundTrack();

   void CalculateRepeatGroundTrack(bool eccVal, Real ECC, bool incVal, Real INC, 
                                   bool rtrVal, Real revsToRepeat, bool dtrVal, 
                                   Real daysToRepeat, bool rpdVal, 
                                   Real revsPerDay);

   //accessor methods
   Real GetSMA();
   Real GetALT();
   Real GetECC();
   Real GetINC();
   Real GetROP();
   Real GetROA();
   Real GetP();
   bool IsError();
   std::string GetError();

private:
   struct orbitElements 
   {
      Real SMA;
      Real ECC;
      Real INC;
   };

   orbitElements elements;
   std::string errormsg;
   bool isError;
};

#endif

