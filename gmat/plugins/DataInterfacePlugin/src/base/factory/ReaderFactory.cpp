//$Id: ReaderFactory.cpp 11978 2013-06-24 15:15:23Z wendys-dev $
//------------------------------------------------------------------------------
//                         ReaderFactory
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS
// contract, Task Order 28
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: September 20, 2012
//
/**
 * Implementation of a factory used in a GMAT plugin
 */
//------------------------------------------------------------------------------


#include "ReaderFactory.hpp"
#include "MessageInterface.hpp"

// Supported subtypes
#include "TcopsVHFAscii.hpp"

//------------------------------------------------------------------------------
// ReaderFactory()
//------------------------------------------------------------------------------
/**
 * Constructor
 */
//------------------------------------------------------------------------------
ReaderFactory::ReaderFactory() :
   Factory           (Gmat::INTERFACE)
{
   if (creatables.empty())
   {
      creatables.push_back("TVHF_ASCII");
   }
}


//------------------------------------------------------------------------------
// ~ReaderFactory()
//------------------------------------------------------------------------------
/**
 * Destructor
 */
//------------------------------------------------------------------------------
ReaderFactory::~ReaderFactory()
{
}


//------------------------------------------------------------------------------
// ReaderFactory(const ReaderFactory& elf)
//------------------------------------------------------------------------------
/**
 * Copy constructor
 *
 * @param elf The factory copied here
 */
//------------------------------------------------------------------------------
ReaderFactory::ReaderFactory(const ReaderFactory& elf) :
   Factory           (elf)
{
   if (creatables.empty())
   {
      creatables.push_back("TVHF_ASCII");
   }
}


//------------------------------------------------------------------------------
// ReaderFactory& operator=(const ReaderFactory& elf)
//------------------------------------------------------------------------------
/**
 * Assignment operator
 *
 * @param elf The factory copied to this one
 *
 * @return this instance, set to match elf
 */
//------------------------------------------------------------------------------
ReaderFactory& ReaderFactory::operator=(
      const ReaderFactory& elf)
{
   if (this != &elf)
   {
      Factory::operator=(elf);

      if (creatables.empty())
      {
         creatables.push_back("TVHF_ASCII");
      }
   }

   return *this;
}


//------------------------------------------------------------------------------
//GmatBase* CreateObject(const std::string &ofType, const std::string &withName)
//------------------------------------------------------------------------------
/**
 * Returns a GmatBase object pointer to a new object.
 *
 * @param ofType   specific type of Spacecraft object to create.
 * @param withName name to give to the newly created Spacecraft object.
 *
 * @return pointer to a new Spacecraft object.
 *
 * @exception <FactoryException> thrown if the factory does not create
 * objects of the requested type.
 */
//------------------------------------------------------------------------------
GmatBase* ReaderFactory::CreateObject(const std::string &ofType,
                                const std::string &withName)
{
   GmatBase *retval = NULL;

   if (ofType == "TVHF_ASCII")
      retval = new TcopsVHFAscii(withName);

   if (retval != NULL)
   {
      if (retval->IsOfType("DataReader") == false)
      {
         delete retval;
         retval = NULL;
         MessageInterface::ShowMessage("The Reader Factory can only create "
               "DataReader subobjects; %s is not a DataReader\n", ofType.c_str());
      }
   }

   return retval;
}

