#pragma once

#include <string>
#define BUFFER_SIZE 1024
using namespace std;
class DataBean {

public:
    string name;
    string unit;
    string telephone;
    string mobilePhone;
    string note;

public:
    bool operator== (const DataBean& dataBean);

public:
    DataBean& operator=(const DataBean& dataBean);

public:
    bool operator< (const DataBean& dataBean);

public:
    DataBean(string name) : name(name){};

public:
    DataBean(string name, string unit, string telephone, string mobilePhone, string note) : name(name), unit(unit),
    telephone(telephone), mobilePhone(mobilePhone), note(note) {}


public:
    void printDataBean();

public:
    string getFormatedInfo();

};





