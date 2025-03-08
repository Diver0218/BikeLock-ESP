#include "Authentication.h"

Authentication::Authentication(Internet *internet_module, std::string authURL) : internet_module(internet_module), authURL(authURL) {}

bool Authentication::validate(std::string token)
{
    internet_module->connect();
    std::string payload = "{\"token\":\"" + token + "\"}";
    int httpResponseCode = internet_module->post(authURL, payload);
    internet_module->disconnect();

    if (httpResponseCode == 200)
    {
        return true;
    }
    else if (httpResponseCode == 401)
    {
        return false;
    }
    else
    {
        return false;
    }
}