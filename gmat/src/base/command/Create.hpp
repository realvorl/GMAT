//$Id$
//------------------------------------------------------------------------------
//                                 Create
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool.
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Wendy C. Shoan
// Created: 2008.03.14
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CCA54C
//
/**
 * Class declaration for the Create command
 */
//------------------------------------------------------------------------------


#ifndef Create_hpp
#define Create_hpp

#include "ManageObject.hpp"


/**
 * Declaration of the Create command
 */
class GMAT_API Create : public ManageObject
{
public:
   Create();
   virtual          ~Create();
   Create(const Create &cr);
   Create&        operator=(const Create &cr);
   
   // Parameter access methods - overridden from GmatBase
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
   virtual GmatBase*    GetRefObject(const Gmat::ObjectType type,
                                     const std::string &name);
   virtual bool         SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                     const std::string &name = "");
   virtual GmatBase*    Clone() const;
   
   bool                 Initialize();
   bool                 Execute();
   virtual void         RunComplete();

   DEFAULT_TO_NO_CLONES
   DEFAULT_TO_NO_REFOBJECTS

protected:
   enum
   {
      OBJECT_TYPE = ManageObjectParamCount, 
      CreateParamCount
   };
   static const std::string PARAMETER_TEXT[CreateParamCount - ManageObjectParamCount];
   
   static const Gmat::ParameterType PARAMETER_TYPE[CreateParamCount - ManageObjectParamCount];
   
   std::string  objType;
   GmatBase     *refObj;
   /// object names and sizes  when the object type is an Array
   StringArray  arrayNames;
   IntegerArray rows;
   IntegerArray columns;
   
   void SetArrayInfo();
   bool InsertIntoLOS(GmatBase *obj, const std::string &withName);
   bool InsertIntoObjectStore(GmatBase *obj, const std::string &withName);
};

#endif /* Create_hpp */
