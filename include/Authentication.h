#include "Internet.h"
#include <string>

class Authentication {

protected:
    std::string authURL;
    Internet *internet_module = nullptr;

public:
    Authentication(Internet *internet_module, std::string authURL);
    bool validate(std::string token);
};