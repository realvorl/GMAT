//$Id: SaveMission.hpp 10352 2012-04-05 20:49:42Z djcinsb $
//------------------------------------------------------------------------------
//                                  SaveMission
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Linda Jun (NASA/GSFC)
// Created: 2010/08/03
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
/**
 * Class declaration for the SaveMission command
 */
//------------------------------------------------------------------------------
#ifndef SaveMission_hpp
#define SaveMission_hpp

#include "GmatCommand.hpp"

/**
 * Command used to write whole mission to ASCII files.
 */
class GMAT_API SaveMission : public GmatCommand
{
public:
   SaveMission();
   virtual ~SaveMission();
   SaveMission(const SaveMission& sv);
   SaveMission&         operator=(const SaveMission&);
   
   // inherited from GmatCommand
   virtual bool         Execute();
   
   // inherited from GmatBase
   virtual bool         InterpretAction();
   virtual GmatBase*    Clone(void) const;
   virtual bool         RenameRefObject(const Gmat::ObjectType type,
                                        const std::string &oldName,
                                        const std::string &newName);
   
   // Parameter accessors
   virtual std::string  GetParameterText(const Integer id) const;
   virtual Integer      GetParameterID(const std::string &str) const;
   virtual Gmat::ParameterType
                        GetParameterType(const Integer id) const;
   virtual std::string  GetParameterTypeString(const Integer id) const;
   
   virtual std::string  GetStringParameter(const Integer id) const;
   virtual std::string  GetStringParameter(const std::string &label) const;
   virtual bool         SetStringParameter(const Integer id, 
                                           const std::string &value);
   virtual bool         SetStringParameter(const std::string &label, 
                                           const std::string &value);
   
   // for generating string
   virtual const std::string&
                        GetGeneratingString(
                           Gmat::WriteMode mode = Gmat::SCRIPTING,
                           const std::string &prefix = "",
                           const std::string &useName = "");
   
   DEFAULT_TO_NO_CLONES

protected:
   // Parameter IDs
   enum  
   {
      FILE_NAME = GmatCommandParamCount,
      SaveMissionParamCount
   };
   
   std::string fileName;
   
   static const std::string
      PARAMETER_TEXT[SaveMissionParamCount - GmatCommandParamCount];
   static const Gmat::ParameterType
      PARAMETER_TYPE[SaveMissionParamCount - GmatCommandParamCount];
};

#endif // SaveMission_hpp
