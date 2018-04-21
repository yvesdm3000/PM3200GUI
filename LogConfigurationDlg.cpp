/***************************************************************
 * Name:      LogConfigurationDlg.cpp
 * Purpose:   Code for Log Configuration Dialog
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
#include "LogConfigurationDlg.h"
#include <wx/valnum.h>
#include <wx/dirdlg.h>

//(*InternalHeaders(LogConfigurationDlg)
#include <wx/string.h>
//*)

//(*IdInit(LogConfigurationDlg)
const long LogConfigurationDlg::ID_LOGDIR = wxNewId();
const long LogConfigurationDlg::ID_SELLOGDIR = wxNewId();
const long LogConfigurationDlg::ID_STATICTEXT1 = wxNewId();
const long LogConfigurationDlg::ID_LOGFIELDS = wxNewId();
const long LogConfigurationDlg::ID_STATICTEXT2 = wxNewId();
const long LogConfigurationDlg::ID_STATICTEXT3 = wxNewId();
const long LogConfigurationDlg::ID_LOGFREQ = wxNewId();
const long LogConfigurationDlg::ID_STATICTEXT4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(LogConfigurationDlg,wxDialog)
	//(*EventTable(LogConfigurationDlg)
	//*)
END_EVENT_TABLE()

LogConfigurationDlg::LogConfigurationDlg(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(LogConfigurationDlg)
	wxGridBagSizer* GridBagSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _T("Configure logging"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	GridBagSizer1 = new wxGridBagSizer(0, 0);
	LogDir = new wxTextCtrl(this, ID_LOGDIR, _T("Text"), wxDefaultPosition, wxSize(149,21), 0, wxDefaultValidator, _T("ID_LOGDIR"));
	GridBagSizer1->Add(LogDir, wxGBPosition(3, 0), wxGBSpan(1, 2), wxALL|wxEXPAND, 5);
	SelLogDir = new wxButton(this, ID_SELLOGDIR, _T("..."), wxDefaultPosition, wxSize(31,23), 0, wxDefaultValidator, _T("ID_SELLOGDIR"));
	GridBagSizer1->Add(SelLogDir, wxGBPosition(3, 2), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	LogfieldsLabel = new wxStaticText(this, ID_STATICTEXT1, _T("Log fields:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	GridBagSizer1->Add(LogfieldsLabel, wxGBPosition(0, 0), wxDefaultSpan, wxALL, 5);
	LogFields = new wxCheckListBox(this, ID_LOGFIELDS, wxDefaultPosition, wxSize(203,267), 0, 0, 0, wxDefaultValidator, _T("ID_LOGFIELDS"));
	LogFields->SetFocus();
	GridBagSizer1->Add(LogFields, wxGBPosition(1, 0), wxGBSpan(1, 3), wxALL|wxEXPAND, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT2, _T("Log directory:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	GridBagSizer1->Add(StaticText1, wxGBPosition(2, 0), wxDefaultSpan, wxALL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT3, _T("Log frequency:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	GridBagSizer1->Add(StaticText2, wxGBPosition(4, 0), wxDefaultSpan, wxALL, 5);
	LogFreq = new wxTextCtrl(this, ID_LOGFREQ, wxEmptyString, wxDefaultPosition, wxSize(46,21), 0, wxIntegerValidator<unsigned long>(), _T("ID_LOGFREQ"));
	GridBagSizer1->Add(LogFreq, wxGBPosition(5, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT, 3);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT4, _T("(Minutes)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	GridBagSizer1->Add(StaticText3, wxGBPosition(5, 1), wxDefaultSpan, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	GridBagSizer1->Add(StdDialogButtonSizer1, wxGBPosition(6, 0), wxGBSpan(4, 2), wxALL|wxALIGN_RIGHT|wxALIGN_BOTTOM, 5);
	SetSizer(GridBagSizer1);
	GridBagSizer1->Fit(this);
	GridBagSizer1->SetSizeHints(this);

	Connect(ID_SELLOGDIR,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LogConfigurationDlg::OnSelLogDirClick);
	//*)
}

LogConfigurationDlg::~LogConfigurationDlg()
{
	//(*Destroy(LogConfigurationDlg)
	//*)
}


void LogConfigurationDlg::OnSelLogDirClick(wxCommandEvent& event)
{
    wxString path = LogDir->GetValue();
    wxDirDialog d(this, "Choose log directory", "",
                wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    d.SetPath(path);
    if (d.ShowModal() == wxID_OK)
    {
        LogDir->SetValue(d.GetPath());
    }
}
