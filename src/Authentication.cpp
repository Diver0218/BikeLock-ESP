#include "Authentication.h"

Authentication::Authentication(char* authURL) : authURL(authURL) {}

bool Authentication::authenticate(char* username, char* password){
    httpClient.begin(authURL);
    httpClient.addHeader("Content-Type", "application/json");

    String payload = "{\"username\":\"" + String(username) + "\",\"password\":\"" + String(password) + "\"}";
    int httpResponseCode = httpClient.POST(payload);

    if (httpResponseCode == 200) {
        httpClient.end();
        return true;
    }
    else if (httpResponseCode == 401) {
        httpClient.end();
        return false;
    }
    else {
        // server / network error
        // handle error
        httpClient.end();
        return false;
    }
}