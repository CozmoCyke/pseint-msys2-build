#include "LocalizationManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>

// Global variable to store the executable path
static std::string g_exe_path_str;

// Function to set the executable path from main
void SetExecutablePath(const char* argv0) {
    if (argv0) {
        std::string path = argv0;
        size_t pos = path.find_last_of("/\\");
        if (pos != std::string::npos) {
            g_exe_path_str = path.substr(0, pos);
        }
    }
}

// Singleton instance getter
LocalizationManager& LocalizationManager::Instance() {
    static LocalizationManager instance;
    return instance;
}

// Clear loaded translations
void LocalizationManager::Clear() {
    m_exact_dict.clear();
    m_dict.clear();
}

void LocalizationManager::SetLangDirectory(const std::string& dir) {
    m_langDir = dir;
}

// Helper functions (trim, normalize_key)
static std::string trim(const std::string& s) {
    const char* whitespace = " \t\r\n";
    size_t first = s.find_first_not_of(whitespace);
    if (std::string::npos == first) return "";
    size_t last = s.find_last_not_of(whitespace);
    return s.substr(first, (last - first + 1));
}

static std::string normalize_key(const std::string& s) {
    std::string out = trim(s);
    size_t pos;
    while ((pos = out.find('&')) != std::string::npos) {
        out.erase(pos, 1);
    }
    return out;
}

static void skip_spaces(const std::string& s, size_t& i) {
    while (i < s.size() && (s[i] == ' ' || s[i] == '\t')) {
        ++i;
    }
}

static bool parse_quoted_string(const std::string& s, size_t& i, std::string& out) {
    out.clear();
    if (i >= s.size() || s[i] != '"') return false;
    ++i; // skip opening quote

    while (i < s.size()) {
        char c = s[i++];

        if (c == '\\') {
            if (i >= s.size()) return false;
            char n = s[i++];
            switch (n) {
                case '\\': out.push_back('\\'); break;
                case '"':  out.push_back('"');  break;
                case 'n':  out.push_back('\n'); break;
                case 't':  out.push_back('\t'); break;
                case 'r':  out.push_back('\r'); break;
                default:
                    // conserve l'échappement inconnu tel quel
                    out.push_back(n);
                    break;
            }
        } else if (c == '"') {
            return true; // closing quote
        } else {
            out.push_back(c);
        }
    }

    return false;
}

static bool parse_exact_quoted_assignment(const std::string& line, std::string& key, std::string& val) {
    size_t i = 0;
    skip_spaces(line, i);

    if (i >= line.size() || line[i] != '"') return false;
    if (!parse_quoted_string(line, i, key)) return false;

    skip_spaces(line, i);
    if (i >= line.size() || line[i] != '=') return false;
    ++i;

    skip_spaces(line, i);
    if (i >= line.size() || line[i] != '"') return false;
    if (!parse_quoted_string(line, i, val)) return false;

    skip_spaces(line, i);
    if (i != line.size()) return false;

    return true;
}

// File loading implementation
bool LocalizationManager::LoadFile(const std::string& filename) {
    Clear();
    std::ifstream f(filename.c_str());
    if (!f.is_open()) return false;

    std::string line;
    bool firstLine = true;
    while (std::getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (firstLine && line.size() >= 3 &&
            (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }
        firstLine = false;

        // IMPORTANT:
        // - on garde 'line' brute pour pouvoir préserver les espaces significatifs
        // - on utilise une version trimée seulement pour détecter vide/commentaire
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') continue;

        // Nouveau format exact quoted:
        // "clé exacte "="valeur exacte "
        std::string key, val;
        if (parse_exact_quoted_assignment(line, key, val)) {
            m_exact_dict[key] = val;
            continue;
        }

        // Ancien format historique:
        // cle=val
        size_t pos = trimmed.find('=');
        if (pos == std::string::npos) continue;

        key = trim(trimmed.substr(0, pos));
        val = trim(trimmed.substr(pos + 1));

        if (!key.empty()) {
            m_exact_dict[key] = val;
            m_dict[normalize_key(key)] = val;
        }
    }
    return true;
}

// SetLanguage implementation with robust path finding
bool LocalizationManager::SetLanguage(const std::string& langCode) {
    std::string code = langCode.empty() ? "es" : langCode;
    std::cerr << "LANGTRACE SetLanguage requested=" << code << std::endl;

    std::vector<std::string> paths;
    if (!g_exe_path_str.empty()) {
        paths.push_back(g_exe_path_str + "/../lang/" + code + ".txt");
        paths.push_back(g_exe_path_str + "/lang/" + code + ".txt");
    }
    if (!m_langDir.empty()) {
        paths.push_back(m_langDir + "/" + code + ".txt");
    }
    paths.push_back("../lang/" + code + ".txt");
    paths.push_back("lang/" + code + ".txt");

    for (const std::string& path : paths) {
        std::cerr << "LANGTRACE SetLanguage try path=" << path << std::endl;
        if (LoadFile(path)) {
            m_lang = code;
            std::cerr << "LANGTRACE SetLanguage loaded path=" << path << std::endl;
            return true;
        }
    }

    std::cerr << "LANGTRACE SetLanguage failed, falling back to es" << std::endl;
    m_lang = "es";
    Clear();
    return false;
}

// Translate implementation
std::string LocalizationManager::Translate(const char* source) const {
    std::string key = source ? source : "";

    auto exact = m_exact_dict.find(key);
    if (exact != m_exact_dict.end()) return exact->second;

    auto it = m_dict.find(normalize_key(key));
    if (it != m_dict.end()) return it->second;

    return key;
}