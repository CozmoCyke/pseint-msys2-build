#include "LocalizationManager.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <wx/string.h>
#include <wx/buffer.h>
#include <wx/filename.h>
#include <iostream>

LocalizationManager& LocalizationManager::Instance() {
    static LocalizationManager instance;
    return instance;
}

void LocalizationManager::Clear() {
    m_exact_dict.clear();
    m_dict.clear();
}

void LocalizationManager::SetLangDirectory(const wxString& dir) {
    m_langDir = dir;
}

static inline wxString decode_bytes(const std::string& s) {
    if (s.empty()) return wxString();

    wxString ws = wxString::FromUTF8(s.c_str());
    if (ws.empty() && !s.empty()) {
        ws = wxString::From8BitData(s.c_str());
    }

    return ws;
}

static inline wxString decode_bytes(const char* s) {
    if (!s || !*s) return wxString();

    wxString ws = wxString::FromUTF8(s);
    if (ws.empty() && s[0] != '\0') {
        ws = wxString::From8BitData(s);
    }
    return ws;
}

static inline wxString trim_copy(const wxString& s) {
    wxString out = s;
    out.Trim(true);
    out.Trim(false);
    return out;
}

static inline wxString normalize_key_ws(const wxString& input) {
    wxString s = input;

    s.Replace("&", "");

    int tab = s.Find('\t');
    if (tab != wxNOT_FOUND) {
        s = s.Left(tab);
    }

    s = trim_copy(s);
    return s;
}

static inline std::string to_utf8(const wxString& s) {
    wxScopedCharBuffer buf = s.ToUTF8();
    if (!buf) return std::string();
    return std::string(buf.data());
}

static inline std::string normalize_key_utf8(const wxString& input) {
    return to_utf8(normalize_key_ws(input));
}

static inline int find_tab_pos(const wxString& s) {
    return s.Find('\t');
}

static inline wxString label_part(const wxString& s) {
    int tab = find_tab_pos(s);
    return tab == wxNOT_FOUND ? s : s.Left(tab);
}

static inline wxString accel_part(const wxString& s) {
    int tab = find_tab_pos(s);
    return tab == wxNOT_FOUND ? wxString() : s.Mid(tab);
}

static inline bool has_mnemonic(const wxString& s) {
    return s.Find('&') != wxNOT_FOUND;
}

static inline wxString ensure_mnemonic_from_source(const wxString& source, const wxString& translated) {
    if (!has_mnemonic(source) || has_mnemonic(translated)) {
        return translated;
    }

    wxString out = translated;
    for (size_t i = 0; i < out.length(); ++i) {
        if (!wxIsspace(out[i])) {
            out.insert(i, "&");
            return out;
        }
    }

    return "&" + out;
}

static inline wxString merge_translated_label(const wxString& source, const wxString& translated) {
    wxString translated_label = label_part(translated);
    translated_label = ensure_mnemonic_from_source(label_part(source), translated_label);

    wxString translated_accel = accel_part(translated);
    if (translated_accel.IsEmpty()) {
        translated_accel = accel_part(source);
    }

    return translated_label + translated_accel;
}

static inline int find_unescaped_equal(const wxString& s) {
    bool escaped = false;

    for (size_t i = 0; i < s.length(); ++i) {
        wxChar c = s[i];

        if (escaped) {
            escaped = false;
            continue;
        }

        if (c == '\\') {
            escaped = true;
            continue;
        }

        if (c == '=') {
            return static_cast<int>(i);
        }
    }

    return wxNOT_FOUND;
}

static inline wxString unescape_field(const wxString& s) {
    wxString out;
    bool escaped = false;

    for (size_t i = 0; i < s.length(); ++i) {
        wxChar c = s[i];

        if (escaped) {
            if (c == '=' || c == '\\') {
                out += c;
            } else {
                out += '\\';
                out += c;
            }
            escaped = false;
            continue;
        }

        if (c == '\\') {
            escaped = true;
            continue;
        }

        out += c;
    }

    if (escaped) {
        out += '\\';
    }

    return out;
}

bool LocalizationManager::LoadFile(const wxString& filename) {
    Clear();

    std::ifstream f(filename.fn_str(), std::ios::binary);
    if (!f.is_open()) return false;

    std::string rawLine;
    bool firstLine = true;

    while (std::getline(f, rawLine)) {
        if (!rawLine.empty() && rawLine.back() == '\r') {
            rawLine.pop_back();
        }

        wxString line = decode_bytes(rawLine);

        if (firstLine && !line.empty() && line[0] == 0xFEFF) {
            line.Remove(0, 1);
        }
        firstLine = false;

        line = trim_copy(line);

        if (line.empty()) continue;
        if (line[0] == '#') continue;

        int pos = find_unescaped_equal(line);
        if (pos == wxNOT_FOUND) continue;

        wxString key = trim_copy(unescape_field(line.Left(pos)));
        wxString val = trim_copy(unescape_field(line.Mid(pos + 1)));

        if (!key.empty()) {
            m_exact_dict[to_utf8(key)] = to_utf8(val);
            m_dict[normalize_key_utf8(key)] = to_utf8(val);
        }
    }

    return true;
}

bool LocalizationManager::SetLanguage(const char* langCode) {
    std::string code = langCode ? langCode : "es";
    std::cerr << "LocalizationManager::SetLanguage requested=" << code << std::endl;

    wxString codeWx = wxString::FromUTF8(code.c_str());
    wxFileName langFile;

    if (!m_langDir.empty()) {
        langFile.Assign(m_langDir, codeWx + ".txt");
    } else {
        langFile.Assign("lang", codeWx + ".txt");
    }

    langFile.Normalize();
    std::cerr << "LocalizationManager::SetLanguage file=" << langFile.GetFullPath().ToStdString() << std::endl;

    if (LoadFile(langFile.GetFullPath())) {
        m_lang = code;
        std::cerr << "LocalizationManager::SetLanguage loaded lang=" << m_lang << std::endl;
        return true;
    }

    m_lang = "es";
    Clear();
    std::cerr << "LocalizationManager::SetLanguage fallback lang=" << m_lang << std::endl;
    return false;
}

wxString LocalizationManager::Translate(const char* source) const {
    wxString raw = decode_bytes(source);
    std::string exact_key = to_utf8(raw);

    auto exact = m_exact_dict.find(exact_key);
    if (exact != m_exact_dict.end()) {
        return wxString::FromUTF8(exact->second.c_str());
    }

    std::string key = normalize_key_utf8(raw);

    auto it = m_dict.find(key);
    if (it != m_dict.end()) {
        return merge_translated_label(raw, wxString::FromUTF8(it->second.c_str()));
    }

    return raw;
}
