#include "KeywordForms.h"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "LocalizationManager.h"
#include "utils.h"

namespace {

struct KeywordRuntimeSpec {
    std::string display;
    std::string insert;
    std::string aliases;
    std::string help;
    std::string category;
    std::string tooltip;
};

static std::map<std::string, KeywordRuntimeSpec> g_specs;
static std::string g_loaded_language;
static bool g_loaded = false;

static std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return s;
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

static std::string to_upper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

    static std::string CanonicalIdForKeyword(KeywordType kw) {
    switch (kw) {
        case KW_ALGORITMO: return "ALGORITMO";
        case KW_FINALGORITMO: return "FINALGORITMO";
        case KW_POR_COPIA: return "POR_COPIA";
        case KW_POR_REFERENCIA: return "POR_REFERENCIA";
        case KW_SUBALGORITMO: return "SUBALGORITMO";
        case KW_FINSUBALGORITMO: return "FINSUBALGORITMO";
        case KW_SI: return "SI";
        case KW_ENTONCES: return "ENTONCES";
        case KW_SINO: return "SINO";
        case KW_FINSI: return "FINSI";
        case KW_MIENTRAS: return "MIENTRAS";
        case KW_HACER: return "HACER";
        case KW_FINMIENTRAS: return "FINMIENTRAS";
        case KW_REPETIR: return "REPETIR";
        case KW_HASTAQUE: return "HASTAQUE";
        case KW_MIENTRASQUE: return "MIENTRASQUE";
        case KW_PARA: return "PARA";
        case KW_HASTA: return "HASTA";
        case KW_CONPASO: return "CONPASO";
        case KW_FINPARA: return "FINPARA";
        case KW_SEGUN: return "SEGUN";
        case KW_OPCION: return "OPCION";
        case KW_DEOTROMODO: return "DEOTROMODO";
        case KW_FINSEGUN: return "FINSEGUN";
        case KW_LEER: return "LEER";
        case KW_ESCRIBIR: return "ESCRIBIR";
        case KW_ES: return "ES";
        case KW_PARACADA: return "PARACADA";
        case KW_DE: return "DE";
        case KW_DESDE: return "DESDE";
        case KW_DIMENSIONAR: return "DIMENSIONAR";
        case KW_REDIMENSIONAR: return "REDIMENSIONAR";
        case KW_DEFINIR: return "DEFINIR";
        case KW_COMO: return "COMO";
        case KW_TIPO_ENTERO: return "TIPO_ENTERO";
        case KW_TIPO_REAL: return "TIPO_REAL";
        case KW_TIPO_LOGICO: return "TIPO_LOGICO";
        case KW_TIPO_CARACTER: return "TIPO_CARACTER";
        case KW_SIN_SALTAR: return "SIN_SALTAR";
        case KW_LIMPIARPANTALLA: return "LIMPIARPANTALLA";
        case KW_ESPERARTECLA: return "ESPERARTECLA";
        case KW_ESPERARTIEMPO: return "ESPERARTIEMPO";
        case KW_SEGUNDOS: return "SEGUNDOS";
        case KW_MILISEGUNDOS: return "MILISEGUNDOS";
        default: return "";
    }
}

static KeywordRuntimeSpec *FindSpec(KeywordType kw) {
    std::string id = CanonicalIdForKeyword(kw);
    if (id.empty()) return nullptr;
    auto it = g_specs.find(id);
    if (it == g_specs.end()) return nullptr;
    return &it->second;
}

static std::vector<std::string> SplitAliases(const std::string &aliases) {
    std::vector<std::string> out;
    size_t start = 0;
    while (start <= aliases.length()) {
        size_t end = aliases.find(',', start);
        std::string alias = trim(aliases.substr(start, end == std::string::npos ? std::string::npos : end - start));
        if (!alias.empty()) out.push_back(alias);
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return out;
}

static std::string ResolveKeywordsFile(const std::string &lang) {
    std::string path = "keywords/" + lang + ".ini";
    std::ifstream f(path.c_str());
    if (f.is_open()) return path;

    path = "../keywords/" + lang + ".ini";
    std::ifstream f2(path.c_str());
    if (f2.is_open()) return path;

    return "keywords/" + lang + ".ini";
}

static void StoreSpec(const std::string &section, const KeywordRuntimeSpec &spec) {
    std::string key = to_upper(trim(section));
    if (!key.empty())
        g_specs[key] = spec;
}

static bool LoadKeywordsFile(const std::string &path) {
    std::ifstream fil(path.c_str(), std::ios::binary);
    if (!fil.is_open()) return false;

    g_specs.clear();
    KeywordRuntimeSpec current;
    std::string current_section;
    bool first_line = true;

    std::string line;
    while (std::getline(fil, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (first_line && line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }
        first_line = false;

        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        if (line[0] == '[' && line[line.length() - 1] == ']') {
            if (!current_section.empty())
                StoreSpec(current_section, current);
            current = KeywordRuntimeSpec();
            current_section = line.substr(1, line.length() - 2);
            continue;
        }

        size_t pos = line.find('=');
        if (pos == std::string::npos || current_section.empty()) continue;

        std::string key = to_upper(trim(line.substr(0, pos)));
        std::string value = trim(line.substr(pos + 1));

        if (key == "DISPLAY") current.display = value;
        else if (key == "INSERT") current.insert = value;
        else if (key == "ALIASES") current.aliases = value;
        else if (key == "HELP") current.help = value;
        else if (key == "CATEGORY") current.category = value;
        else if (key == "TOOLTIP") current.tooltip = value;
    }

    if (!current_section.empty())
        StoreSpec(current_section, current);

    return !g_specs.empty();
}

static void EnsureLoaded() {
    std::string lang = LocalizationManager::Instance().GetCurrentLanguage();
    if (g_loaded && g_loaded_language == lang) return;

    g_loaded = false;
    g_loaded_language = lang;
    g_specs.clear();

    std::string path = ResolveKeywordsFile(lang);
    std::cerr << "KWFIX overlay path=" << path << std::endl;
    LoadKeywordsFile(path);

    g_loaded = true;
}

} // namespace

std::string GetKeywordRuntimeLexWords() {
    EnsureLoaded();
    std::set<std::string> words;
    for (const auto &entry : g_specs) {
        const KeywordRuntimeSpec &spec = entry.second;
        if (!spec.display.empty()) words.insert(spec.display);
        if (!spec.insert.empty()) words.insert(spec.insert);
        for (const auto &alias : SplitAliases(spec.aliases))
            words.insert(alias);
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
    if (KeywordRuntimeSpec *spec = FindSpec(kw)) {
        if (!spec->display.empty()) words.insert(spec->display);
        if (!spec->insert.empty()) words.insert(spec->insert);
        for (const auto &alias : SplitAliases(spec->aliases))
            words.insert(alias);
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
    KeywordRuntimeSpec *spec = FindSpec(kw);
    if (!spec) return false;

    std::string normalized = to_upper(trim(candidate));
    if (normalized.empty()) return false;

    auto matches = [&](const std::string &value) {
        std::string val_norm = to_upper(trim(value));
        bool res = (val_norm == normalized);
        if (kw == KW_DEFINIR || kw == KW_COMO) {
            std::cerr << "KWTRACE RuntimeHelper lang=" << g_loaded_language 
                      << " canonical=" << CanonicalIdForKeyword(kw)
                      << " raw=" << candidate
                      << " normalized=" << normalized
                      << " display=" << spec->display
                      << " insert=" << spec->insert
                      << " aliases=" << spec->aliases
                      << " check_val=" << value
                      << " result=" << (res ? "yes" : "no")
                      << std::endl;
        }
        return res;
    };

    if (matches(spec->display) || matches(spec->insert)) return true;
    for (const auto &alias : SplitAliases(spec->aliases)) {
        if (matches(alias)) return true;
    }
    return false;
}

void GetKeywordVisibleForms(KeywordType kw, std::vector<std::string> &out) {
    EnsureLoaded();
    if (KeywordRuntimeSpec *spec = FindSpec(kw)) {
        if (!spec->display.empty()) out.push_back(spec->display);
        if (!spec->insert.empty()) {
            if (std::find(out.begin(), out.end(), spec->insert) == out.end())
                out.push_back(spec->insert);
        }
        for (const auto &alias : SplitAliases(spec->aliases)) {
            if (std::find(out.begin(), out.end(), alias) == out.end())
                out.push_back(alias);
        }
    }
}
