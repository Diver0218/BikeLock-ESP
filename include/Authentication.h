#include <string>

class Authentication {

protected:
    char* authURL = "http://192.168.178.49:3498/LockAuth/";

public:
    virtual bool validate(std::string token) = 0;
};