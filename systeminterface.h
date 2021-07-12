#pragma once
#include "DataBean.h"
#include <list>
#include <string>
using namespace std;
class SystemInterface {

private:
    list<DataBean> dataBeans;

// Singleton
public:
    static SystemInterface & getInstance();
public:
    list<DataBean> & getDataBeans();

public:
    void addDataBean(DataBean dataBean);

public:
    void deleteDataBean(DataBean dataBean);

public:
    void modifyDataBean(DataBean dataBean);

public:
    list<DataBean>::iterator searchDataBean(DataBean dataBean);

public:
    bool existDataBean(DataBean dataBean);

public:
    void saveDataBeansToDisk();

public:
    void loadDataBeansFromDisk();

public:
    void loadDataBeansFromDisk(bool clearDetaBeansInMemory, string fileName);

public:
    void listDataBeans();

public:
    template<typename _Predicate>
    list<DataBean *> searchDataBeans(_Predicate pridicator);


};

