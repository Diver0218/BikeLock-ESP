

#include <HTTPClient.h>

class Authentication {
    
private:
    char* authURL;
    HTTPClient httpClient;

public:
    Authentication(char* authURL);
    
    bool authenticate(char* username, char* password);
};