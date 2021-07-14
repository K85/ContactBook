#include "SystemInterface.h"

#include <algorithm>
#include <iterator>
#include <list>

#include "DataBean.h"
using namespace std;

// Singleton
SystemInterface &SystemInterface::getInstance() {
  static SystemInterface instance;
  return instance;
}

list<DataBean> &SystemInterface::getDataBeans() { return this->dataBeans; }

void SystemInterface::addDataBean(DataBean &dataBean) {
  this->dataBeans.push_back(dataBean);

  // Save To Disk.
  this->saveDataBeansToDisk();
}

void SystemInterface::deleteDataBean(DataBean &dataBean) {
  for (list<DataBean>::iterator iter = this->dataBeans.begin();
       iter != this->dataBeans.end(); iter++) {
    // Use Operator ==
    if (*iter == dataBean) {
      this->dataBeans.erase(iter);
      return;
    }
  }

  // Save To Disk.
  this->saveDataBeansToDisk();
}

void SystemInterface::modifyDataBean(DataBean &dataBean) {
  // Find DataBean By PrimaryKey.
  list<DataBean>::iterator iter = searchDataBean(dataBean);

  // Modify DataBean (Using Assignment Constructor).
  *iter = dataBean;

  // Save To Disk.
  this->saveDataBeansToDisk();
}

list<DataBean>::iterator SystemInterface::searchDataBean(DataBean &dataBean) {
  list<DataBean>::iterator iter =
      find(dataBeans.begin(), dataBeans.end(), dataBean);
  return iter;
}

bool SystemInterface::existDataBean(DataBean &dataBean) {
  return SystemInterface::getInstance().searchDataBean(dataBean) !=
         SystemInterface::getInstance().getDataBeans().end();
}

bool SystemInterface::saveDataBeansToDisk() {
  FILE *file = fopen(SystemInterface::DATABEANS_FILE_NAME.c_str(), "w");

  if (file == NULL) {
    printf("Cannot Open This File\n");
    return false;
  }

  for (DataBean dataBean : dataBeans) {
    dataBean.outputToFile(file);
  }

  fclose(file);
  return true;
}

bool SystemInterface::loadDataBeansFromDisk(bool clearDetaBeansInMemory,
                                            string fileName) {
  // Clear DataBeans in Memory.
  if (clearDetaBeansInMemory) this->dataBeans.clear();

  // Read DataBeans from Disk.
  FILE *file = fopen(fileName.c_str(), "r");

  if (file == NULL) {
    printf("Cannot Open This File\n");
    return false;
  }

  /* Read DataBean Attributes. */
  DataBean::inputFromFile(file);
  fclose(file);
  return true;
}

bool SystemInterface::loadDataBeansFromDisk() {
  return SystemInterface::loadDataBeansFromDisk(
      true, SystemInterface::DATABEANS_FILE_NAME.c_str());
}

template <typename _Predicate>
list<list<DataBean>::iterator> SystemInterface::searchDataBeans(
    _Predicate predicate) {
  // Do Search.
  list<DataBean>::iterator curIter =
      SystemInterface::getInstance().getDataBeans().begin();
  list<DataBean>::iterator endIter =
      SystemInterface::getInstance().getDataBeans().end();
  list<list<DataBean>::iterator> searchedDataBeans;

  while (true) {
    curIter = find_if(curIter, endIter, predicate);

    if (curIter != endIter) {
      searchedDataBeans.push_back(curIter);
    }

    if ((curIter == endIter) || (++curIter == endIter)) break;
  }

  return searchedDataBeans;
}

string SystemInterface::filter(string raw) {
  raw = SystemInterface::trim(raw);
  for (char &ch : raw) {
    if ((ch == ' ') || (ch == '\n') || (ch == '\t')) ch = '_';
  }
  return raw;
}

bool SystemInterface::matchRegex(string text, string reg) {
  return std::regex_match(text, regex(reg));
}

string &SystemInterface::trim(string &str) {
  if (str.empty()) {
    return str;
  }

  str.erase(0, str.find_first_not_of(" "));
  str.erase(str.find_last_not_of(" ") + 1);
  return str;
}
