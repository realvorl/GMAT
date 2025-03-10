//$Id: ManageObjectPanel.cpp 10048 2011-12-06 16:58:22Z lindajun $
//------------------------------------------------------------------------------
//                              ManageObjectPanel
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Author: Linda Jun
// Created: 2011/12/05
//
/**
 * This class sets up input for ManageObject command 
 */
//------------------------------------------------------------------------------

#include "ManageObjectPanel.hpp"
#include "MessageInterface.hpp"

//#define DEBUG_MANAGEOBJECT_CREATE
//#define DEBUG_MANAGEOBJECT_LOAD
//#define DEBUG_MANAGEOBJECT_SAVE

//------------------------------------------------------------------------------
// event tables and other macros for wxWindows
//------------------------------------------------------------------------------

BEGIN_EVENT_TABLE(ManageObjectPanel, GmatPanel)
   EVT_BUTTON(ID_BUTTON_OK, GmatPanel::OnOK)
   EVT_BUTTON(ID_BUTTON_APPLY, GmatPanel::OnApply)
   EVT_BUTTON(ID_BUTTON_CANCEL, GmatPanel::OnCancel)
   EVT_BUTTON(ID_BUTTON_SCRIPT, GmatPanel::OnScript)
   EVT_CHECKLISTBOX(ID_CHECKLISTBOX, ManageObjectPanel::OnCheckListBoxChange)
END_EVENT_TABLE()

//------------------------------
// public methods
//------------------------------

//------------------------------------------------------------------------------
// ManageObjectPanel(wxWindow *parent)
//------------------------------------------------------------------------------
/**
 * Constructs ManageObjectPanel object.
 *
 * @param <parent> input parent.
 */
//------------------------------------------------------------------------------
ManageObjectPanel::ManageObjectPanel(wxWindow *parent, GmatCommand *cmd)
   : GmatPanel(parent)

{
   theCommand = cmd;
   
   if (theCommand != NULL)
   {
      Create();
      Show();
   }
}


//------------------------------------------------------------------------------
// ~ManageObjectPanel()
//------------------------------------------------------------------------------
ManageObjectPanel::~ManageObjectPanel()
{
   theGuiManager->UnregisterCheckListBox("AllObject", mObjectCheckListBox);
}


//---------------------------------
// protected methods
//---------------------------------

//----------------------------------
// methods inherited from GmatPanel
//----------------------------------

//------------------------------------------------------------------------------
// void Create()
//------------------------------------------------------------------------------
/**
 * Creates the panel for the Maneuver Command
 */
//------------------------------------------------------------------------------
void ManageObjectPanel::Create()
{
   StringArray items;
   int bsize = 3;
   
   // create object label
   wxStaticText *objectLabel = NULL;
   wxStaticText *autoGlobalLabel = NULL;
	
	#ifdef DEBUG_MANAGEOBJECT_CREATE
	MessageInterface::ShowMessage
		("ManageObjectPanel::Create() The command is '%s'\n", theCommand->GetTypeName().c_str());
	#endif
	
	if (theCommand->IsOfType("Save"))
	{
      objectLabel = new wxStaticText(this, ID_TEXT, wxT("Please Select Objects To Save"),
												 wxDefaultPosition, wxDefaultSize, 0);
		// list of object, include automatic global objects
		mObjectCheckListBox = theGuiManager->
			GetAllObjectCheckListBox(this, ID_CHECKLISTBOX, wxSize(300,200), true);
   }
	else if (theCommand->IsOfType("Global"))
	{
      objectLabel = new wxStaticText(this, ID_TEXT, wxT("Please Select Objects To Make Global"),
												 wxDefaultPosition, wxDefaultSize, 0);
		// list of object, exclude automatic global objects
		mObjectCheckListBox = theGuiManager->
			GetAllObjectCheckListBox(this, ID_CHECKLISTBOX, wxSize(300,200), false);
		wxString autoGlobalText;
		wxArrayString autoGlobalList = theGuiManager->GetAutoGlobalObjectList();
		int numObjs = autoGlobalList.GetCount();
		for (int i = 0; i < numObjs - 1; i++)
			autoGlobalText += autoGlobalList[i] + ", ";
		autoGlobalText += autoGlobalList[numObjs - 1];
		autoGlobalText += " are Automatic Global Objects.";
		int height = (autoGlobalText.Len() / 30  + 1) * 10;
		autoGlobalLabel = new wxStaticText(this, ID_TEXT, autoGlobalText,
													  wxDefaultPosition, wxSize(300, height),
													  wxALIGN_LEFT|wxST_NO_AUTORESIZE);
		autoGlobalLabel->SetForegroundColour(*wxRED);
   }
	else
	{
      objectLabel = new wxStaticText(this, ID_TEXT, wxT("Please Select Objects"),
												 wxDefaultPosition, wxDefaultSize, 0);
		// list of object, include automatic global objects
		mObjectCheckListBox = theGuiManager->
			GetAllObjectCheckListBox(this, ID_CHECKLISTBOX, wxSize(300,200), true);
	}
	
   // create sizers
   wxBoxSizer *pageBoxSizer = new wxBoxSizer(wxVERTICAL);
   
   // add object label and combobox to sizer
   pageBoxSizer->Add(objectLabel, 0, wxALIGN_CENTER|wxALL, bsize);
   pageBoxSizer->Add(mObjectCheckListBox, 0, wxALIGN_CENTER|wxGROW|wxALL, bsize);
	if (autoGlobalLabel != NULL)
		pageBoxSizer->Add(autoGlobalLabel, 0, wxALIGN_CENTER|wxALL, bsize);
   
   // add to middle sizer
   theMiddleSizer->Add(pageBoxSizer, 0, wxALIGN_CENTRE|wxALL, bsize);     

}


//------------------------------------------------------------------------------
// virtual void LoadData()
//------------------------------------------------------------------------------
void ManageObjectPanel::LoadData()
{
   // Set the pointer for the "Show Script" button
   mObject = theCommand;
   
   // Set objects
   
   // Get object list from the command. It can be any object type.
   StringArray objNames = theCommand->GetStringArrayParameter("ObjectNames");

   for (UnsignedInt i=0; i<objNames.size(); i++)
   {
      #ifdef DEBUG_MANAGEOBJECT_LOAD
      MessageInterface::ShowMessage("   objName=<%s>\n", objNames[i].c_str());
      #endif
      
      wxString objName = objNames[i].c_str();
      for (UnsignedInt j=0; j<mObjectCheckListBox->GetCount(); j++)
      {
         wxString objStr = mObjectCheckListBox->GetString(j);
         if (objStr.BeforeFirst(' ') == objName)
            mObjectCheckListBox->Check(j);
      }
   }
}


//------------------------------------------------------------------------------
// virtual void SaveData()
//------------------------------------------------------------------------------
void ManageObjectPanel::SaveData()
{
   int count = mObjectCheckListBox->GetCount();
   
   int checkedCount = 0;
   canClose = true;
   
   //-----------------------------------------------------------------
   // check for number of objects checked
   //-----------------------------------------------------------------
   for (int i=0; i<count; i++)
      if (mObjectCheckListBox->IsChecked(i))
         checkedCount++;
   
   if (checkedCount == 0)
   {
      MessageInterface::PopupMessage
         (Gmat::ERROR_, "Please select one or more objects to save.");
      canClose = false;
      return;
   }
   
   #ifdef DEBUG_MANAGEOBJECT_SAVE
   MessageInterface::ShowMessage
      ("ManageObjectPanel::SaveData() number of checked object=%d\n", checkedCount);
   #endif
   
   //-----------------------------------------------------------------
   // save values to base, base code should do any range checking
   //-----------------------------------------------------------------
   
   wxString objStr, objName;
   
   theCommand->TakeAction("Clear");
   
   for (int i=0; i<count; i++)
   {
      if (mObjectCheckListBox->IsChecked(i))
      {
         objStr = mObjectCheckListBox->GetString(i);
         objName = objStr.BeforeFirst(' ');
         
         #ifdef DEBUG_MANAGEOBJECT_SAVE
         MessageInterface::ShowMessage
            ("objStr=<%s>, objName=<%s>\n", objStr.c_str(), objName.c_str());
         #endif
         
         theCommand->SetStringParameter("ObjectNames", objName.c_str());
      }
   }
}

//---------------------------------
// event handling
//---------------------------------

//------------------------------------------------------------------------------
// void OnCheckListBoxChange(wxCommandEvent& event)
//------------------------------------------------------------------------------
void ManageObjectPanel::OnCheckListBoxChange(wxCommandEvent& event)
{
   EnableUpdate(true);
}

