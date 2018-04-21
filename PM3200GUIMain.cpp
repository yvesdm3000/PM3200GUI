/***************************************************************
 * Name:      PM3200GUIMain.cpp
 * Purpose:   Code for Application Frame
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
#include "PM3200GUIMain.h"
#include "LogConfigurationDlg.h"
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include "DisplayConfigurationDlg.h"
#include "SerialPortSetupDlg.h"
#include <wx/file.h>

//DECLARE_EVENT_TYPE(wxEVT_IOTHREAD, -1)
//DEFINE_EVENT_TYPE(wxEVT_IOTHREAD)

class IOThread : public wxThread
{
public:
    IOThread(wxFrame* parent, PM3200IO& io) : wxThread(wxTHREAD_DETACHED), m_pParent(parent), m_io(io)
    {

    }

    wxThread::ExitCode Entry()
    {
        while (m_io.IsConnected())
        {
            UpdateRegisters(2004, 32);
            UpdateRegisters(3000, 1212);
            UpdateRegisters(27214, 880);
            UpdateRegisters(45130, 76);
        }
        return 0;
    }
    void UpdateRegisters(int start, int count)
    {
        auto registers = new PM3200Registers(m_io.ReadRegisters(start,count));
        if (registers->GetRegisterCount() > 0)
        {
            wxCommandEvent evt(wxEVT_THREAD, start);
            evt.SetClientData(registers);
            m_pParent->AddPendingEvent(evt);
        }
        else
            delete registers;
    }
private:
    wxEvtHandler* m_pParent;
    PM3200IO& m_io;
};


//(*InternalHeaders(PM3200GUIFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(PM3200GUIFrame)
const long PM3200GUIFrame::ID_GRID1 = wxNewId();
const long PM3200GUIFrame::idMenuQuit = wxNewId();
const long PM3200GUIFrame::ID_CONFIGUREFIELDS = wxNewId();
const long PM3200GUIFrame::ID_MENUITEM4 = wxNewId();
const long PM3200GUIFrame::ID_STARTLOGGING = wxNewId();
const long PM3200GUIFrame::ID_STOPLOGGING = wxNewId();
const long PM3200GUIFrame::ID_CONFIGURELOGGING = wxNewId();
const long PM3200GUIFrame::idMenuAbout = wxNewId();
const long PM3200GUIFrame::ID_STATUSBAR1 = wxNewId();
const long PM3200GUIFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PM3200GUIFrame,wxFrame)
    //(*EventTable(PM3200GUIFrame)
    //*)
    EVT_COMMAND(wxID_ANY, wxEVT_THREAD, PM3200GUIFrame::OnThread)
END_EVENT_TABLE()

PM3200GUIFrame::PM3200GUIFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PM3200GUIFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, id, _("PM3200GUI"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    LiveValues = new wxGrid(this, ID_GRID1, wxDefaultPosition, wxSize(248,450), 0, _T("ID_GRID1"));
    LiveValues->CreateGrid(0,1);
    LiveValues->SetFocus();
    LiveValues->EnableEditing(false);
    LiveValues->EnableGridLines(true);
    LiveValues->SetRowLabelSize(200);
    LiveValues->SetColLabelValue(0, _("Value"));
    LiveValues->SetDefaultCellFont( LiveValues->GetFont() );
    LiveValues->SetDefaultCellTextColour( LiveValues->GetForegroundColour() );
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    ConfigureFields = new wxMenuItem(Menu3, ID_CONFIGUREFIELDS, _("Configure fields..."), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(ConfigureFields);
    SerialPortSetup = new wxMenuItem(Menu3, ID_MENUITEM4, _("Serial port setup..."), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(SerialPortSetup);
    MenuBar1->Append(Menu3, _("Edit"));
    Menu4 = new wxMenu();
    StartLogging = new wxMenuItem(Menu4, ID_STARTLOGGING, _("Start logging"), wxEmptyString, wxITEM_NORMAL);
    Menu4->Append(StartLogging);
    StopLogging = new wxMenuItem(Menu4, ID_STOPLOGGING, _("Stop logging"), wxEmptyString, wxITEM_NORMAL);
    Menu4->Append(StopLogging);
    StopLogging->Enable(false);
    Menu4->AppendSeparator();
    MenuItem6 = new wxMenuItem(Menu4, ID_CONFIGURELOGGING, _("Configure..."), wxEmptyString, wxITEM_NORMAL);
    Menu4->Append(MenuItem6);
    MenuBar1->Append(Menu4, _("Logging"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    Timer.SetOwner(this, ID_TIMER1);
    Timer.Start(1000, false);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnQuit);
    Connect(ID_CONFIGUREFIELDS,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnConfigureFieldsSelected);
    Connect(ID_MENUITEM4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnSerialPortSetupSelected);
    Connect(ID_STARTLOGGING,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnStartLoggingSelected);
    Connect(ID_STOPLOGGING,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnStopLoggingSelected);
    Connect(ID_CONFIGURELOGGING,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnConfigureLogging);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PM3200GUIFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&PM3200GUIFrame::OnTimer1Trigger);
    //*)

    UpdateGridRows();
    m_nextLogTimePoint = std::chrono::steady_clock::now();

    m_pThread = nullptr;
    m_loggingActivated = false;
}

PM3200GUIFrame::~PM3200GUIFrame()
{
    //(*Destroy(PM3200GUIFrame)
    //*)
}

void PM3200GUIFrame::OnQuit(wxCommandEvent& event)
{
    Timer.Stop();
    m_io.Disconnect();
    Close();
}

void PM3200GUIFrame::OnAbout(wxCommandEvent& event)
{
    //wxString msg = wxbuildinfo(long_f);
    wxString msg = "PM3200 GUI\nGUI to read and log values from a PM3250 and PM3255 Power Meter\n(C) by Alfavisio BVBA\n\nContact: Yves De Muyter (yves@alfavisio.be)";
    wxMessageBox(msg, _("Welcome to..."));
}

void PM3200GUIFrame::OnConfigureLogging(wxCommandEvent& event)
{
    LogConfigurationDlg dlg(this);
    auto list = m_data.GetRegisterNames();
    dlg.LogFields->InsertItems(list,0);
    wxRegKey key(wxRegKey::HKCU, "Software\\Alfavisio\\PM3200");
    key.Create();
    wxString str;
    wxString logdir;

    if (key.HasValue("LogValues"))
    {
        key.QueryValue("LogValues", str);
        wxStringTokenizer tok(str, "|");
        while (tok.HasMoreTokens())
        {
            wxString s = tok.GetNextToken();
            int idx = dlg.LogFields->FindString(s);
            if (idx != wxNOT_FOUND)
            {
                dlg.LogFields->Check(idx);
            }
        }
    }
    long freq = 10;
    if (key.HasValue("LogFrequency"))
    {
        key.QueryValue("LogFrequency", &freq);
    }
    dlg.LogFreq->SetValue(wxString::Format("%u", freq));

    if (key.HasValue("LogDir"))
    {
        key.QueryValue("LogDir", logdir);
    }
    dlg.LogDir->SetValue(logdir);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxArrayInt checkedItems;
        dlg.LogFields->GetCheckedItems(checkedItems);

        wxString str;
        for (auto& index : checkedItems)
        {
            if (!str.empty())
                str += "|";
            str += list[index];
        }
        key.SetValue("LogValues", str);

        if (dlg.LogFreq->GetValue().ToLong(&freq) )
            key.SetValue("LogFrequency", freq);

        key.SetValue("LogDir", dlg.LogDir->GetValue());
    }
}

void PM3200GUIFrame::DisconnectIO()
{
    m_io.Disconnect();
    if (m_pThread)
    {
        m_pThread = nullptr;
    }
}

void PM3200GUIFrame::OnTimer1Trigger(wxTimerEvent& event)
{
    wxRegKey key(wxRegKey::HKCU, "Software\\Alfavisio\\PM3200");
    key.Create();
    if (!m_io.IsConnected())
    {
        if (!key.HasValue("ComPort"))
        {
            GetStatusBar()->SetLabelText("COM port not configured");
            return;
        }
        wxString comport;
        key.QueryValue("ComPort", comport);
        long baudrate = 4800;
        if (key.HasValue("BaudRate"))
        {
            key.QueryValue("BaudRate", &baudrate);
        }
        long slaveAddress = 1;
        if (key.HasValue("SlaveAddress"))
        {
            key.QueryValue("SlaveAddress", &slaveAddress);
        }
        if (!m_io.Connect(comport, baudrate, slaveAddress))
        {
            GetStatusBar()->SetLabelText("Could not connect to PM3200");
            return;
        }
        std::string name = m_io.GetName();
        GetStatusBar()->SetLabelText("Connected to "+name);
        m_pThread = new IOThread(this, m_io);
        m_pThread->Create();
        m_pThread->Run();
    }

    auto t2 = std::chrono::steady_clock::now();

    if (!key.HasValue("LogFrequency"))
    {
        fprintf(stderr,"LogFrequency not set\n");
        fflush(stderr);
        return;
    }

    if (m_nextLogTimePoint <= t2)
    {
        fprintf(stderr, "Logging timepoint reached\n");
        fflush(stderr);
        if (m_loggingActivated)
        {
            fprintf(stderr,"Writing log entry\n");
            fflush(stderr);
            AppendLog(key);
        }
        else
        {
            fprintf(stderr,"Logging not activated\n");
            fflush(stderr);
        }

        long logfrequency = 1;
        key.QueryValue("LogFrequency", &logfrequency);

        while (m_nextLogTimePoint <= t2)
        {
            m_nextLogTimePoint += std::chrono::minutes(logfrequency);
        }
    }
    else
    {
        auto dur = m_nextLogTimePoint - t2;
        fprintf(stderr,"Remaining time for next log: %llu\n", (unsigned long long)std::chrono::duration_cast<std::chrono::seconds>(dur).count());
        fflush(stderr);
    }
}

void PM3200GUIFrame::AppendLog(wxRegKey& key)
{
    time_t now = time(nullptr);
    struct tm* d = gmtime(&now);
    if (!key.HasValue("LogDir"))
    {
        fprintf(stderr,"LogDir not set\n");
        fflush(stderr);
        return;
    }
    if (!key.HasValue("LogValues"))
    {
        fprintf(stderr,"LogValues not set\n");
        fflush(stderr);
        return;
    }
    wxString dir;
    key.QueryValue("LogDir", dir);
    wxString path = dir + "/" + wxString::Format("%04d%02d%02d.csv", 1900 + (int)d->tm_year, (int)d->tm_mon, (int)d->tm_mday);
    bool writeHeader = false;
    if (!wxFileExists(path))
    {
        writeHeader = true;
    }
    wxString logValues;
    key.QueryValue("LogValues", logValues);

    wxFile f;
    if (!f.Open(path, wxFile::OpenMode::write_append))
    {
        fprintf(stderr,"Could not open file for writing\n");
        return;
    }
    fprintf(stderr,"Opened file for writing: %s\n", path.c_str().AsChar());
    fflush(stderr);
    if (writeHeader)
    {
        f.Write("Timestamp,");
        // Write header
        wxStringTokenizer tok(logValues, "|");
        while (tok.HasMoreTokens())
        {
            wxString s = tok.GetNextToken();
            f.Write(s);
            if (tok.HasMoreTokens())
                f.Write(",");
        }
        f.Write("\r\n");
    }
    struct tm* tm = localtime(&now);
    f.Write(wxString::Format("%04d-%02d-%02d %02d:%02d:00,", 1900 + (int)tm->tm_year, (int)tm->tm_mon, (int)tm->tm_mday, (int)tm->tm_hour, (int)tm->tm_min));
    wxStringTokenizer tok(logValues, "|");
    while (tok.HasMoreTokens())
    {
        wxString s = tok.GetNextToken();
        int reg = m_data.LookupRegister(s.c_str());
        int count = m_data.LookupRegisterCount(reg);
        for (auto& p : m_registers)
        {
            if (p.second.HasRegisters(reg,count))
            {
                wxString str;
                switch(m_data.LookupType(reg))
                {
                case PM3200Data::RegisterType::UInt16:
                    str = wxString::Format("%u", (int)p.second.GetRegisterUInt16(reg));
                    break;
                case PM3200Data::RegisterType::UInt32:
                    str = wxString::Format("%u", (int)p.second.GetRegisterUInt32(reg));
                    break;
                case PM3200Data::RegisterType::Float32:
                    str = wxString::Format("%f", p.second.GetRegisterFloat32(reg));
                    break;
                case PM3200Data::RegisterType::Int64:
                    str = wxString::Format("%lld", (long long)p.second.GetRegisterInt64(reg));
                    break;
                case PM3200Data::RegisterType::DateTime:
                    {
                        auto t = p.second.GetRegisterDateTime(reg);
                        struct tm* d = localtime(&t);
                        str = wxString::Format("%4d-%2d-%2d %2d:%2d:%2d", 1900 + (int)d->tm_year, (int)d->tm_mon, (int)d->tm_mday, (int)d->tm_hour, (int)d->tm_min, (int)d->tm_sec);
                    }
                    break;
                }
                f.Write(str);
                break;
            }
        }
        if (tok.HasMoreTokens())
            f.Write(",");
    }
    f.Write("\r\n");
    f.Close();
}

void PM3200GUIFrame::OnThread(wxCommandEvent& event)
{
    wxRegKey key(wxRegKey::HKCU, "Software\\Alfavisio\\PM3200");
    PM3200Registers* registers = static_cast<PM3200Registers*>(event.GetClientData());
    UpdateGridCells(*registers, key);

    m_registers[registers->GetRegisterStart()].swap(*registers);

    delete(registers);
}

void PM3200GUIFrame::UpdateGridCells(PM3200Registers& registers, wxRegKey& key)
{
    if (key.HasValue("DisplayValues"))
    {
        wxGridUpdateLocker batch(LiveValues);
        if (m_DisplayValues.empty())
        {
            wxString str;
            key.QueryValue("DisplayValues", str);
            wxStringTokenizer tok(str, "|");
            while (tok.HasMoreTokens())
            {
                m_DisplayValues.push_back(tok.GetNextToken());
            }
        }
        int idx = 0;
        for (auto& label : m_DisplayValues)
        {
            wxString str;
            int reg = m_data.LookupRegister(label);
            switch (m_data.LookupType(reg))
            {
            case PM3200Data::RegisterType::UInt16:
                if (registers.HasRegisters(reg,1))
                    str = wxString::Format("%d", (int)registers.GetRegisterUInt16(reg));
                else
                {
                    ++idx;
                    continue;
                }
                break;
            case PM3200Data::RegisterType::UInt32:
                if (registers.HasRegisters(reg,2))
                    str = wxString::Format("%u", (unsigned int)registers.GetRegisterUInt32(reg));
                else
                {
                    ++idx;
                    continue;
                }
                break;
            case PM3200Data::RegisterType::Float32:
                if (registers.HasRegisters(reg,2))
                    str = wxString::Format("%f", registers.GetRegisterFloat32(reg));
                else
                {
                    ++idx;
                    continue;
                }
                break;
            case PM3200Data::RegisterType::Int64:
                if (registers.HasRegisters(reg,4))
                    str = wxString::Format("%lld", registers.GetRegisterInt64(reg));
                else
                {
                    ++idx;
                    continue;
                }
                break;
            case PM3200Data::RegisterType::DateTime:
                if (registers.HasRegisters(reg, 4))
                {
                    wxDateTime dt(registers.GetRegisterDateTime(reg));
                    str = dt.Format();
                }
                else
                {
                    ++idx;
                    continue;
                }
                break;

            }
            LiveValues->SetCellValue(idx, 0, str);
            ++idx;
        }
    }
}

void PM3200GUIFrame::OnConfigureFieldsSelected(wxCommandEvent& event)
{
    DisplayConfigurationDlg dlg(this);
    auto list = m_data.GetRegisterNames();
    dlg.DisplayFields->InsertItems(list,0);
    wxRegKey key(wxRegKey::HKCU, "Software\\Alfavisio\\PM3200");
    key.Create();

    if (key.HasValue("DisplayValues"))
    {
        wxString str;
        key.QueryValue("DisplayValues", str);
        wxStringTokenizer tok(str, "|");
        while (tok.HasMoreTokens())
        {
            wxString s = tok.GetNextToken();
            int idx = dlg.DisplayFields->FindString(s);
            if (idx != wxNOT_FOUND)
            {
                dlg.DisplayFields->Check(idx);
            }
        }
    }
    if (dlg.ShowModal() == wxID_OK)
    {
        wxArrayInt checkedItems;
        dlg.DisplayFields->GetCheckedItems(checkedItems);

        wxString str;
        for (auto& index : checkedItems)
        {
            if (!str.empty())
                str += "|";
            str += list[index];
        }
        key.SetValue("DisplayValues", str);
        UpdateGridRows();
    }
}

void PM3200GUIFrame::UpdateGridRows()
{
    m_DisplayValues.clear();
    wxGridUpdateLocker batch(LiveValues);
    if (LiveValues->GetNumberRows() > 0)
        LiveValues->DeleteRows(0, LiveValues->GetNumberRows());
    else
    {
        LiveValues->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
        LiveValues->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    }


    wxRegKey key(wxRegKey::HKCU, "Software\\Alfavisio\\PM3200");
    key.Create();

    wxString str;

    if (key.HasValue("DisplayValues"))
    {
        key.QueryValue("DisplayValues", str);
        wxStringTokenizer tok(str, "|");
        while (tok.HasMoreTokens())
        {
            wxString s = tok.GetNextToken();
            LiveValues->AppendRows(1);
            LiveValues->SetRowLabelValue(LiveValues->GetNumberRows()-1, s);
            m_DisplayValues.push_back(s);
        }
    }
}

void PM3200GUIFrame::OnSerialPortSetupSelected(wxCommandEvent& event)
{
    SerialPortSetupDlg dlg(this);
    wxRegKey key(wxRegKey::HKCU, "Software\\Alfavisio\\PM3200");
    key.Create();
    if (key.HasValue("ComPort"))
    {
        wxString comport;
        key.QueryValue("ComPort", comport);
        int sel = dlg.ComPort->FindString(comport);
        dlg.ComPort->SetSelection(sel);
    }
    if (key.HasValue("BaudRate"))
    {
        long baudrate = 19200;
        key.QueryValue("BaudRate", &baudrate);
        wxString str = wxString::Format("%d", baudrate);
        int sel = dlg.BaudRate->FindString(str);
        dlg.BaudRate->SetSelection(sel);
    }
    if (dlg.ShowModal() == wxID_OK)
    {
        int sel = dlg.ComPort->GetCurrentSelection();
        key.SetValue("ComPort", dlg.ComPort->GetString(sel));

        sel = dlg.BaudRate->GetCurrentSelection();
        long baudrate = 19200;
        wxString str = dlg.BaudRate->GetString(sel);
        if (str.ToLong(&baudrate))
            key.SetValue("BaudRate", baudrate);
        m_io.Disconnect();

    }
}

void PM3200GUIFrame::OnStartLoggingSelected(wxCommandEvent& event)
{
    m_loggingActivated = true;
    wxMenuBar* mb = GetMenuBar();
    if (!mb)
        return;
    wxMenuItem* item = mb->FindItem(ID_STOPLOGGING);
    if (!item)
        return;
    item->Enable();
    item = mb->FindItem(ID_STARTLOGGING);
    if (!item)
        return;
    item->Enable(false);
    m_nextLogTimePoint = std::chrono::steady_clock::now();
}

void PM3200GUIFrame::OnStopLoggingSelected(wxCommandEvent& event)
{
    m_loggingActivated = false;
    wxMenuBar* mb = GetMenuBar();
    if (!mb)
        return;
    wxMenuItem* item = mb->FindItem(ID_STOPLOGGING);
    if (!item)
        return;
    item->Enable(false);
    item = mb->FindItem(ID_STARTLOGGING);
    if (!item)
        return;
    item->Enable();
}
