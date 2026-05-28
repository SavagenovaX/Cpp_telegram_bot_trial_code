#pragma once
#include <string>
#include <curl/curl.h>
using namespace std;

static size_t WriteCallback(void* c, size_t s,
                             size_t n, string* r) {
    r->append((char*)c, s * n);
    return s * n;
}

string httpGet(const string& url) {
    CURL* curl = curl_easy_init();
    string res;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 35L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return res;
}

string httpPost(const string& url, const string& data) {
    CURL* curl = curl_easy_init();
    string res;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 35L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return res;
}

string urlEncode(const string& s) {
    CURL* curl = curl_easy_init();
    string enc;
    if (curl) {
        char* out = curl_easy_escape(
            curl, s.c_str(), s.length()
        );
        if (out) { enc = out; curl_free(out); }
        curl_easy_cleanup(curl);
    }
    return enc;
}

void send(const string& token,
          const string& chatId,
          const string& text) {
    string url  = "https://api.telegram.org/bot"
                + token + "/sendMessage";
    string data = "chat_id=" + chatId
                + "&text="   + urlEncode(text)
                + "&parse_mode=Markdown";
    httpPost(url, data);
}

void sendLocation(const string& token,
                  const string& chatId,
                  const string& lat,
                  const string& lon) {
    string url  = "https://api.telegram.org/bot"
                + token + "/sendLocation";
    string data = "chat_id="    + chatId
                + "&latitude="  + lat
                + "&longitude=" + lon;
    httpPost(url, data);
}

void sendPhoto(const string& token,
               const string& chatId,
               const string& path) {
    string cmd = "curl -s -F chat_id=" + chatId
               + " -F photo=@" + path
               + " https://api.telegram.org/bot"
               + token + "/sendPhoto";
    system(cmd.c_str());
}
