#include <list>
#include <iterator>
#include <algorithm>
#include <numeric>
#include "DataBean.h"
#include "SystemInterface.h"
#include <QDebug>
using namespace std;



// Singleton
SystemInterface & SystemInterface::getInstance() {
        static SystemInterface instance;
        return instance;
    }

list<DataBean> & SystemInterface::getDataBeans() {
        return this->dataBeans;
    }

void SystemInterface::addDataBean(DataBean dataBean) {
        this->dataBeans.push_back(dataBean);

        // Save To Disk.
        this->saveDataBeansToDisk();
    }

void SystemInterface::deleteDataBean(DataBean dataBean) {
        for (list<DataBean>::iterator iter = this->dataBeans.begin(); iter != this->dataBeans.end(); iter++) {
            if (!iter->name.compare(dataBean.name)) {
                this->dataBeans.erase(iter);
                // Save To Disk.
                this->saveDataBeansToDisk();
                return;
            }
         }

    }

void SystemInterface::modifyDataBean(DataBean dataBean) {
        // Find DataBean By ID.
        list<DataBean>::iterator iter = searchDataBean(dataBean);
        qDebug("联系人的旧数据: %s\n", iter->getFormatedInfo().c_str());

        // Modify Values.
        iter->name = dataBean.name;
        iter->unit = dataBean.unit;
        iter->telephone = dataBean.telephone;
        iter->mobilePhone = dataBean.mobilePhone;
        iter->note = dataBean.note;
        qDebug("修改完成, 新数据: %s\n", iter->getFormatedInfo().c_str());

        // Save To Disk.
        this->saveDataBeansToDisk();
    }


    list<DataBean>::iterator SystemInterface::searchDataBean(DataBean dataBean) {
        list<DataBean>::iterator iter = find(dataBeans.begin(), dataBeans.end(), dataBean);
        return iter;
    }


    bool SystemInterface::existDataBean(DataBean dataBean) {
        return SystemInterface::getInstance().searchDataBean(dataBean) != SystemInterface::getInstance().getDataBeans().end();
    }

    void SystemInterface::saveDataBeansToDisk() {

        FILE * file = fopen("DataBeans.dat", "w+");

        if(file == NULL) {
            printf("Cannot Open This File\n");
            return;
        }

        for(DataBean dataBean : dataBeans) {
            fprintf(file, "%s %s %s %s %s\n", dataBean.name.c_str(), dataBean.unit.c_str(), dataBean.telephone.c_str(), dataBean.mobilePhone.c_str(), dataBean.note.c_str());
        }

        fclose(file);
    }


    void SystemInterface::loadDataBeansFromDisk(bool clearDetaBeansInMemory, string fileName) {

        // Clear DataBeans in Memory.
        if (clearDetaBeansInMemory) this->dataBeans.clear();


        // Read DataBeans from Disk.
        FILE * file = fopen(fileName.c_str(), "r");

        if(file == NULL) {
            printf("Cannot Open This File\n");
            return;
        }

        // Dim DataBean Attributes.
        char name_buf[BUFFER_SIZE];
        char unit_buf[BUFFER_SIZE];
        char telephone_buf[BUFFER_SIZE];
        char mobilePhone_buf[BUFFER_SIZE];
        char note_buf[BUFFER_SIZE];

        while (fscanf(file, "%s %s %s %s %s\n", &name_buf, &unit_buf, &telephone_buf, &mobilePhone_buf, &note_buf) != EOF) {
            this->dataBeans.push_back(DataBean(string(name_buf), string(unit_buf), string(telephone_buf), string(mobilePhone_buf),  string(note_buf)));
        }

        fclose(file);
    }

     void SystemInterface::loadDataBeansFromDisk() {
         SystemInterface::loadDataBeansFromDisk(true, "DataBeans.dat");
     }

     void SystemInterface::listDataBeans() {

     }

     template<typename _Predicate>
     list<DataBean *> SystemInterface::searchDataBeans(_Predicate predicator) {
         // Do Search.
         list<DataBean>::iterator curIter = SystemInterface::getInstance().getDataBeans().begin();
         list<DataBean>::iterator endIter = SystemInterface::getInstance().getDataBeans().end();
         list<DataBean *> searchedDataBeans;

         while (true) {
             curIter = find_if(curIter,
                     endIter, predicator);
            if (curIter != endIter) {
                searchedDataBeans.push_back(&(*curIter));
            }

            if (curIter == endIter || ++curIter == endIter)  break;
         }

        return searchedDataBeans;
     }
