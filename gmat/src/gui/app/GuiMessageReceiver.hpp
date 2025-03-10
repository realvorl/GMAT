//$Id$
//------------------------------------------------------------------------------
//                             GuiMessageReceiver
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
// Author: Darrel Conway, based on code by Linda Jun
// Created: 2008/04/28
//
/**
 * Declares operations on messages.
 */
//------------------------------------------------------------------------------
#ifndef GuiMessageReceiver_hpp
#define GuiMessageReceiver_hpp

#include "gmatdefs.hpp"
#include <queue>        // for queue
#include "MessageReceiver.hpp"


/**
 * GuiMessageReceiver implements the methods to present messages to the user
 * in GMAT's message window.
 * 
 * This class is implemented as a singleton.
 */
class GuiMessageReceiver : public MessageReceiver
{

public:
   static GuiMessageReceiver*  Instance();

   virtual void ClearMessage();
   virtual int  GetNumberOfMessageLines();
   virtual void ShowMessage(const std::string &msg);
   virtual void ShowMessage(const char *msg, ...);
   virtual void PopupMessage(Gmat::MessageType msgType, const std::string &msg);
   virtual void PopupMessage(Gmat::MessageType msgType, const char *msg, ...);
   virtual void PopupAbortContinue(const std::string &abortMsg,
                                  const std::string &continueMsg,
                                  const std::string &msg);
   
   virtual void LogMessage(const std::string &msg);
   virtual void LogMessage(const char *msg, ...);
   
   virtual std::string GetLogFileName();

   virtual void SetLogEnable(bool flag);
   virtual void SetLogPath(const std::string &pathname, bool append = false);
   virtual void SetLogFile(const std::string &filename);
   
   virtual void OpenLogFile(const std::string &filename, bool append = false);
   virtual void CloseLogFile();
   
   virtual std::string GetMessage();
   virtual void PutMessage(const std::string &msg);
   virtual void ClearMessageQueue();
   
private:
   static GuiMessageReceiver*  instance;
   const int MAX_MESSAGE_LENGTH;
   
   std::queue<std::string> messageQueue;
   std::string popupMessage;
   std::string abortMessage;
   std::string continueMessage;
   Gmat::MessageType messageType;
   int showIntervalInMilSec;
   short messageExist;
   std::string logFileName;
   FILE *logFile;
   bool logEnabled;
   bool logFileSet;
   std::ostream *msgStream;
   
   GuiMessageReceiver();
   ~GuiMessageReceiver();
};

#endif

