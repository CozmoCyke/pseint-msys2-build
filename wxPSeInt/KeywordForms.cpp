#include "KeywordForms.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <wx/filename.h>

#include "ConfigManager.h"
#include "LocalizationManager.h"
#include "string_conversions.h"

namespace {

struct KeywordRuntimeSpec {
	wxString display;
	wxString insert;
	wxString aliases;
	wxString help;
	wxString category;
	wxString tooltip;
};

static std::map<std::string, KeywordRuntimeSpec> g_specs;
static std::map<std::string, std::string> g_meta_aliases;
static std::string g_loaded_language;
static bool g_loaded = false;

static wxString DecodeBytes(const std::string &s) {
	if (s.empty()) return wxString();
	wxString ws = wxString::FromUTF8(s.c_str());
	if (ws.empty() && !s.empty()) ws = wxString::From8BitData(s.c_str());
	return ws;
}

static std::string ToUtf8(const wxString &s) {
	wxScopedCharBuffer buf = s.ToUTF8();
	if (!buf) return std::string();
	return std::string(buf.data());
}

static wxString TrimCopy(wxString s) {
	s.Trim(true);
	s.Trim(false);
	return s;
}

static wxString UpperCopy(wxString s) {
	s.MakeUpper();
	return s;
}

static wxString LowerCopy(wxString s) {
	s.MakeLower();
	return s;
}

	static std::string CanonicalIdForKeyword(KeywordType kw) {
	switch (kw) {
		case KW_LEER: return "LEER";
		case KW_ESCRIBIR: return "ESCRIBIR";
	case KW_ES: return "ES";
	case KW_PARACADA: return "PARACADA";
	case KW_DE: return "DE";
	case KW_DESDE: return "DESDE";
	case KW_DIMENSIONAR: return "DIMENSIONAR";
	case KW_REDIMENSIONAR: return "REDIMENSIONAR";
	case KW_MIENTRASQUE: return "MIENTRASQUE";
	case KW_DEFINIR: return "DEFINIR";
		case KW_COMO: return "COMO";
		case KW_SIN_SALTAR: return "SIN_SALTAR";
		case KW_LIMPIARPANTALLA: return "LIMPIARPANTALLA";
		case KW_ESPERARTECLA: return "ESPERARTECLA";
		case KW_ESPERARTIEMPO: return "ESPERARTIEMPO";
		case KW_SEGUNDOS: return "SEGUNDOS";
		case KW_MILISEGUNDOS: return "MILISEGUNDOS";
		default: return std::string();
	}
}

static KeywordRuntimeSpec *FindSpec(KeywordType kw) {
	const std::string id = CanonicalIdForKeyword(kw);
	if (id.empty()) return nullptr;
	auto it = g_specs.find(id);
	if (it == g_specs.end()) return nullptr;
	return &it->second;
}

static const KeywordRuntimeSpec *FindSpec(const wxString &word, std::string *canonical_id);
static void BuildUiMetaAliases();
static const KeywordRuntimeSpec *ResolveUiMetaSpecForWord(const wxString &word, std::string *canonical_id, bool *resolved_alias = nullptr);

static const KeywordRuntimeSpec *FindSpec(const wxString &word) {
	std::string unused;
	return FindSpec(word, &unused);
}

static const KeywordRuntimeSpec *FindSpec(const wxString &word, std::string *canonical_id) {
	wxString normalized = TrimCopy(word);
	if (normalized.IsEmpty()) return nullptr;
	normalized.MakeUpper();

	for (auto &entry : g_specs) {
		const wxString id = wxString::FromUTF8(entry.first.c_str());
		if (UpperCopy(id) == normalized) {
			if (canonical_id) *canonical_id = entry.first;
			return &entry.second;
		}

		wxString display = TrimCopy(entry.second.display);
		if (!display.IsEmpty()) {
			wxString tmp = display;
			tmp.MakeUpper();
			if (tmp == normalized) {
				if (canonical_id) *canonical_id = entry.first;
				return &entry.second;
			}
		}

		wxString insert = TrimCopy(entry.second.insert);
		if (!insert.IsEmpty()) {
			wxString tmp = insert;
			tmp.MakeUpper();
			if (tmp == normalized) {
				if (canonical_id) *canonical_id = entry.first;
				return &entry.second;
			}
		}

		const std::string aliases_utf8 = ToUtf8(entry.second.aliases);
		size_t start = 0;
		while (start <= aliases_utf8.length()) {
			size_t end = aliases_utf8.find(',', start);
			std::string alias_raw = aliases_utf8.substr(start, end == std::string::npos ? std::string::npos : end - start);
			wxString alias = TrimCopy(DecodeBytes(alias_raw));
			if (!alias.IsEmpty()) {
				wxString tmp = alias;
				tmp.MakeUpper();
				if (tmp == normalized) {
					if (canonical_id) *canonical_id = entry.first;
					return &entry.second;
				}
			}
			if (end == std::string::npos) break;
			start = end + 1;
		}
	}

	return nullptr;
}

static std::vector<wxString> SplitAliases(const wxString &aliases) {
	std::vector<wxString> out;
	const std::string aliases_utf8 = ToUtf8(aliases);
	size_t start = 0;
	while (start <= aliases_utf8.length()) {
		size_t end = aliases_utf8.find(',', start);
		std::string alias_raw = aliases_utf8.substr(start, end == std::string::npos ? std::string::npos : end - start);
		wxString alias = TrimCopy(DecodeBytes(alias_raw));
		if (!alias.IsEmpty()) out.push_back(alias);
		if (end == std::string::npos) break;
		start = end + 1;
	}
	return out;
}

static wxString ResolveKeywordsFile(const std::string &lang) {
	wxString langWx = wxString::FromUTF8(lang.c_str());
	wxFileName primary(wxString("keywords"), langWx + ".ini");
	primary.Normalize();
	if (wxFileName::FileExists(primary.GetFullPath()))
		return primary.GetFullPath();

	wxFileName fallback(wxString("..") + wxFILE_SEP_PATH + wxString("keywords"), langWx + ".ini");
	fallback.Normalize();
	if (wxFileName::FileExists(fallback.GetFullPath()))
		return fallback.GetFullPath();

	return primary.GetFullPath();
}

static void StoreSpec(const wxString &section, const KeywordRuntimeSpec &spec) {
	wxString key = TrimCopy(section);
	key.MakeUpper();
	if (!key.IsEmpty())
		g_specs[ToUtf8(key)] = spec;
}

static void ApplySpecField(const wxString &section, const wxString &field_raw, const wxString &value) {
	wxString key = TrimCopy(section);
	key.MakeUpper();
	if (key.IsEmpty())
		return;

	KeywordRuntimeSpec &spec = g_specs[ToUtf8(key)];
	wxString field = TrimCopy(field_raw);
	field.MakeLower();

	if (field == "display") spec.display = value;
	else if (field == "insert") spec.insert = value;
	else if (field == "aliases") spec.aliases = value;
	else if (field == "help") spec.help = value;
	else if (field == "category") spec.category = value;
	else if (field == "tooltip") spec.tooltip = value;
}

static bool LoadKeywordsFile(const wxString &path) {
	std::ifstream fil(path.fn_str(), std::ios::binary);
	if (!fil.is_open())
		return false;

	g_specs.clear();
	KeywordRuntimeSpec current;
	wxString current_section;
	bool first_line = true;

	std::string raw;
	while (std::getline(fil, raw)) {
		if (!raw.empty() && raw.back() == '\r')
			raw.pop_back();

		wxString line = DecodeBytes(raw);
		if (first_line && !line.empty() && line[0] == 0xFEFF)
			line.Remove(0, 1);
		first_line = false;

		line = TrimCopy(line);
		if (line.IsEmpty() || line[0] == '#' || line[0] == ';')
			continue;

		if (line[0] == '[' && line[line.length() - 1] == ']') {
			if (!current_section.IsEmpty())
				StoreSpec(current_section, current);
			current = KeywordRuntimeSpec();
			current_section = line.Mid(1, line.length() - 2);
			continue;
		}

		int pos = line.Find('=');
		if (pos == wxNOT_FOUND)
			continue;

		wxString key = TrimCopy(line.Left(pos));
		wxString value = TrimCopy(line.Mid(pos + 1));
		if (!current_section.IsEmpty()) {
			key.MakeLower();
			if (key == "display") current.display = value;
			else if (key == "insert") current.insert = value;
			else if (key == "aliases") current.aliases = value;
			else if (key == "help") current.help = value;
			else if (key == "category") current.category = value;
			else if (key == "tooltip") current.tooltip = value;
			continue;
		}

		const wxString prefix = "keyword.";
		wxString key_lower = LowerCopy(key);
		if (!key_lower.StartsWith(prefix))
			continue;

		wxString suffix = key.Mid(prefix.length());
		int dot = suffix.Find('.', true);
		if (dot == wxNOT_FOUND)
			continue;

		wxString section = suffix.Left(dot);
		wxString field = suffix.Mid(dot + 1);
		if (section.IsEmpty() || field.IsEmpty())
			continue;

		ApplySpecField(section, field, value);
	}

	if (!current_section.IsEmpty())
		StoreSpec(current_section, current);

	return !g_specs.empty();
}

static void EnsureLoaded() {
	const std::string lang = LocalizationManager::Instance().GetCurrentLanguage();
	if (g_loaded && g_loaded_language == lang)
		return;

	g_loaded = false;
	g_loaded_language = lang;
	g_specs.clear();
	g_meta_aliases.clear();

	wxString path = ResolveKeywordsFile(lang);
	LoadKeywordsFile(path);
	BuildUiMetaAliases();

	g_loaded = true;
}

static wxString GetFieldOrFallback(const wxString &value, const std::string &fallback) {
	if (!value.IsEmpty())
		return value;
	return _Z(fallback.c_str());
}

static bool IsUiMetaCategory(const wxString &category) {
	wxString normalized = LowerCopy(TrimCopy(category));
	return normalized == "predef" || normalized == "constant";
}

static wxString MetaCategoryToken(const wxString &category) {
	wxString normalized = LowerCopy(TrimCopy(category));
	if (normalized == "constant")
		return "CONSTANT";
	return "PREDEF";
}

static void LogKeywordMetaOnce(const std::string &canonical, const wxString &category, const char *field, const char *source) {
	if (!IsUiMetaCategory(category))
		return;

	static std::set<std::string> seen;
	const std::string lang = LocalizationManager::Instance().GetCurrentLanguage().empty() ? "es" : LocalizationManager::Instance().GetCurrentLanguage();
	const std::string token = lang + "|" + canonical + "|" + field + "|" + source;
	if (!seen.insert(token).second)
		return;

	std::cerr
		<< "[KW][META][" << _W2S(MetaCategoryToken(category)) << "] "
		<< "canonical=" << canonical
		<< " field=" << field
		<< " source=" << source
		<< " lang=" << lang
		<< std::endl;
}

static void LogMetaAliasLoadOnce(const std::string &alias, const std::string &canonical) {
	static std::set<std::string> seen;
	const std::string lang = LocalizationManager::Instance().GetCurrentLanguage().empty() ? "es" : LocalizationManager::Instance().GetCurrentLanguage();
	const std::string token = lang + "|" + alias + "|" + canonical;
	if (!seen.insert(token).second)
		return;

	std::cerr
		<< "[KW][META][ALIAS] "
		<< "alias=" << ToUtf8(LowerCopy(DecodeBytes(alias)))
		<< " canonical=" << canonical
		<< " lang=" << lang
		<< std::endl;
}

static void LogMetaAliasConflict(const std::string &alias, const std::string &canonical1, const std::string &canonical2) {
	const std::string lang = LocalizationManager::Instance().GetCurrentLanguage().empty() ? "es" : LocalizationManager::Instance().GetCurrentLanguage();
	std::cerr
		<< "[KW][META][ALIAS][CONFLICT] "
		<< "alias=" << ToUtf8(LowerCopy(DecodeBytes(alias)))
		<< " canonical1=" << canonical1
		<< " canonical2=" << canonical2
		<< " lang=" << lang
		<< std::endl;
}

static void LogMetaResolveOnce(const wxString &input, const std::string &canonical, const char *field, const char *source) {
	static std::set<std::string> seen;
	const std::string lang = LocalizationManager::Instance().GetCurrentLanguage().empty() ? "es" : LocalizationManager::Instance().GetCurrentLanguage();
	const std::string token = lang + "|" + ToUtf8(UpperCopy(TrimCopy(input))) + "|" + canonical + "|" + field + "|" + source;
	if (!seen.insert(token).second)
		return;

	std::cerr
		<< "[KW][META][RESOLVE] "
		<< "input=" << ToUtf8(UpperCopy(TrimCopy(input)))
		<< " canonical=" << canonical
		<< " field=" << field
		<< " source=" << source
		<< " lang=" << lang
		<< std::endl;
}

static void BuildUiMetaAliases() {
	g_meta_aliases.clear();
	std::set<std::string> conflicted;

	for (const auto &entry : g_specs) {
		const std::string &canonical = entry.first;
		const KeywordRuntimeSpec &spec = entry.second;
		if (!IsUiMetaCategory(spec.category))
			continue;

		for (const auto &alias_wx : SplitAliases(spec.aliases)) {
			std::string alias = ToUtf8(UpperCopy(alias_wx));
			if (alias.empty() || alias == canonical || conflicted.count(alias))
				continue;

			auto it = g_meta_aliases.find(alias);
			if (it != g_meta_aliases.end() && it->second != canonical) {
				LogMetaAliasConflict(alias, it->second, canonical);
				g_meta_aliases.erase(it);
				conflicted.insert(alias);
				continue;
			}

			g_meta_aliases[alias] = canonical;
			LogMetaAliasLoadOnce(alias, canonical);
		}
	}
}

static const KeywordRuntimeSpec *ResolveUiMetaSpecForWord(const wxString &word, std::string *canonical_id, bool *resolved_alias) {
	if (resolved_alias) *resolved_alias = false;

	wxString normalized = UpperCopy(TrimCopy(word));
	if (normalized.IsEmpty())
		return nullptr;

	const std::string normalized_utf8 = ToUtf8(normalized);
	auto alias_it = g_meta_aliases.find(normalized_utf8);
	if (alias_it != g_meta_aliases.end()) {
		auto spec_it = g_specs.find(alias_it->second);
		if (spec_it != g_specs.end()) {
			if (canonical_id) *canonical_id = alias_it->second;
			if (resolved_alias) *resolved_alias = true;
			return &spec_it->second;
		}
	}

	return FindSpec(word, canonical_id);
}

static wxString GetVisibleFormOrFallback(KeywordType kw, const std::string &fallback) {
	EnsureLoaded();
	if (KeywordRuntimeSpec *spec = FindSpec(kw)) {
		if (!spec->display.IsEmpty())
			return spec->display;
		if (!spec->insert.IsEmpty())
			return spec->insert;
	}
	return _Z(fallback.c_str());
}

static std::vector<wxString> GetVisibleForms(KeywordType kw) {
	EnsureLoaded();
	std::vector<wxString> out;
	if (KeywordRuntimeSpec *spec = FindSpec(kw)) {
		if (!spec->display.IsEmpty())
			out.push_back(spec->display);
		if (!spec->insert.IsEmpty()) {
			bool present = false;
			for (const auto &item : out)
				if (item == spec->insert) {
					present = true;
					break;
				}
			if (!present)
				out.push_back(spec->insert);
		}
		for (const auto &alias : SplitAliases(spec->aliases)) {
			bool present = false;
			for (const auto &item : out)
				if (item == alias) {
					present = true;
					break;
				}
			if (!present)
				out.push_back(alias);
		}
	}
	return out;
}

static bool IsAuditKeywordForm(const wxString &form) {
	wxString normalized = TrimCopy(form);
	if (normalized.IsEmpty()) return false;
	normalized.MakeUpper();
	return normalized == "READ" || normalized == "LIRE" || normalized == "WRITE" || normalized == "ECRIRE"
		|| normalized == "LEER" || normalized == "ESCRIBIR"
		|| normalized == "DEFINE" || normalized == "AS" || normalized == "DEFINIR" || normalized == "COMME" || normalized == "COMO";
}

} // namespace

wxString GetKeywordDisplayForm(KeywordType kw, const std::string &fallback) {
	return GetVisibleFormOrFallback(kw, fallback);
}

wxString GetKeywordInsertionForm(KeywordType kw, const std::string &fallback) {
	EnsureLoaded();
	if (KeywordRuntimeSpec *spec = FindSpec(kw)) {
		if (!spec->insert.IsEmpty())
			return spec->insert;
		if (!spec->display.IsEmpty())
			return spec->display;
	}
	return _Z(fallback.c_str());
}

wxString GetKeywordTooltipForm(KeywordType kw, const std::string &fallback) {
	EnsureLoaded();
	if (KeywordRuntimeSpec *spec = FindSpec(kw))
		return GetFieldOrFallback(spec->tooltip, fallback);
	return _Z(fallback.c_str());
}

wxString GetKeywordCanonicalForWord(const wxString &word) {
	EnsureLoaded();
	std::string canonical;
	bool resolved_alias = false;
	if (ResolveUiMetaSpecForWord(word, &canonical, &resolved_alias)) {
		if (resolved_alias)
			LogMetaResolveOnce(word, canonical, "canonical", "alias");
		return DecodeBytes(canonical);
	}
	return wxEmptyString;
}

wxString GetKeywordDisplayFormForWord(const wxString &word) {
	EnsureLoaded();
	std::string canonical;
	bool resolved_alias = false;
	if (const KeywordRuntimeSpec *spec = ResolveUiMetaSpecForWord(word, &canonical, &resolved_alias)) {
		if (!spec->display.IsEmpty()) {
			if (resolved_alias)
				LogMetaResolveOnce(word, canonical, "display", "alias");
			LogKeywordMetaOnce(canonical, spec->category, "display", "keywords");
			return spec->display;
		}
		if (!spec->insert.IsEmpty()) {
			if (resolved_alias)
				LogMetaResolveOnce(word, canonical, "display", "alias");
			LogKeywordMetaOnce(canonical, spec->category, "display", "keywords");
			return spec->insert;
		}
	}
	return wxEmptyString;
}

wxString GetKeywordTooltipFormForWord(const wxString &word) {
	EnsureLoaded();
	std::string canonical;
	bool resolved_alias = false;
	if (const KeywordRuntimeSpec *spec = ResolveUiMetaSpecForWord(word, &canonical, &resolved_alias)) {
		if (!spec->tooltip.IsEmpty()) {
			if (resolved_alias)
				LogMetaResolveOnce(word, canonical, "tooltip", "alias");
			LogKeywordMetaOnce(canonical, spec->category, "tooltip", "keywords");
			return spec->tooltip;
		}
	}
	return wxEmptyString;
}

wxString GetKeywordCategoryForWord(const wxString &word) {
	EnsureLoaded();
	std::string canonical;
	bool resolved_alias = false;
	if (const KeywordRuntimeSpec *spec = ResolveUiMetaSpecForWord(word, &canonical, &resolved_alias)) {
		if (resolved_alias)
			LogMetaResolveOnce(word, canonical, "category", "alias");
		return LowerCopy(TrimCopy(spec->category));
	}
	return wxEmptyString;
}

wxString GetKeywordHelpPageForWord(const wxString &word) {
	EnsureLoaded();
	std::string canonical;
	bool resolved_alias = false;
	if (const KeywordRuntimeSpec *spec = ResolveUiMetaSpecForWord(word, &canonical, &resolved_alias)) {
		if (!spec->help.IsEmpty()) {
			if (resolved_alias)
				LogMetaResolveOnce(word, canonical, "help", "alias");
			LogKeywordMetaOnce(canonical, spec->category, "help", "keywords");
			return spec->help;
		}
	}
	return wxEmptyString;
}

std::string GetKeywordRuntimeLexWords() {
	EnsureLoaded();
	std::set<std::string> words;
	for (const auto &entry : g_specs) {
		const KeywordRuntimeSpec &spec = entry.second;
		auto add_word = [&](const wxString &value) {
			wxString word = TrimCopy(value);
			if (!word.IsEmpty())
				words.insert(ToUtf8(word));
		};
		add_word(spec.display);
		add_word(spec.insert);
		for (const auto &alias : SplitAliases(spec.aliases))
			add_word(alias);
	}

	std::string out;
	for (const auto &word : words) {
		if (!out.empty()) out += ' ';
		out += word;
	}
	return out;
}

std::string GetKeywordRuntimeLexWords(KeywordType kw) {
	EnsureLoaded();
	std::set<std::string> words;
	std::vector<wxString> forms;
	GetKeywordVisibleForms(kw, forms);
	for (const auto &form : forms) {
		wxString word = TrimCopy(form);
		if (!word.IsEmpty())
			words.insert(ToUtf8(word));
	}

	std::string out;
	for (const auto &word : words) {
		if (!out.empty()) out += ' ';
		out += word;
	}
	return out;
}

bool IsKeywordRuntimeWord(KeywordType kw, const std::string &candidate) {
	EnsureLoaded();
	const KeywordRuntimeSpec *spec = FindSpec(kw);
	if (!spec) return false;

	const std::string lang = LocalizationManager::Instance().GetCurrentLanguage();
	std::cerr << "KWTRACE ENTER RuntimeHelper lang=" << lang
	          << " canonical=" << (kw==KW_LEER ? "LEER" : (kw==KW_ESCRIBIR ? "ESCRIBIR" : "OTHER"))
	          << " raw=" << candidate
	          << std::endl;
	wxString normalized = DecodeBytes(candidate);
	normalized = TrimCopy(normalized);
	if (normalized.IsEmpty()) return false;
	normalized.MakeUpper();

	auto matches = [&](const wxString &value) {
		wxString current = TrimCopy(value);
		if (current.IsEmpty()) return false;
		current.MakeUpper();
		return current == normalized;
	};

	bool result = matches(spec->display) || matches(spec->insert);
	for (const auto &alias : SplitAliases(spec->aliases)) {
		if (matches(alias)) {
			result = true;
			break;
		}
	}
	if (IsAuditKeywordForm(normalized)) {
		std::cerr << "KWTRACE RuntimeHelper"
		          << " lang=" << lang
		          << " canonical=" << (kw==KW_LEER ? "LEER" : (kw==KW_ESCRIBIR ? "ESCRIBIR" : "OTHER"))
		          << " raw=" << ToUtf8(candidate)
		          << " normalized=" << ToUtf8(normalized)
		          << " display=" << ToUtf8(spec->display)
		          << " insert=" << ToUtf8(spec->insert)
		          << " aliases=" << ToUtf8(spec->aliases)
		          << " result=" << (result ? "yes" : "no")
		          << std::endl;
	}
	return result;
}

void GetKeywordVisibleForms(KeywordType kw, std::vector<wxString> &out) {
	std::vector<wxString> forms = GetVisibleForms(kw);
	out.insert(out.end(), forms.begin(), forms.end());
}

void GetKeywordVisibleForms(KeywordType kw, std::vector<std::string> &out) {
	std::vector<wxString> forms;
	GetKeywordVisibleForms(kw, forms);
	for (const auto &form : forms) {
		out.push_back(ToUtf8(form));
	}
}
