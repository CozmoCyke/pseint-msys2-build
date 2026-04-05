#include "FuncsManager.hpp"
#include "ErrorHandler.hpp"
#include "utils.h"
#include "strFuncs.hpp"
#include "intercambio.h"
#include "global.h"
#include "DataValue.h"
#include "LocalizationManager.h"
#include <fstream>
#include <sstream>
#include <set>
using namespace std;

namespace {

struct KeywordPredefOverlay {
	std::string lang;
	std::set<std::string> declared_predefs;
	std::set<std::string> canonicals;
	std::map<std::string,std::string> aliases;
};

struct ResolvedPredef {
	const Funcion *func = nullptr;
	std::string canonical;
	std::string normalized;
	std::string alias;
	enum Source { canonical_source, alias_source, miss_source } source = miss_source;
};

static std::string TrimCopy(const std::string &s) {
	size_t first = s.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) return "";
	size_t last = s.find_last_not_of(" \t\r\n");
	return s.substr(first, last - first + 1);
}

static std::string CurrentKeywordLang() {
	std::string lang = LocalizationManager::Instance().GetCurrentLanguage();
	if (lang.empty()) lang = "es";
	return lang;
}

static std::string NormalizePredefName(std::string s) {
	for (char &c : s) c = Normalize(c);
	return s;
}

static std::string LowerAsciiCopy(std::string s) {
	for (char &c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	return s;
}

static std::string ResolveKeywordsPath() {
	std::string lang_code = CurrentKeywordLang();
	std::string path = "keywords/" + lang_code + ".ini";
	std::ifstream f(path.c_str());
	if (f.good()) return path;
	path = "../keywords/" + lang_code + ".ini";
	std::ifstream f2(path.c_str());
	if (f2.good()) return path;
	return "keywords/" + lang_code + ".ini";
}

static std::vector<std::string> SplitAliases(const std::string &s) {
	std::vector<std::string> out;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, ',')) {
		item = TrimCopy(item);
		if (!item.empty()) out.push_back(item);
	}
	return out;
}

static const KeywordPredefOverlay &GetKeywordPredefOverlay() {
	static KeywordPredefOverlay overlay;
	std::string lang = CurrentKeywordLang();
	if (overlay.lang == lang) return overlay;

	overlay = KeywordPredefOverlay();
	overlay.lang = lang;

	std::ifstream fil(ResolveKeywordsPath().c_str(), std::ios::binary);
	if (!fil.is_open()) return overlay;

	std::string line;
	const std::string prefix = "keyword.";
	const std::string suffix = ".category";
	const std::string aliases_suffix = ".aliases";
	std::map<std::string,std::vector<std::string>> aliases_by_canonical;
	while (std::getline(fil, line)) {
		line = TrimCopy(line);
		if (line.empty() || line[0] == '#') continue;
		if (line.compare(0, prefix.size(), prefix) != 0) continue;
		size_t eq = line.find('=');
		if (eq == std::string::npos) continue;
		std::string key = TrimCopy(line.substr(0, eq));
		std::string raw_value = TrimCopy(line.substr(eq + 1));
		size_t suffix_pos = key.rfind(suffix);
		if (suffix_pos != std::string::npos && suffix_pos > prefix.size()) {
			std::string value = NormalizePredefName(raw_value);
			if (value == "PREDEF") {
				std::string canonical = key.substr(prefix.size(), suffix_pos - prefix.size());
				overlay.declared_predefs.insert(NormalizePredefName(canonical));
			}
			continue;
		}
		size_t aliases_pos = key.rfind(aliases_suffix);
		if (aliases_pos != std::string::npos && aliases_pos > prefix.size()) {
			std::string canonical = key.substr(prefix.size(), aliases_pos - prefix.size());
			aliases_by_canonical[NormalizePredefName(canonical)] = SplitAliases(raw_value);
		}
	}

	std::set<std::string> claimed_aliases;
	for (const auto &entry : aliases_by_canonical) {
		const std::string &canonical = entry.first;
		for (const std::string &raw_alias : entry.second) {
			std::string alias = NormalizePredefName(raw_alias);
			if (alias.empty()) continue;
			auto existing = overlay.aliases.find(alias);
			if (existing != overlay.aliases.end() && existing->second != canonical) {
				std::cerr
					<< "[KW][ALIAS][CONFLICT] "
					<< "alias=" << LowerAsciiCopy(alias)
					<< " canonical1=" << existing->second
					<< " canonical2=" << canonical
					<< " lang=" << overlay.lang
					<< std::endl;
				overlay.aliases.erase(existing);
				claimed_aliases.insert(alias);
				continue;
			}
			if (!claimed_aliases.count(alias)) {
				overlay.aliases[alias] = canonical;
				claimed_aliases.insert(alias);
			}
		}
	}

	for (const std::string &name : overlay.declared_predefs) {
		if (!claimed_aliases.count(name)) overlay.canonicals.insert(name);
	}

	return overlay;
}

static void EnsureKeywordPredefsOverlayLogged(const FuncsManager::FuncsContainer &predefs) {
	static std::string logged_lang;
	const KeywordPredefOverlay &overlay = GetKeywordPredefOverlay();
	if (logged_lang == overlay.lang) return;
	logged_lang = overlay.lang;

	for (const std::string &canonical : overlay.declared_predefs) {
		std::string normalized = LowerAsciiCopy(NormalizePredefName(canonical));
		std::cerr
			<< "[KW][LOAD][PREDEF] "
			<< "canonical=" << canonical
			<< " normalized=" << normalized
			<< " source=keywords"
			<< " match=" << (predefs.count(canonical) ? "existing" : "missing")
			<< " lang=" << overlay.lang
			<< std::endl;
	}

	for (const auto &entry : overlay.aliases) {
		std::cerr
			<< "[KW][LOAD][ALIAS] "
			<< "alias=" << LowerAsciiCopy(entry.first)
			<< " canonical=" << entry.second
			<< " lang=" << overlay.lang
			<< std::endl;
	}
}

static ResolvedPredef ResolvePredefName(const FuncsManager::FuncsContainer &predefs, const std::string &name) {
	ResolvedPredef resolved;
	resolved.normalized = LowerAsciiCopy(NormalizePredefName(name));
	std::string normalized_upper = NormalizePredefName(name);
	const KeywordPredefOverlay &overlay = GetKeywordPredefOverlay();

	if (overlay.canonicals.count(normalized_upper)) {
		auto it = predefs.find(normalized_upper);
		if (it != predefs.end()) {
			resolved.func = it->second.get();
			resolved.canonical = normalized_upper;
			resolved.source = ResolvedPredef::canonical_source;
			return resolved;
		}
	}

	auto alias_it = overlay.aliases.find(normalized_upper);
	if (alias_it != overlay.aliases.end()) {
		auto it = predefs.find(alias_it->second);
		if (it != predefs.end()) {
			resolved.func = it->second.get();
			resolved.canonical = alias_it->second;
			resolved.alias = LowerAsciiCopy(alias_it->first);
			resolved.source = ResolvedPredef::alias_source;
			return resolved;
		}
	}

	auto it = predefs.find(normalized_upper);
	if (it != predefs.end()) {
		resolved.func = it->second.get();
		resolved.canonical = normalized_upper;
		resolved.source = ResolvedPredef::canonical_source;
		return resolved;
	}

	return resolved;
}

static void LogResolvedPredef(const std::string &input, const ResolvedPredef &resolved) {
	std::string lang = CurrentKeywordLang();
	std::cerr
		<< "[KW][RESOLVE][PREDEF] "
		<< "input=" << LowerAsciiCopy(input)
		<< " normalized=" << resolved.normalized;
	if (resolved.source == ResolvedPredef::canonical_source) {
		std::cerr
			<< " source=canonical"
			<< " canonical=" << resolved.canonical;
	} else if (resolved.source == ResolvedPredef::alias_source) {
		std::cerr
			<< " source=alias"
			<< " canonical=" << resolved.canonical
			<< " alias=" << resolved.alias;
	} else {
		std::cerr << " source=miss";
	}
	std::cerr << " lang=" << lang << std::endl;
}

} // namespace

void FuncsManager::UnloadSubprocesos() {
	m_subs.clear();
	m_main_name.clear();
}

void FuncsManager::UnloadPredefs() {
	m_predefs.clear();
}

FuncsManager::~FuncsManager ( ) {
	UnloadPredefs();
	UnloadSubprocesos();
}

void FuncsManager::AddSub(std::unique_ptr<Funcion> &&func) {
	m_subs[func->id] = std::move(func);
}

const Funcion *FuncsManager::GetSub(const std::string &name) const {
	auto it = m_subs.find(name);
	_expects(it!=m_subs.end());
	return it->second.get();
}

const Funcion *FuncsManager::GetFunction(const std::string &name, bool must_exists) const {
	EnsureKeywordPredefsOverlayLogged(m_predefs);
	ResolvedPredef resolved = ResolvePredefName(m_predefs, name);
	if (resolved.func != nullptr) {
		LogResolvedPredef(name, resolved);
		return resolved.func;
	}
	auto it = m_subs.find(name);
	if(it!=m_subs.end()) return it->second.get();
	LogResolvedPredef(name, resolved);
	_expects(not must_exists);
	return nullptr;
}

Funcion *FuncsManager::GetEditableSub(const std::string &name) {
	auto it = m_subs.find(name);
	_expects(it!=m_subs.end());
	return it->second.get();
}

bool FuncsManager::IsSub(const std::string &name) const {
	return m_subs.count(name)!=0;
}

bool FuncsManager::IsPredef(const std::string &name) const {
	EnsureKeywordPredefsOverlayLogged(m_predefs);
	ResolvedPredef resolved = ResolvePredefName(m_predefs, name);
	LogResolvedPredef(name, resolved);
	return resolved.func != nullptr;
}

bool FuncsManager::IsFunction(const std::string &name) const {
	return IsPredef(name) or IsSub(name);
}

bool FuncsManager::HaveMain() const { 
	return not m_main_name.empty(); 
}

void FuncsManager::SetMain(const std::string &name) { 
	_expects(m_subs.count(name));
	m_main_name = name;
}

std::string FuncsManager::GetMainName() const {
	return m_main_name; 
}

const Funcion *FuncsManager::GetMainFunc() const {
	return GetSub(m_main_name);
}
