#include "DataBean.h"

#include <string.h>

#include <qDebug>
#include <string>

#include "SystemInterface.h"

#define BUFFER_SIZE 1024
#pragma warning(disable : 4996)

using namespace std;

bool DataBean::operator==(const DataBean& dataBean) {
  return !this->name.compare(dataBean.name);
}

DataBean& DataBean::operator=(const DataBean& dataBean) {
  this->name = dataBean.name;
  this->unit = dataBean.unit;
  this->telephone = dataBean.telephone;
  this->mobilePhone = dataBean.mobilePhone;
  this->note = dataBean.note;
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

  sprintf(buf, "Name: %s\nUnit: %s\nTelephone: %s\nMobilePhone: %s\nNote: %s",
          this->name.c_str(), this->unit.c_str(), this->telephone.c_str(),
          this->mobilePhone.c_str(), this->note.c_str());
  return string(buf);
}

void DataBean::printDataBean() {
  qDebug("%s\n", this->getFormatedInfo().c_str());
}

void DataBean::outputToFile(FILE* file) {
  fprintf(file, "%s %s %s %s %s\n", this->name.c_str(), this->unit.c_str(),
          this->telephone.c_str(), this->mobilePhone.c_str(),
          this->note.c_str());
}

void DataBean::inputFromFile(FILE* file) {
  char name_buf[BUFFER_SIZE];
  char unit_buf[BUFFER_SIZE];
  char telephone_buf[BUFFER_SIZE];
  char mobilePhone_buf[BUFFER_SIZE];
  char note_buf[BUFFER_SIZE];

  while (fscanf(file, "%s %s %s %s %s\n", &name_buf, &unit_buf, &telephone_buf,
                &mobilePhone_buf, &note_buf) != EOF) {
    SystemInterface::getInstance().getDataBeans().push_back(
        DataBean(string(name_buf), string(unit_buf), string(telephone_buf),
                 string(mobilePhone_buf), string(note_buf)));
  }
}
