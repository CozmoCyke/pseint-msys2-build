#ifndef HELPMANAGER_H
#define HELPMANAGER_H
#include <wx/string.h>
#include <wx/hashset.h>
#include "mxUtils.h"

#define MAX_ERRORS_TXT 512

class HelpManager {
private:
	wxString errors[MAX_ERRORS_TXT];
	mxHashStringString commands;
	mxHashStringString quickhelp;
public:
	HelpManager();
	void LoadErrors();
	void LoadCommands();
	void LoadQuickHelp();
	wxString GetErrorText(const wxString &text, int num);
	wxString GetCommandText(const wxString &command);
	wxString GetQuickHelp(wxString key, wxString def_val="nohelp.html");
};


extern HelpManager *help;

#endif

