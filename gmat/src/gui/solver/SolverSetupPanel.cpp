//$Id: SolverSetupPanel.cpp 9457 2011-04-21 18:40:34Z lindajun $
//------------------------------------------------------------------------------
//                           SolverSetupPanel
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG06CA54C
//
// Author: Darrel Conway, Thinking Systems, Inc.
// Created: 2008/04/17
/**
 * This class is a generic setup panel used by plug-in solvers.
 */
//------------------------------------------------------------------------------

#include "SolverSetupPanel.hpp"
#include "MessageInterface.hpp"



//-----------------------------------------
// static members
//-----------------------------------------
const wxString SolverSetupPanel::TF_SCHEMES[2] = 
   {
      wxT("false"),
      wxT("true")
   };

/// wxWidget event mappings for the panel
BEGIN_EVENT_TABLE(SolverSetupPanel, GmatPanel)
   EVT_COMBOBOX(ID_COMBOBOX, SolverSetupPanel::OnComboBoxChange)
   EVT_TEXT(ID_TEXTCTRL, SolverSetupPanel::OnTextChange)
//   EVT_CHECKBOX(ID_CHECKBOX, SQPSetupPanel::OnCheckboxChange)
END_EVENT_TABLE()


//-----------------------------------------
// public methods
//-----------------------------------------

//------------------------------------------------------------------------------
// SolverSetupPanel(wxWindow *parent, const wxString &name)
//------------------------------------------------------------------------------
/**
 * Panel constructor
 * 
 * @param parent Owner for this panel
 * @param name Name of the solver that is to be configured 
 */
//------------------------------------------------------------------------------
SolverSetupPanel::SolverSetupPanel(wxWindow *parent, const wxString &name):
   GmatPanel        (parent)
{
   theSolver =
      (Solver*)theGuiInterpreter->GetConfiguredObject(name.c_str());

   isTextModified = true;
   
   if (theSolver != NULL)
   {
      Create();
      Show();
   }
   else
   {
      // show error message
   }
}

//------------------------------------------------------------------------------
// ~SolverSetupPanel()
//------------------------------------------------------------------------------
/**
 * Destructor.
 */
//------------------------------------------------------------------------------
SolverSetupPanel::~SolverSetupPanel()
{
}

//------------------------------------------------------------------------------
// void Create()
//------------------------------------------------------------------------------
/**
 * Inherited function that is called to create the panel.  This method calls the
 * Setup() method.
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::Create()
{
   Setup(this);
}

//------------------------------------------------------------------------------
// void LoadData()
//------------------------------------------------------------------------------
/**
 * Populates the panel with the configurable property data in the Solver 
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::LoadData()
{
   // load data from the core engine
   try
   {
      mObject = theSolver;
      
      std::string label;
      Integer propertyCount = theSolver->GetParameterCount();
      
      for (Integer i = 0; i < propertyCount; ++i)
      {
         if (mObject->IsParameterReadOnly(i) == false)
         {
            label = mObject->GetParameterText(i);
            LoadControl(label);
         }
      }
   }
   catch (BaseException &e)
   {
      MessageInterface::ShowMessage
         ("SolverSetupPanel:LoadData() error occurred!\n%s\n",
          e.GetFullMessage().c_str());
   }
   
   // explicitly disable apply button
   // it is turned on in each of the panels
   EnableUpdate(false);
}


//------------------------------------------------------------------------------
// void SaveData()
//------------------------------------------------------------------------------
/**
 * Passes configuration data from the panel to the Solver object
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::SaveData()
{
   canClose = true;
   
   try
   {
      for (std::map<std::string, Integer>::iterator i = controlMap.begin();
            i != controlMap.end(); ++i)
      {
         SaveControl(i->first);
         if (!canClose)
            return;
      }
   }
   catch (BaseException &e)
   {
      MessageInterface::PopupMessage(Gmat::ERROR_, e.GetFullMessage());
      canClose = false;
      return;
   }
}

//------------------------------------------------------------------------------
// void Setup(wxWindow *parent)
//------------------------------------------------------------------------------
/**
 * Uses the solver to build and populate the controls needed on the panel
 * 
 * This method walks through the properties of the Solver.  For each one that is
 * writable, it creates a descriptor and control, and saves these pieces in the
 * propertyDescriptors and propertyControls vectors.  The control indices in 
 * these vectors are stored by name in the controlMap.  The controls are then
 * places into a 2-column sizer for display on the panel.
 * 
 * @param parent The window that is getting setup (typically this window).
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::Setup(wxWindow *parent)
{
   if (theSolver == NULL)
      return;
   
   Integer propertyCount = theSolver->GetParameterCount();
   Integer j = 0;
   
   for (Integer i = 0; i < propertyCount; ++i)
   {
      if (theSolver->IsParameterReadOnly(i) == false)
      {
         propertyDescriptors.push_back(new wxStaticText(parent, ID_TEXT, 
               wxT(theSolver->GetParameterText(i).c_str())));
         
         controlMap[theSolver->GetParameterText(i)] = j++;

         wxControl* control = BuildControl(parent, i);
         propertyControls.push_back(control);
      }
   }

   wxFlexGridSizer *fGSMain = new wxFlexGridSizer(2);
   wxGridSizer *gSSpecs = new wxGridSizer(2);
   Integer border = 3;

   std::vector<wxStaticText*>::iterator item;

   for(item = propertyDescriptors.begin(), j = 0; 
       item != propertyDescriptors.end(); ++item, ++j) 
   {
      gSSpecs->Add(*item, 0, wxALL|wxALIGN_RIGHT, border);
      gSSpecs->Add(propertyControls[j], 0, wxALL|wxALIGN_LEFT, border);
   }

   fGSMain->Add(gSSpecs, 0, wxALL|wxALIGN_RIGHT, border*5);
   theMiddleSizer->Add(fGSMain, 0, wxALL|wxALIGN_CENTER, 5);
}

//------------------------------------------------------------------------------
// wxControl* BuildControl(wxWindow *parent, Integer index)
//------------------------------------------------------------------------------
/**
 * Builds a wxWidget control for an object property
 * 
 * @param parent The window that owns the control
 * @param index The index for the property that the constructed control 
 *              represents
 * 
 * @return The new control
 */
//------------------------------------------------------------------------------
wxControl *SolverSetupPanel::BuildControl(wxWindow *parent, Integer index)
{
   wxControl *control = NULL;
   
   Gmat::ParameterType type = theSolver->GetParameterType(index);
   
   switch (type)
   {
      case Gmat::BOOLEAN_TYPE:
         {
            wxComboBox *cbControl = new wxComboBox(parent, ID_COMBOBOX, "true", 
                  wxDefaultPosition, wxDefaultSize, 2, TF_SCHEMES, 
                  wxCB_READONLY);
                           
            control = cbControl;
         }
         break;
         
      case Gmat::STRING_TYPE:
      default:
         control = new wxTextCtrl(parent, ID_TEXTCTRL, 
                     wxT(""), wxDefaultPosition, wxSize(100,-1));
         break;
   }
   
   return control;
}

//------------------------------------------------------------------------------
// void LoadControl(const std::string &label)
//------------------------------------------------------------------------------
/**
 * Sets the data for a control
 * 
 * @param label The control's label
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::LoadControl(const std::string &label)
{
   Integer index = theSolver->GetParameterID(label);
   Gmat::ParameterType type = theSolver->GetParameterType(index);

   wxControl *theControl = propertyControls[controlMap[label]];
   wxString valueString;
   
   switch (type)
   {
      case Gmat::BOOLEAN_TYPE:
         if (theSolver->GetBooleanParameter(theSolver->GetParameterID(label)))
            ((wxComboBox*)(theControl))->SetValue(wxT("true"));
         else
            ((wxComboBox*)(theControl))->SetValue(wxT("false"));
         break;
      
      case Gmat::REAL_TYPE:
         {
            Real val = theSolver->GetRealParameter(
                  theSolver->GetParameterID(label));
            valueString << val;
            ((wxTextCtrl*)theControl)->ChangeValue(valueString);
         }
         break;
         
      case Gmat::INTEGER_TYPE:
         {
            Integer val = theSolver->GetIntegerParameter(
                  theSolver->GetParameterID(label));
            valueString << val;
            ((wxTextCtrl*)theControl)->ChangeValue(valueString);
         }
         break;
      
      case Gmat::STRING_TYPE:
         valueString = wxT(theSolver->GetStringParameter(label).c_str());
         ((wxTextCtrl*)theControl)->ChangeValue(valueString);
         break;

      default:
         break;
   }
}


//------------------------------------------------------------------------------
// void SaveControl(const std::string &label)
//------------------------------------------------------------------------------
/**
 * Passes a control's data to the Solver
 * 
 * @param label The string associated with the control.
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::SaveControl(const std::string &label)
{
   Integer index = theSolver->GetParameterID(label);
   Gmat::ParameterType type = theSolver->GetParameterType(index);

   wxControl *theControl = propertyControls[controlMap[label]];
   std::string valueString;
   
   switch (type)
   {
      case Gmat::BOOLEAN_TYPE:
         {
            bool val = true;
            if (((wxComboBox*)theControl)->GetValue() == "false")
               val = false;
            theSolver->SetBooleanParameter(index, val);
         }
         break;
      
      case Gmat::REAL_TYPE:
         {
            Real val; 
            valueString = ((wxTextCtrl*)theControl)->GetValue();
            CheckReal(val, valueString, label, "Real Number");
            if (!canClose)
               return;
            theSolver->SetRealParameter(index, val);
         }
         break;
         
      case Gmat::INTEGER_TYPE:
         {
            Integer val;
            valueString = ((wxTextCtrl*)theControl)->GetValue();
            CheckInteger(val, valueString, label, "Integer");
            if (!canClose)
               return;
            theSolver->SetIntegerParameter(index, val);
         }
         break;
      
      case Gmat::STRING_TYPE:
         valueString = ((wxTextCtrl*)theControl)->GetValue();
         theSolver->SetStringParameter(index, valueString.c_str());
         break;

      default:
         break;
   }
}

//------------------------------------------------------------------------------
// void OnComboBoxChange(wxCommandEvent& event)
//------------------------------------------------------------------------------
/**
 * Event handler for comboboxes
 * 
 * Activates the Apply button when selection is changed.
 * 
 * @param event The triggering event.
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::OnComboBoxChange(wxCommandEvent& event)
{
   if (theApplyButton != NULL)
      EnableUpdate(true);
}


//------------------------------------------------------------------------------
// void OnTextChange(wxCommandEvent& event)
//------------------------------------------------------------------------------
/**
 * Event handler for text boxes
 * 
 * Activates the Apply button when text is changed.
 * 
 * @param event The triggering event.
 */
//------------------------------------------------------------------------------
void SolverSetupPanel::OnTextChange(wxCommandEvent& event)
{
   isTextModified = true;
   EnableUpdate(true);
}
