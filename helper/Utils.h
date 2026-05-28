#pragma once
#include <string>
#include <cstdio>
using namespace std;

// ── Run Shell Command ─────────────────────────────────────
string runCmd(const string& cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    char buf[256];
    string res;
    while (fgets(buf, sizeof(buf), pipe)) res += buf;
    pclose(pipe);
    return res;
}

// ── Get JSON Value ────────────────────────────────────────
string jsonGet(const string& json, const string& key) {
    string search = "\"" + key + "\":";
    size_t pos    = json.find(search);
    if (pos == string::npos) return "N/A";
    pos += search.length();
    while (pos < json.size() && json[pos] == ' ') pos++;
    if (json[pos] == '"') {
        pos++;
        size_t end = json.find('"', pos);
        return json.substr(pos, end - pos);
    }
    size_t end = json.find_first_of(",}", pos);
    return json.substr(pos, end - pos);
}

// ── Get Timestamp ─────────────────────────────────────────
string timestamp() {
    return to_string(time(nullptr));
}
