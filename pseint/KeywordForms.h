#ifndef KEYWORDFORMS_H
#define KEYWORDFORMS_H

#include <string>
#include <vector>
#include "Keywords.hpp"

std::string GetKeywordRuntimeLexWords(KeywordType kw);
std::string GetKeywordRuntimeLexWords();
bool IsKeywordRuntimeWord(KeywordType kw, const std::string &candidate);
void GetKeywordVisibleForms(KeywordType kw, std::vector<std::string> &out);

#endif
