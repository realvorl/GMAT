//$Id: ManageObject.hpp 10048 2011-12-06 16:58:22Z lindajun $
//------------------------------------------------------------------------------
//                                 ManageObject
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Wendy C. Shoan
// Created: 2008.03.12
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CCA54C
//
/**
 * Class declaration for the ManageObject command- this is a base class for the 
 * Create and GLobal commands
 */
//------------------------------------------------------------------------------


#ifndef ManageObject_hpp
#define ManageObject_hpp

#include "GmatCommand.hpp"


/**
 * Base class for Create and Global commands
 */
class GMAT_API ManageObject : public GmatCommand
{
public:
   ManageObject(const std::string &typeStr);
   virtual ~ManageObject();
   ManageObject(const ManageObject &mo);
   ManageObject&        operator=(const ManageObject &mo);
   
   // Parameter access methods - overridden from GmatBase
   virtual std::string  GetParameterText(const Integer id) const;
   virtual Integer      GetParameterID(const std::string &str) const;
   virtual Gmat::ParameterType
                        GetParameterType(const Integer id) const;
   virtual std::string  GetParameterTypeString(const Integer id) const;

   virtual std::string  GetStringParameter(const Integer id) const;
   virtual std::string  GetStringParameter(const Integer id,
                                           const Integer index) const;
   virtual std::string  GetStringParameter(const std::string &label,
                                           const Integer index) const;
   virtual bool         SetStringParameter(const Integer id, 
                                           const std::string &value);
   virtual bool         SetStringParameter(const std::string &label, 
                                           const std::string &value);
   virtual bool         SetStringParameter(const Integer id, 
                                           const std::string &value,
                                           const Integer index);
   virtual bool         SetStringParameter(const std::string &label, 
                                           const std::string &value,
                                           const Integer index);
   virtual const StringArray& 
                        GetStringArrayParameter(const Integer id) const;

   virtual const std::string&  
                        GetGeneratingString(
                           Gmat::WriteMode mode = Gmat::SCRIPTING,
                           const std::string &prefix = "",
                           const std::string &useName = "");
	
   virtual bool         TakeAction(const std::string &action,  
                                   const std::string &actionData = "");
	
   virtual bool         Initialize();

protected:
   enum
   {
      OBJECT_NAMES = GmatCommandParamCount, 
      ManageObjectParamCount
   };
   static const std::string PARAMETER_TEXT[ManageObjectParamCount - GmatCommandParamCount];

   static const Gmat::ParameterType PARAMETER_TYPE[ManageObjectParamCount - GmatCommandParamCount];
  
   /// array of names for the objects to manage
   StringArray objectNames;
   
   bool InsertIntoGOS(GmatBase *obj, const std::string &withName);

};

#endif /* ManageObject_hpp */
