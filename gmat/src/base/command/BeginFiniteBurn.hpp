//$Id: BeginFiniteBurn.hpp 10748 2012-08-14 22:02:26Z djcinsb $
//------------------------------------------------------------------------------
//                            BeginFiniteBurn
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under MOMS task
// order 124.
//
// Author: Darrel J. Conway, Thinking Systems, Inc.
// Created: 2005/01/04
//
/**
 *  Definition code for the BeginFiniteBurn command.
 */
//------------------------------------------------------------------------------

#ifndef BeginFiniteBurn_hpp
#define BeginFiniteBurn_hpp


#include "GmatCommand.hpp"
#include "FiniteBurn.hpp"
#include "FiniteThrust.hpp"
#include "Spacecraft.hpp"
#include "Thruster.hpp"


/**
 * BeginFiniteBurn Class, used to turn on thrusters used in a finite burn.
 */
class GMAT_API BeginFiniteBurn : public GmatCommand
{
public:
   BeginFiniteBurn();
   virtual ~BeginFiniteBurn();
   BeginFiniteBurn(const BeginFiniteBurn& begman);
   BeginFiniteBurn&       operator=(const BeginFiniteBurn& begman);
   
   virtual bool         TakeAction(const std::string &action,  
                           const std::string &actionData = "");
   virtual std::string  GetRefObjectName(const Gmat::ObjectType type) const;
   virtual const ObjectTypeArray&
                        GetRefObjectTypeArray();
   virtual const StringArray&
                        GetRefObjectNameArray(const Gmat::ObjectType type);
   virtual bool         SetRefObjectName(const Gmat::ObjectType type,
                                        const std::string &name);
   
   virtual GmatBase*    GetGmatObject(const Gmat::ObjectType type, 
                                  const std::string objName = "");

   virtual GmatBase*    Clone() const;
   virtual bool         RenameRefObject(const Gmat::ObjectType type,
                                        const std::string &oldName,
                                        const std::string &newName);
   virtual const std::string&
                        GetGeneratingString(
                           Gmat::WriteMode mode = Gmat::SCRIPTING,
                           const std::string &prefix = "",
                           const std::string &useName = "");
   
   virtual void         SetTransientForces(std::vector<PhysicalModel*> *tf);
   
   virtual bool         Initialize();
   virtual bool         Execute();
   
   DEFAULT_TO_NO_CLONES

protected:
   /// Name of the FiniteBurn object used to set the maneuver details
   std::string                   burnName;
   /// The FiniteBurn object
   FiniteBurn                    *maneuver;
   /// The FiniteThrust that is available for the force models
   FiniteThrust                  *burnForce;
   /// The vector of forces managed by the Sandbox
   std::vector<PhysicalModel*>   *transientForces;
   /// The names of the spacecraft that get maneuvered
   StringArray                   satNames;
   /// The spacecraft that get maneuvered
   std::vector<Spacecraft *>     sats;
   /// The thrusters that get activated
   std::vector<Thruster *>       thrusters;

   void ValidateThrusters();
   
};

#endif // BeginFiniteBurn_hpp
