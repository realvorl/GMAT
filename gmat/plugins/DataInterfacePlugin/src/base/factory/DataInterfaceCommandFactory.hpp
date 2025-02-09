//$Id: DataInterfaceCommandFactory.hpp 11978 2013-06-24 15:15:23Z wendys-dev $
//------------------------------------------------------------------------------
//                          DataInterfaceCommandFactory
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
/**
 * Definition of the factory used to create message writing commands in this
 * plugin
 */
//------------------------------------------------------------------------------


#ifndef DataInterfaceCommandFactory_hpp
#define DataInterfaceCommandFactory_hpp

#include "DataInterfaceDefs.hpp"
#include "Factory.hpp"


/**
 * Factory class that creates one class of your plugin's objects
 *
 * Rename this class from SaveCommandFactory( to something more descriptive of your 
 * plugin object(s).
 *
 * @note: The "SAMPLE_API" descriptor needs to match the definition in your 
 * ...Defs.hpp file
 */
class DATAINTERFACE_API DataInterfaceCommandFactory : public Factory
{
public:
   DataInterfaceCommandFactory();
   virtual ~DataInterfaceCommandFactory();
   DataInterfaceCommandFactory(const DataInterfaceCommandFactory& elf);
   DataInterfaceCommandFactory& operator=(const DataInterfaceCommandFactory& elf);

   virtual GmatCommand* CreateCommand(const std::string &ofType,
                                      const std::string &withName = "");
};

#endif /* DataInterfaceCommandFactory_hpp */
