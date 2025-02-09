//$Id: ArrayTemplate.hpp 9513 2011-04-30 21:23:06Z djcinsb $
//------------------------------------------------------------------------------
//                            ArrayTemplate
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
// Author: M. Weippert, T. McRoberts, L. Jun, E. Corderman
// Created: 1995/10/10 for GSS project
// Modified: 2003/09/15 Linda Jun - Replaced GSSString with std::string
//
/**
 * Ccontains the declarations for the ArrayTemplate array container class
 * (see Notes below for assumptions about parameter T)
 */
//
//------------------------------------------------------------------------------
#ifndef ArrayTemplate_hpp
#define ArrayTemplate_hpp

#include "gmatdefs.hpp"
#include "BaseException.hpp"

class GMAT_API ArrayTemplateExceptions
{
public:
    class OutOfBounds : public BaseException
          {public : OutOfBounds(const std::string& message =
          "ArrayTemplate error : out-of-bounds.")
          : BaseException(message) {};  };
    class DimensionError : public BaseException
          {public : DimensionError(const std::string& message =
          "ArrayTemplate error : dimension error.")
          : BaseException(message) {};  };
    class UnsizedArray : public BaseException
          {public : UnsizedArray(const std::string& message =
          "ArrayTemplate error : unsized array.")
          : BaseException(message) {};  };
    class ArrayAlreadySized : public BaseException
          {public : ArrayAlreadySized(const std::string& message =
          "ArrayTemplate error : array already sized.")
          : BaseException(message) {};  };
    class IllegalSize : public BaseException
          {public : IllegalSize(const std::string& message =
          "ArrayTemplate error : illegal size.")
          : BaseException(message) {};  };
};

template <class T>
class ArrayTemplate 
{

/**
 * @note
 *  Assumptions about template parameter types:
 *       Type has appropriate initializers and operators
 *       (constructors, "=", "==", "!="operators)
 *  The exceptions are declared in a separate class because the current HP
 *  compiler cannot properly handle exceptions declared a template class 
 *  and thrown in another template class.
 */

public:

    ArrayTemplate();
    ArrayTemplate(Integer sizeOfArray);
    // ArrayTemplate(Integer sizeOfArray, const T& a1, ...); // not currently
                                                         // allowed by compiler
    ArrayTemplate(Integer sizeOfArray, const T* array); // copy from c style array  
    ArrayTemplate(const ArrayTemplate<T> &array); 
    virtual ~ArrayTemplate();
   
    // operators
   
    const ArrayTemplate<T>& operator=(const ArrayTemplate<T> &array); 
    bool operator==(const ArrayTemplate<T> &array) const;
    bool operator!=(const ArrayTemplate<T> &array) const;
    virtual T&        operator()(Integer index);          
    virtual const T&  operator()(Integer index) const;
    virtual T&        operator[](Integer index);        
    virtual const T&  operator[](Integer index) const;

    virtual bool IsSized() const;
    virtual void SetSize(Integer size);
    virtual Integer  GetSize() const;
    virtual T    GetElement(Integer index) const;
    virtual void SetElement(Integer index, const T& value);
    
    const T* GetDataVector() const {return elementD;}
 
protected:
    void init(Integer s);      // used internally for initialization

    T    *elementD;
    Integer  sizeD;
    bool isSizedD;

private:
};

#ifndef EXTERNAL_TEMPLATE_DEFINITION
#include "ArrayTemplate.cpp"
#endif

#endif // h_ArrayTemplate_h
