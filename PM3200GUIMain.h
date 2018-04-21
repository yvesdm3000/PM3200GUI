/***************************************************************
 * Name:      PM3200GUIMain.h
 * Purpose:   Defines Application Frame
 * Author:    Yves De Muyter (yves@alfavisio.be)
 * Created:   2018-04-02
 * Copyright: Yves De Muyter (yves@alfavisio.be)
 * License:   MIT
 **************************************************************/
/*
Copyright 2018 Yves De Muyter (yves@alfavisio.be)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef PM3200GUIMAIN_H
#define PM3200GUIMAIN_H

#include "PM3200Data.h"
#include "PM3200IO.h"
#include <chrono>
#include <wx/msw/registry.h>
#include <map>

//(*Headers(PM3200GUIFrame)
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
//*)

class PM3200GUIFrame: public wxFrame
{
    public:

        PM3200GUIFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PM3200GUIFrame();

        void DisconnectIO();
        void UpdateGridRows();
        void UpdateGridCells(PM3200Registers& registers, wxRegKey& key);

        void OnThread(wxCommandEvent& event);
        void AppendLog(wxRegKey& key);

    private:
        PM3200Data m_data;
        PM3200IO m_io;
        std::chrono::steady_clock::time_point m_nextLogTimePoint;
        std::vector<wxString> m_DisplayValues;
        wxThread* m_pThread;
        std::map<int,PM3200Registers> m_registers;
        bool m_loggingActivated;

        //(*Handlers(PM3200GUIFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnConfigureLogging(wxCommandEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnConfigureFieldsSelected(wxCommandEvent& event);
        void OnSerialPortSetupSelected(wxCommandEvent& event);
        void OnStartLoggingSelected(wxCommandEvent& event);
        void OnStopLoggingSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(PM3200GUIFrame)
        static const long ID_GRID1;
        static const long idMenuQuit;
        static const long ID_CONFIGUREFIELDS;
        static const long ID_MENUITEM4;
        static const long ID_STARTLOGGING;
        static const long ID_STOPLOGGING;
        static const long ID_CONFIGURELOGGING;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(PM3200GUIFrame)
        wxGrid* LiveValues;
        wxMenu* Menu3;
        wxMenu* Menu4;
        wxMenuBar* MenuBar1;
        wxMenuItem* ConfigureFields;
        wxMenuItem* MenuItem6;
        wxMenuItem* SerialPortSetup;
        wxMenuItem* StartLogging;
        wxMenuItem* StopLogging;
        wxStatusBar* StatusBar1;
        wxTimer Timer;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PM3200GUIMAIN_H
