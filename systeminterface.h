#pragma once
#include "DataBean.h"
#include <list>
#include <string>
using namespace std;
class SystemInterface {
private:

  const string DATABEANS_FILE_NAME = "DataBeans.dat";
  list<DataBean>dataBeans;

  // Singleton

public:

  static SystemInterface& getInstance();

public:

  list<DataBean>& getDataBeans();

public:

  void addDataBean(DataBean& dataBean);

public:

  void deleteDataBean(DataBean& dataBean);

public:

  void modifyDataBean(DataBean& dataBean);

public:

  list<DataBean>::iterator searchDataBean(DataBean& dataBean);

public:

  bool existDataBean(DataBean& dataBean);

public:

  bool saveDataBeansToDisk();

public:

  bool loadDataBeansFromDisk();

public:

  bool loadDataBeansFromDisk(bool   clearDetaBeansInMemory,
                             string fileName);

public:

  template<typename _Predicate>
  list<list<DataBean>::iterator>searchDataBeans(_Predicate pridicator);

public:

  static string filter(string raw);
};
