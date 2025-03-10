//$Id: SolverVariablesPanel.hpp 9514 2011-04-30 21:44:00Z djcinsb $
//------------------------------------------------------------------------------
//                           SolverVariablesPanel
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Waka Waktola
// Created: 2004/21/01
// Modified: 2004/05/06 by Allison Greene to inherit from GmatPanel
/**
 * This class contains the Solver Variables window.
 */
//------------------------------------------------------------------------------

#ifndef SolverVariablesPanel_hpp
#define SolverVariablesPanel_hpp

#include "gmatwxdefs.hpp"

// base includes
#include "gmatdefs.hpp"
#include "GuiInterpreter.hpp"
#include "GmatPanel.hpp"

class SolverVariablesPanel : public GmatPanel
{
public:
    // constructors
    SolverVariablesPanel(wxWindow *parent);
    ~SolverVariablesPanel(); 
    
private:             
    wxGrid *varsGrid;
    
    wxStaticText *descStaticText;
    wxStaticText *solverStaticText;
    wxStaticText *varStaticText;
    wxStaticText *pertStaticText;
    wxStaticText *maxStaticText;
    wxStaticText *lowerStaticText;
    wxStaticText *upperStaticText;
    
    wxTextCtrl *descTextCtrl;
    wxTextCtrl *varTextCtrl;
    wxTextCtrl *pertTextCtrl;
    wxTextCtrl *maxTextCtrl;
    wxTextCtrl *lowerTextCtrl;
    wxTextCtrl *upperTextCtrl;
    
    wxButton *editButton;
    wxButton *updateButton;
    
    wxComboBox *solverComboBox;
    
    wxString solverString;
    wxString propertyString;
    wxString descriptionString;
    
    Integer numOfVars;
    Integer nextRow;
        
    // methods inherited from GmatPanel
    virtual void Create();
    virtual void LoadData();
    virtual void SaveData();
    //loj: 2/27/04 commented out
    //virtual void OnHelp();
    //virtual void OnScript();

    // Layout & data handling methods
    void Initialize();
    void Setup(wxWindow *parent);
    
    // Grid table event method
    void OnCellValueChanged(wxGridEvent &event);
    
    // Text control event method
    void OnTextUpdate(wxCommandEvent& event);    
    
    // Combobox event method
    void OnSolverSelection(wxCommandEvent &event);
    
    // Button event methods
    void OnButton(wxCommandEvent& event);

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE();
  
    // IDs for the controls and the menu commands
    enum
    {     
        ID_TEXT = 53000,
        ID_TEXTCTRL,
        ID_BUTTON,
        ID_COMBO,
        ID_GRID,
        SOL_COL,
        PRO_COL,
        DES_COL
    };
};

#endif // SolverGoalsPanel_hpp

