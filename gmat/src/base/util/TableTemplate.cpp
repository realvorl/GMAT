//$Id: TableTemplate.cpp 10591 2012-06-20 19:04:41Z wendys-dev $
//------------------------------------------------------------------------------
//                              TableTemplate
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
// Author: M. Weippert, T. McRoberts, E. Corderman
// Created: 1995/10/10 for GSS project
// Modified: 2003/09/15 Linda Jun - See TableTemplate.hpp
//
/**
 * Provides declarations for the TableTemplate template class, representing
 * a 2-dimensional table of any type T (see assumptions about type T, below)
 */
//------------------------------------------------------------------------------
// #include <stdarg.h> will be used for variable argument list when implemented
#include "TableTemplate.hpp"

//------------------------------------------------------------------------------
//  public methods
//------------------------------------------------------------------------------
//  <default constructor>
//  TableTemplate()
//------------------------------------------------------------------------------
template <class T>
TableTemplate<T>::TableTemplate()
   :
   elementD((T*) 0), rowsD(0), colsD(0), isSizedD(false)
{
}

//------------------------------------------------------------------------------
//  <constructor>
//  TableTemplate(int r, int c)
//------------------------------------------------------------------------------
template <class T>
TableTemplate<T>::TableTemplate(int r, int c)
{
   if ((r < 0) || (c < 0))
   {
      throw TableTemplateExceptions::IllegalSize();
   }

   init(r, c);
}

//------------------------------------------------------------------------------
//  <constructor>
//  TableTemplate(int r, int c, const T &a11,...)
//
//  Notes: Current compiler will not accept this statement, although it is
//         proper C++. Hopefully, a future compiler will.
//------------------------------------------------------------------------------
/*
  template <class T>
  TableTemplate<T>::TableTemplate(int r, int c, const T &a11,...)
  {
  int i,j;
  va_list ap;        // argument list

  if ((r < 0) || (c < 0)
  {
  throw TableTemplateExceptions::IllegalSize();
  }
    
  init(r,c);        // allocate memory and set size

  va_start(ap,all);        // make ap point to first argument in list
  elementD[0] = all;         // first element is all

  // remaining elements are in arg list
  for( i = 0;i < r; i++)
  {
  for(j = 0; j < r; j++)
  {
  elementD[i,j} = va_arg(ap, const T*);
  }
  }
  }
*/

//------------------------------------------------------------------------------
//  <constructor>
//  TableTemplate(int r, int c, const T* array)
//------------------------------------------------------------------------------
template <class T>
TableTemplate<T>::TableTemplate(int r, int c, const T* array) 
{
   if ((r < 0) || (c < 0))
   {
      throw TableTemplateExceptions::IllegalSize();
   }

   init(r, c);

   int i;
   for(i = 0; i < rowsD*colsD; i++)
   {
      elementD[i] = array[i];
   }
}

//------------------------------------------------------------------------------
//  <copy constructor>
//  TableTemplate(const TableTemplate<T> &Table)
//------------------------------------------------------------------------------
template <class T>
TableTemplate<T>::TableTemplate(const TableTemplate<T> &Table) 
{
   if (Table.IsSized() == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   init(Table.rowsD, Table.colsD);
    
   for (int i = 0; i < rowsD*colsD; i++)
   {
      elementD[i] = Table.elementD[i];
   }
}

//------------------------------------------------------------------------------
//  <destructor>
//  ~TableTemplate()
//------------------------------------------------------------------------------
template <class T>
TableTemplate<T>::~TableTemplate() 
{
   delete[] elementD;
}

//------------------------------------------------------------------------------
//  bool operator==(const TableTemplate<T> &table) const
//------------------------------------------------------------------------------
template <class T>
bool
TableTemplate<T>::operator==(const TableTemplate<T> &table) const
{
   int i;

   if ((isSizedD == false) || (table.IsSized() == false))
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   if(this!=& table)
   {
      if((rowsD == table.rowsD)&&(colsD == table.colsD))
         for (i = 0; i < rowsD*colsD; i++)
         {
            if(elementD[i] != table.elementD[i])
            {
               return false;
            }
         }
      else
      {
         return false;
      }
   }
   
   return true;
}

//------------------------------------------------------------------------------
//  bool operator!=(const TableTemplate<T> &table) const
//------------------------------------------------------------------------------
template <class T>
bool
TableTemplate<T>::operator!=(const TableTemplate<T> &table) const
{
   return (bool)(!(*this == table));
}

//------------------------------------------------------------------------------
//  T& operator()(int r, int c)
//------------------------------------------------------------------------------
template <class T>
T&
TableTemplate<T>::operator()(int r, int c) {
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   if((r >= rowsD) || (r < 0) || (c >= colsD) || (c < 0))
      throw TableTemplateExceptions::OutOfBounds();
   
   return elementD[r*colsD + c];
}

//------------------------------------------------------------------------------
//  const T& operator()(int r, int c) const
//------------------------------------------------------------------------------
template <class T>
const T&
TableTemplate<T>::operator()(int r, int c) const 
{
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   if((r >= rowsD) || (r < 0) || (c >= colsD) || (c < 0))
      throw TableTemplateExceptions::OutOfBounds();
   return elementD[r*colsD + c];
}

//------------------------------------------------------------------------------
//  TableTemplate<T>& operator=(const TableTemplate<T> &table)
//
//  Notes: throws DimensionError() if (rowsD != table.rowsD) or 
//         (colsD != table.colsD)
//------------------------------------------------------------------------------
template <class T>
TableTemplate<T>&
TableTemplate<T>::operator=(const TableTemplate<T> &table) 
{
   if (table.IsSized() == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   if (isSizedD == false)
   {
      init(table.rowsD, table.colsD);
   }

   if (*this != table) {
      if((rowsD != table.rowsD) ||(colsD != table.colsD))
         throw TableTemplateExceptions::DimensionError();
      for (int i = 0; i < rowsD*colsD; i++)
      {
         elementD[i] = table.elementD[i];
      }
   }
   
   return *this;
}

//------------------------------------------------------------------------------
//  virtual T GetElement(int r, int c)
//
//  Notes: throws OutOfBounds error if (r >= rowsD) or (c >= colsD).
//------------------------------------------------------------------------------
template <class T>
T
TableTemplate<T>::GetElement(int r, int c) const 
{
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   if((r >= rowsD) || (r < 0) || (c >= colsD) || (c < 0))
      throw TableTemplateExceptions::OutOfBounds();
   
   return elementD[r*colsD + c];
}

//------------------------------------------------------------------------------
//  virtual void SetElement(int r, int c, const T &value)
//
//  Notes: throws OutOfBounds error if (r >= rowsD) or (c >= colsD).
//------------------------------------------------------------------------------
template <class T>
void
TableTemplate<T>::SetElement(int r, int c, const T &value) 
{
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   if((r >= rowsD) || (r < 0) || (c >= colsD) || (c < 0))
      throw TableTemplateExceptions::OutOfBounds();
   
   elementD[r*colsD + c] = value;
}

//------------------------------------------------------------------------------
//  virtual bool IsSized() const
//------------------------------------------------------------------------------
template <class T>
bool
TableTemplate<T>::IsSized() const
{
   return isSizedD;
}

//------------------------------------------------------------------------------
//  virtual void SetSize(int r, int c, bool zeroElements);
//------------------------------------------------------------------------------
template <class T>
void
TableTemplate<T>::SetSize(int r, int c, bool zeroElements)
{
   T        *saved          = NULL;
   Integer  oldRows         = rowsD;
   Integer  oldCols         = colsD;

   if (isSizedD == true)
   {
      //throw TableTemplateExceptions::TableAlreadySized();
      // wcs = 2008.05.07 - we need to be able to resize
      if (!zeroElements)
      {
         saved = new T[rowsD*colsD];
         for (int i=0; i<rowsD*colsD; i++)
            saved[i] = elementD[i];
      }
      delete [] elementD;
   }

   if ((r < 0) || (c < 0))
   {
      throw TableTemplateExceptions::IllegalSize();
   }

   init(r, c);
   if (!zeroElements)
   {
      if ((saved != NULL) && (oldRows != 0) && (oldCols != 0))
      {
         for (int ii = 0; ii < oldRows; ii++)
         {
            if (ii >= rowsD) break;
            for (int jj = 0; jj < oldCols; jj++)
            {
               if (jj >= colsD) break;
               // set to old values here
               elementD[(ii * colsD) + jj] = saved[(ii * oldCols) + jj];
            }
         }
         delete saved;
      }
   }
}

//------------------------------------------------------------------------------
//  virtual void GetSize(int &r, int &c) const
//------------------------------------------------------------------------------
template <class T>
void
TableTemplate<T>::GetSize(int &r, int &c) const 
{
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   r = rowsD;
   c = colsD;
}

//------------------------------------------------------------------------------
//  virtual int GetNumColumns() const
//------------------------------------------------------------------------------
template <class T>
int
TableTemplate<T>::GetNumColumns() const 
{
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   return colsD;
}

//------------------------------------------------------------------------------
//  virtual int GetNumRows() const
//------------------------------------------------------------------------------
template <class T>
int
TableTemplate<T>::GetNumRows() const 
{
   if (isSizedD == false)
   {
      throw TableTemplateExceptions::UnsizedTable();
   }

   return rowsD;
}

//------------------------------------------------------------------------------
//  protected methods
//------------------------------------------------------------------------------
//  void init(int r, int c)
//------------------------------------------------------------------------------
template <class T>
void
TableTemplate<T>::init(int r, int c)
{
   rowsD = r;
   colsD = c;

   if ((rowsD == 0) || (colsD == 0))
   {
      rowsD = colsD = 0;
      elementD = (T *) 0;
   }
   else
   {
      elementD = new T[rowsD*colsD];

      //loj: 9/20/04 added to initialize to 0.0
      for (int i=0; i<rowsD*colsD; i++)
         elementD[i] = 0.0;
   }
   isSizedD = true;
}

