#ifndef INTERNET_H
#define INTERNET_H

#include <string>

class Internet {

    public:
        virtual void connect() = 0;
        virtual void disconnect() = 0;
        virtual int get(std::string url) = 0;
        virtual int post(std::string url, std::string payload) = 0;
};

#endif // INTERNET_H