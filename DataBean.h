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

  bool operator==(const DataBean& dataBean);

  DataBean& operator=(const DataBean& dataBean);

  bool operator<(const DataBean& dataBean);

  DataBean(string name) : name(name) {}

  DataBean(string name, string unit, string telephone, string mobilePhone,
           string note)
      : name(name),
        unit(unit),
        telephone(telephone),
        mobilePhone(mobilePhone),
        note(note) {}

  DataBean() {}

  void printDataBean();

  string getFormatedInfo();

  void outputToFile(FILE* file);

  static void inputFromFile(FILE* file);
};
