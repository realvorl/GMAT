//$Id: ScriptEventPanel.hpp 10964 2012-10-10 21:06:25Z lindajun $
//------------------------------------------------------------------------------
//                              ScriptEventPanel
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
// Author: Allison Greene
// Created: 2005/1/12
// Modified:
//    2006.12.04 Linda Jun
//       - Implemented save changes by replacing old command sequence with new
//
/**
 * Declares ScriptEventPanel class.
 */
//------------------------------------------------------------------------------

#ifndef ScriptEventPanel_hpp
#define ScriptEventPanel_hpp

#include "GmatPanel.hpp"
#include "Parameter.hpp"
#include "MissionTreeItemData.hpp"
#include <wx/laywin.h>

#ifdef __USE_STC_EDITOR__
#include "Editor.hpp"
#endif

class ScriptEventPanel: public GmatPanel
{
public:
   // constructors
   ScriptEventPanel(wxWindow *parent, MissionTreeItemData *item);
   ~ScriptEventPanel();
   
   virtual void SetEditorModified(bool flag);
   
   wxTextCtrl *mFileContentsTextCtrl;
   
#ifdef __USE_STC_EDITOR__
   Editor* GetEditor() { return mEditor; };
#endif
   
private:
   // member data
#ifdef __USE_STC_EDITOR__
   Editor *mEditor;
#endif
   
   MissionTreeItemData *theItem;
   GmatCommand *theCommand;
   GmatCommand *mPrevCommand;
   GmatCommand *mNextCommand;
   GmatCommand *mNewCommand;
   
   wxSashLayoutWindow* theCommentsWin;
   wxSashLayoutWindow* theScriptsWin;
   
   wxTextCtrl  *mCommentTextCtrl;
   wxGridSizer *mBottomSizer;
   wxBoxSizer  *mPageSizer;
   
   void SaveComments();
   void ReplaceScriptEvent();
   
   // for Debug
   void ShowCommand(const std::string &title1, GmatCommand *cmd1,
                    const std::string &title2 = "", GmatCommand *cmd2 = NULL);
   
   // methods inherited from GmatPanel
   virtual void Create();
   virtual void LoadData();
   virtual void SaveData();
   
   // event handling
   void OnCommentChange(wxCommandEvent& event);
   void OnScriptChange(wxCommandEvent& event);
   void OnSashDrag(wxSashEvent &event);
   void OnSize(wxSizeEvent &event);
   
   DECLARE_EVENT_TABLE();
   
   // IDs for the controls and the menu commands
   enum
   {
      ID_SASH_WINDOW = 9000,
      ID_TEXT,
      ID_COMMENT_CTRL,
      ID_SCRIPT_CTRL,
      ID_STC,
   };
};

#endif
