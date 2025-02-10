#include <string>

class Authentication {

protected:
    char* authURL = "http://jamin.chickenkiller.com:8000/LockAuth";

public:
    Authentication();
    
    virtual bool validate(std::string token) = 0;
};