//$Id: OptimizePanel.hpp 10380 2012-04-16 22:29:15Z lindajun $
//------------------------------------------------------------------------------
//                           OptimizePanel
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc.
//
// Author: Linda Jun (NASA/GSFC)
// Created: 2007/01/26
//
/**
 * This class contains the Optimize setup window.
 */
//------------------------------------------------------------------------------
#ifndef OptimizePanel_hpp
#define OptimizePanel_hpp

#include "gmatwxdefs.hpp"
#include "GmatCommand.hpp"
#include "GmatPanel.hpp"

class OptimizePanel : public GmatPanel
{
public:
   // constructor
   OptimizePanel(wxWindow *parent, GmatCommand *cmd);
   ~OptimizePanel();   
   
private:
   
   GmatCommand *theCommand;
   
   wxComboBox *mSolverComboBox;
   wxComboBox *mSolverModeComboBox;
   wxComboBox *mExitModeComboBox;
   wxButton   *mApplyCorrectionsButton;
   
   // methods inherited from GmatPanel
   virtual void Create();
   virtual void LoadData();
   virtual void SaveData();
   
   // Combobox event method
   void OnComboBoxChange(wxCommandEvent& event);
   
   // Button press event method
   void OnApplyButtonPress(wxCommandEvent& event);

   // any class wishing to process wxWindows events must use this macro
   DECLARE_EVENT_TABLE();
   
   // IDs for the controls and the menu commands
   enum
   {     
      ID_TEXT = 51000,
      ID_COMBO,
      ID_APPLYBUTTON
   };
};

#endif // OptimizePanel_hpp

