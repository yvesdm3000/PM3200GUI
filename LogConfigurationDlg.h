/***************************************************************
 * Name:      PM3200GUIMain.cpp
 * Purpose:   Defines Log Configuration Dialog
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
#ifndef LOGCONFIGURATIONDLG_H
#define LOGCONFIGURATIONDLG_H

//(*Headers(LogConfigurationDlg)
#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/dialog.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class LogConfigurationDlg: public wxDialog
{
	public:

		LogConfigurationDlg(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~LogConfigurationDlg();

		//(*Declarations(LogConfigurationDlg)
		wxButton* SelLogDir;
		wxCheckListBox* LogFields;
		wxStaticText* LogfieldsLabel;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxTextCtrl* LogDir;
		wxTextCtrl* LogFreq;
		//*)

	protected:

		//(*Identifiers(LogConfigurationDlg)
		static const long ID_LOGDIR;
		static const long ID_SELLOGDIR;
		static const long ID_STATICTEXT1;
		static const long ID_LOGFIELDS;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_LOGFREQ;
		static const long ID_STATICTEXT4;
		//*)

	private:

		//(*Handlers(LogConfigurationDlg)
		void OnSelLogDirClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
