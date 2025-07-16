/*
write http responser
*/

#include <fstream>
#include <iostream>
#include <string>
#include <curl/curl.h>

// Callback-функция для записи полученных данных
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getHttpPage(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Следовать редиректам
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

int main() {
    std::ofstream out ("/home/volsh/cfprog/Tasks_Generator/src/output.txt");
    std::string url = "https://codeforces.com/problemset/problem/2121/H";
    std::string pageContent = getHttpPage(url);
    out << "Page content:\n" << pageContent << std::endl;
    return 0;
}