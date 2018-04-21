/***************************************************************
 * Name:      DisplayConfigurationDlg.cpp
 * Purpose:   Code for Display Configuration Dialog
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

#include "DisplayConfigurationDlg.h"

//(*InternalHeaders(DisplayConfigurationDlg)
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(DisplayConfigurationDlg)
const long DisplayConfigurationDlg::ID_STATICTEXT1 = wxNewId();
const long DisplayConfigurationDlg::ID_DISPLAYFIELDLIST = wxNewId();
//*)

BEGIN_EVENT_TABLE(DisplayConfigurationDlg,wxDialog)
	//(*EventTable(DisplayConfigurationDlg)
	//*)
END_EVENT_TABLE()

DisplayConfigurationDlg::DisplayConfigurationDlg(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DisplayConfigurationDlg)
	wxFlexGridSizer* FlexGridSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(3, 1, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Live measurement fields: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL, 5);
	DisplayFields = new wxCheckListBox(this, ID_DISPLAYFIELDLIST, wxDefaultPosition, wxSize(292,266), 0, 0, 0, wxDefaultValidator, _T("ID_DISPLAYFIELDLIST"));
	FlexGridSizer1->Add(DisplayFields, 1, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	FlexGridSizer1->Add(StdDialogButtonSizer1, 1, wxALL|wxALIGN_RIGHT|wxALIGN_BOTTOM, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)
}

DisplayConfigurationDlg::~DisplayConfigurationDlg()
{
	//(*Destroy(DisplayConfigurationDlg)
	//*)
}

