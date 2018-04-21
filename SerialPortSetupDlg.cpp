/***************************************************************
 * Name:      SerialPortSetupDlg.cpp
 * Purpose:   Code for Serial Port Setup Dialog
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
#include "SerialPortSetupDlg.h"

//(*InternalHeaders(SerialPortSetupDlg)
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(SerialPortSetupDlg)
const long SerialPortSetupDlg::ID_STATICTEXT1 = wxNewId();
const long SerialPortSetupDlg::ID_CHOICE1 = wxNewId();
const long SerialPortSetupDlg::ID_STATICTEXT2 = wxNewId();
const long SerialPortSetupDlg::ID_CHOICE2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SerialPortSetupDlg,wxDialog)
	//(*EventTable(SerialPortSetupDlg)
	//*)
END_EVENT_TABLE()

SerialPortSetupDlg::SerialPortSetupDlg(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SerialPortSetupDlg)
	wxGridBagSizer* GridBagSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	SetMinSize(wxSize(300,800));
	GridBagSizer1 = new wxGridBagSizer(0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Serial port: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	GridBagSizer1->Add(StaticText1, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ComPort = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	ComPort->Append(_T("COM1"));
	ComPort->Append(_T("COM2"));
	ComPort->Append(_T("COM3"));
	ComPort->Append(_T("COM4"));
	ComPort->Append(_T("COM5"));
	ComPort->Append(_T("COM6"));
	GridBagSizer1->Add(ComPort, wxGBPosition(0, 1), wxDefaultSpan, wxALL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Baud rate: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	GridBagSizer1->Add(StaticText2, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BaudRate = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	BaudRate->Append(_T("9600"));
	BaudRate->Append(_T("19200"));
	BaudRate->Append(_T("38400"));
	GridBagSizer1->Add(BaudRate, wxGBPosition(1, 1), wxDefaultSpan, wxALL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	GridBagSizer1->Add(StdDialogButtonSizer1, wxGBPosition(2, 0), wxGBSpan(1, 2), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(GridBagSizer1);
	GridBagSizer1->Fit(this);
	GridBagSizer1->SetSizeHints(this);
	//*)
}

SerialPortSetupDlg::~SerialPortSetupDlg()
{
	//(*Destroy(SerialPortSetupDlg)
	//*)
}

