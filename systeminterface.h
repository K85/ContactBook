#pragma once
#include <list>
#include <regex>
#include <string>

#include "DataBean.h"
using namespace std;
class SystemInterface {
 private:
  const string DATABEANS_FILE_NAME = "DataBeans.dat";
  list<DataBean> dataBeans;

 public:
  // Singleton
  static SystemInterface& getInstance();

  list<DataBean>& getDataBeans();

  void addDataBean(DataBean& dataBean);

  void deleteDataBean(DataBean& dataBean);

  void modifyDataBean(DataBean& dataBean);

  list<DataBean>::iterator searchDataBean(DataBean& dataBean);

  bool existDataBean(DataBean& dataBean);

  bool saveDataBeansToDisk();

  bool loadDataBeansFromDisk();

  bool loadDataBeansFromDisk(bool clearDetaBeansInMemory, string fileName);

  template <typename _Predicate>
  list<list<DataBean>::iterator> searchDataBeans(_Predicate predicate);

  static string filter(string raw);

  static string& trim(string& str);

  static bool matchRegex(string text, string reg);
};
