//------------------------------------------------------------------------------
//                              Divide
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number S-67573-G
//
// Author: LaMont Ruley
// Created: 2006/04/07
//
/**
 * Declares Divide class.
 */
//------------------------------------------------------------------------------
#ifndef Divide_hpp
#define Divide_hpp

#include "MathFunction.hpp"

class GMAT_API Divide : public MathFunction
{
public:
   Divide(const std::string &nomme);
   virtual ~Divide();
   Divide(const Divide &copy);
                 
   // inherited from GmatBase
   virtual GmatBase* Clone() const;

   virtual void GetOutputInfo(Integer &type, Integer &rowCount, Integer &colCount);
   virtual bool ValidateInputs(); 
   virtual Real Evaluate();
   virtual Rmatrix MatrixEvaluate();  
};

#endif // Divide_hpp
