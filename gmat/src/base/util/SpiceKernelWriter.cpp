//$Id:$
//------------------------------------------------------------------------------
//                              SpiceKernelWriter
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under
// FDSS Task order 28.
//
// Author: Wendy C. Shoan
// Created: 2009.12.07
//
/**
 * Implementation of the SpiceKernelWriter, which writes SPICE data (kernel) files.
 *
 * This code creates a temporary text file, required in order to include META-Data
 * (commentary) in the SPK file.  The file is deleted from the system after the
 * commentary is added to the SPK file.  The name of this temporary text file
 * takes the form
 *       GMATtmpSPKcmmnt<objName>.txt
 * where <objName> is the name of the object for whom the SPK file is created
 *
 * If the code is unable to create the temporary file (e.g., because of a permission
 * problem), the SPK file will still be generated but will contain no META-data.
 *
 */
//------------------------------------------------------------------------------
#include <stdio.h>
#include <sstream>

#include "SpiceKernelWriter.hpp"
#include "MessageInterface.hpp"
#include "StringUtil.hpp"
#include "TimeTypes.hpp"
#include "TimeSystemConverter.hpp"
#include "UtilityException.hpp"
#include "RealUtilities.hpp"

//#define DEBUG_SPICE_KERNEL_WRITER

//---------------------------------
// static data
//---------------------------------
// none

//---------------------------------
// public methods
//---------------------------------

//------------------------------------------------------------------------------
//  SpiceKernelWriter()
//------------------------------------------------------------------------------
/**
 * This method creates an object of the SpiceKernelWriter class
 * (default constructor).
 *
 */
//------------------------------------------------------------------------------
SpiceKernelWriter::SpiceKernelWriter() :
   SpiceInterface()
{
}

//------------------------------------------------------------------------------
//  SpiceKernelWriter(const SpiceKernelWriter &copy)
//------------------------------------------------------------------------------
/**
 * This method creates an object of the SpiceKernelWriter class, with data
 * copied from the input instance.
 * (copy constructor).
 *
 */
//------------------------------------------------------------------------------
SpiceKernelWriter::SpiceKernelWriter(const SpiceKernelWriter &copy) :
   SpiceInterface(copy)
{
}

//------------------------------------------------------------------------------
//  SpiceKernelWriter& operator=(const SpiceKernelWriter &copy)
//------------------------------------------------------------------------------
/**
 * This method sets data on "this" SpiceKernelWriter instance, copying data
 * from the input instance.
 *
 */
//------------------------------------------------------------------------------
SpiceKernelWriter& SpiceKernelWriter::operator=(const SpiceKernelWriter &copy)
{
   if (&copy != this)
   {
      SpiceInterface::operator=(copy);
   }

   return *this;
}

//------------------------------------------------------------------------------
//  ~SpiceKernelWriter()
//------------------------------------------------------------------------------
/**
 * This method deletes "this" SpiceKernelWriter instance.
 * (destructor)
 *
 */
//------------------------------------------------------------------------------
SpiceKernelWriter::~SpiceKernelWriter()
{
}

