#include "DataBean.h"
#include <string>
#include <string.h>
#define BUFFER_SIZE 1024
#pragma warning(disable:4996)

using namespace std;

bool DataBean::operator==(const DataBean& dataBean) {
  return !this->name.compare(dataBean.name);
}

DataBean& DataBean::operator=(const DataBean& dataBean) {
  this->name        = dataBean.name;
  this->unit        = dataBean.unit;
  this->telephone   = dataBean.telephone;
  this->mobilePhone = dataBean.mobilePhone;
  this->note        = dataBean.note;
  return *this;
}

bool DataBean::operator<(const DataBean& dataBean) {
  // Sort DataBeans By Name.
  if (strcmp(this->name.c_str(), dataBean.name.c_str()) < 0) {
    return true;
  } else {
    return false;
  }
}

string DataBean::getFormatedInfo() {
  char buf[BUFFER_SIZE];

  sprintf(buf,
          "Name: %s\nUnit: %s\nTelephone: %s\nMobilePhone: %s\nNote: %s",
          this->name.c_str(),
          this->unit.c_str(),
          this->telephone.c_str(),
          this->mobilePhone.c_str(),
          this->note.c_str());
  return string(buf);
}

void DataBean::printDataBean() {
  printf("%s\n", this->getFormatedInfo().c_str());
}
