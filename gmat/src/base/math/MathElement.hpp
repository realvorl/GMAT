//$Id: MathElement.hpp 9513 2011-04-30 21:23:06Z djcinsb $
//------------------------------------------------------------------------------
//                                   MathElement
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG04CC06P.
//
// Author: Waka Waktola
// Created: 2006/04/04
//
/**
 * Defines the Math elements class for Math in scripts.
 */
//------------------------------------------------------------------------------

#ifndef MathElement_hpp
#define MathElement_hpp

#include "gmatdefs.hpp"
#include "GmatBase.hpp"
#include "MathNode.hpp"
#include "MathException.hpp"
#include "Parameter.hpp"
#include "Array.hpp"
#include "ElementWrapper.hpp"
#include <map>

class GMAT_API MathElement : public MathNode
{
public:
   MathElement(const std::string &typeStr, const std::string &nomme);
   virtual ~MathElement();
   MathElement(const MathElement &me);
   MathElement& operator=(const MathElement &me);
   
   // for math elemement wrappers
   ////void                 SetMathWrappers(std::map<std::string, ElementWrapper*> *wrapperMap);
   virtual void         SetMathWrappers(WrapperMap *wrapperMap);
   
   // Inherited (MathNode) methods
   virtual void         SetMatrixValue(const Rmatrix &mat);
   virtual bool         ValidateInputs();
   virtual void         GetOutputInfo(Integer &type, Integer &rowCount,
                                      Integer &colCount);
   virtual bool         SetChildren(MathNode *leftChild, MathNode *rightChild);
   virtual MathNode*    GetLeft();
   virtual MathNode*    GetRight();
   
   virtual Real         Evaluate();
   virtual Rmatrix      MatrixEvaluate();
   
   // Inherited (GmatBase) methods
   virtual bool         RenameRefObject(const Gmat::ObjectType type,
                                        const std::string &oldName,
                                        const std::string &newName);   
   virtual GmatBase*    Clone(void) const; 
   virtual GmatBase*    GetRefObject(const Gmat::ObjectType type,
                                     const std::string &name);
   virtual bool         SetRefObject(GmatBase *obj, const Gmat::ObjectType type,
                                     const std::string &name = "");
   virtual std::string  GetRefObjectName(const Gmat::ObjectType type) const;
   virtual bool         SetRefObjectName(const Gmat::ObjectType type,
                                         const std::string &name);
   virtual const StringArray&
                        GetRefObjectNameArray(const Gmat::ObjectType type);
   
protected:
   
   /// A pointer to the referenced object (i.e. the leaf node or element).  
   /// This pointer is set when the MathTree is initialized in the Sandbox.
   Parameter* refObject;
   
   /// Holds the name of the GMAT object that is accessed by this node
   std::string refObjectName;    
   std::string refObjectType;
   
   /// The list of names of Wrapper objects
   StringArray wrapperObjectNames;
   /// Wrapper name and ElementWrapper pointer Map for RHS math element
   WrapperMap *theWrapperMap;
   
   void SetWrapperObjectNames(const std::string &name);
   void SetWrapperObject(GmatBase *obj, const std::string &name);
   ElementWrapper* FindWrapper(const std::string &name);
};

#endif //MathElement_hpp
