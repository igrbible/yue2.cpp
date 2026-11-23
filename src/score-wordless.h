// score-wordless.h: a planned section no lyric line is sung in keeps its
// chords and its instrument, while the vocal voice rests through it.
//
// The planner writes notes into the Vocal voice of an intro, an outro or a
// section the lyrics leave empty; with no syllable to carry them they are sung
// as a wordless "aaah". Sections are matched to the lyrics' [tag] blocks in
// order and by kind, as the lyric schedule matches them.
#pragma once

#include <cctype>
#include <cstring>
#include <string>
#include <vector>

struct Yue2LyricBlock {
    std::string kind;
    bool        sung = false;
};

static std::vector<std::string> yue2_wordless_words(const std::string & label) {
    std::vector<std::string> words;
    std::string              word;
    for (unsigned char c : label) {
        if (std::isalnum(c)) {
            word += (char) std::tolower(c);
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

static std::string yue2_wordless_kind(const std::string & label) {
    std::vector<std::string> words = yue2_wordless_words(label);
    for (size_t i = 0; i < words.size(); i++) {
        if (words[i] == "prechorus" || (words[i] == "pre" && i + 1 < words.size() && words[i + 1] == "chorus")) {
            return "pre-chorus";
        }
    }
    for (const std::string & word : words) {
        if (word == "verse" || word == "chorus" || word == "bridge" || word == "intro" || word == "outro" ||
            word == "interlude") {
            return word;
        }
    }
    std::string all;
    for (const std::string & word : words) {
        all += (all.empty() ? "" : " ") + word;
    }
    return all;
}

static bool yue2_wordless_played(const std::string & label) {
    for (const std::string & word : yue2_wordless_words(label)) {
        if (word == "intro" || word == "outro" || word == "instrumental" || word == "solo" || word == "break" ||
            word == "interlude") {
            return true;
        }
    }
    return false;
}

static std::vector<Yue2LyricBlock> yue2_wordless_blocks(const std::string & lyrics) {
    std::vector<Yue2LyricBlock> blocks;
    size_t                      from = 0;
    while (from <= lyrics.size()) {
        size_t      nl   = lyrics.find('\n', from);
        std::string line = lyrics.substr(from, nl == std::string::npos ? std::string::npos : nl - from);
        from             = nl == std::string::npos ? lyrics.size() + 1 : nl + 1;
        size_t a         = line.find_first_not_of(" \t\r");
        size_t b         = line.find_last_not_of(" \t\r");
        if (a == std::string::npos) {
            continue;
        }
        line = line.substr(a, b - a + 1);
        if (line.size() >= 2 && line.front() == '[' && line.back() == ']') {
            blocks.push_back({ yue2_wordless_kind(line.substr(1, line.size() - 2)), false });
            continue;
        }
        if (!blocks.empty()) {
            for (unsigned char c : line) {
                if (std::isalnum(c) || c >= 0x80) {
                    blocks.back().sung = true;
                    break;
                }
            }
        }
    }
    return blocks;
}

// The notes of a vocal line as rests of the same length; chord symbols, inline
// fields, bar lines and rests stay as they are.
static std::string yue2_wordless_rest_line(const std::string & line) {
    std::string out;
    size_t      i = 0;
    while (i < line.size()) {
        char c = line[i];
        if (c == '"') {
            size_t close = line.find('"', i + 1);
            size_t end   = close == std::string::npos ? line.size() : close + 1;
            out += line.substr(i, end - i);
            i = end;
            continue;
        }
        if (c == '[' && i + 1 < line.size() && std::isalpha((unsigned char) line[i + 1]) && i + 2 < line.size() &&
            line[i + 2] == ':') {
            size_t close = line.find(']', i);
            size_t end   = close == std::string::npos ? line.size() : close + 1;
            out += line.substr(i, end - i);
            i = end;
            continue;
        }
        size_t at = i;
        while (at < line.size() && (line[at] == '^' || line[at] == '_' || line[at] == '=')) {
            at++;
        }
        if (at < line.size() && std::strchr("ABCDEFGabcdefg", line[at]) && line[at] != '\0') {
            at++;
            while (at < line.size() && (line[at] == ',' || line[at] == '\'')) {
                at++;
            }
            size_t digits = at;
            while (at < line.size() && std::isdigit((unsigned char) line[at])) {
                at++;
            }
            out += 'z';
            out += line.substr(digits, at - digits);
            if (at < line.size() && line[at] == '-') {
                at++;
            }
            i = at;
            continue;
        }
        out += c;
        i++;
    }
    return out;
}

// A tie at the end of a sung line cannot run into a rest
static void yue2_wordless_untie(std::string * line) {
    size_t at = line->find_last_not_of(" \t|");
    if (at != std::string::npos && (*line)[at] == '-') {
        line->erase(at, 1);
    }
}

// The plan with its unsung sections' vocal notes made rests; the plan itself
// when the lyrics carry no section tags to tell them by.
static std::string yue2_score_silence_unsung(const std::string & abc, const std::string & lyrics) {
    std::vector<Yue2LyricBlock> blocks = yue2_wordless_blocks(lyrics);
    if (blocks.empty()) {
        return abc;
    }
    std::vector<std::string> lines;
    size_t                   from = 0;
    while (from < abc.size()) {
        size_t nl = abc.find('\n', from);
        lines.push_back(abc.substr(from, nl == std::string::npos ? std::string::npos : nl - from));
        from = nl == std::string::npos ? abc.size() : nl + 1;
    }
    size_t      next      = 0;
    bool        silent    = false;
    bool        vocal     = false;
    long        last_sung = -1;
    std::string voice;
    for (size_t index = 0; index < lines.size(); index++) {
        std::string & line = lines[index];
        if (line.compare(0, 2, "% ") == 0) {
            std::string label = line.substr(2);
            std::string kind  = yue2_wordless_kind(label);
            size_t      found = next;
            while (found < blocks.size() && blocks[found].kind != kind) {
                found++;
            }
            if (found < blocks.size()) {
                next   = found + 1;
                silent = !blocks[found].sung;
            } else {
                silent = yue2_wordless_played(label);
            }
            continue;
        }
        if (line.size() >= 2 && line[0] == 'V' && line[1] == ':') {
            std::string name = line.substr(2);
            size_t      a    = name.find_first_not_of(" \t");
            name             = a == std::string::npos ? std::string() : name.substr(a);
            vocal            = name.compare(0, 5, "Vocal") == 0;
            continue;
        }
        if (!vocal || line.empty() || line[0] == '%' || line.find('|') == std::string::npos ||
            (line.size() >= 2 && std::isalpha((unsigned char) line[0]) && line[1] == ':')) {
            continue;
        }
        if (silent) {
            if (last_sung >= 0) {
                yue2_wordless_untie(&lines[(size_t) last_sung]);
                last_sung = -1;
            }
            line = yue2_wordless_rest_line(line);
        } else {
            last_sung = (long) index;
        }
    }
    std::string out;
    for (size_t index = 0; index < lines.size(); index++) {
        out += lines[index];
        if (index + 1 < lines.size() || (!abc.empty() && abc.back() == '\n')) {
            out += '\n';
        }
    }
    return out;
}
