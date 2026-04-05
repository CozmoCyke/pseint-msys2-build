#ifndef KEYWORD_FORMS_H
#define KEYWORD_FORMS_H

#include <vector>
#include <string>
#include <wx/string.h>
#include "../pseint/Keywords.hpp"

wxString GetKeywordDisplayForm(KeywordType kw, const std::string &fallback);
wxString GetKeywordInsertionForm(KeywordType kw, const std::string &fallback);
wxString GetKeywordTooltipForm(KeywordType kw, const std::string &fallback);
wxString GetKeywordCanonicalForWord(const wxString &word);
wxString GetKeywordDisplayFormForWord(const wxString &word);
wxString GetKeywordTooltipFormForWord(const wxString &word);
wxString GetKeywordCategoryForWord(const wxString &word);
wxString GetKeywordHelpPageForWord(const wxString &word);
std::string GetKeywordRuntimeLexWords(KeywordType kw);
std::string GetKeywordRuntimeLexWords();
bool IsKeywordRuntimeWord(KeywordType kw, const std::string &candidate);
void GetKeywordVisibleForms(KeywordType kw, std::vector<std::string> &out);
void GetKeywordVisibleForms(KeywordType kw, std::vector<wxString> &out);

#endif
