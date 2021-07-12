#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include "DataBean.cpp"
#include "SystemInterface.cpp"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include "LevenshteinAlgotithm.h"

#define TEXT_SIMILARITY_THRESHOLD 0.3

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(this->width(),this->height());

    // Redraw ContactList.
    on_pushButton_ListContact_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_AddContact_clicked()
{

    // Name Already Exist ?
    string name = stringFilter(ui->lineEdit_operateContact_Name->text().toStdString());
    if (name.empty()) {
        QMessageBox::warning(NULL, NULL, "Contact Name Can't be Empty!");
        return;
    }

    if (SystemInterface::getInstance().existDataBean(DataBean(name))) {
        QMessageBox::warning(NULL, NULL, "Contact Name Already Exist!");
        return;
    }

    /* Construct DataBean. */
    string unit = stringFilter(ui->lineEdit_operateContact_Unit->text().toStdString());
    string telephone = stringFilter(ui->lineEdit_operateContact_Telephone->text().toStdString());
    string mobilePhone = stringFilter(ui->lineEdit_operateContact_MobilePhone->text().toStdString());
    string note = stringFilter(ui->lineEdit_operateContact_Note->text().toStdString());
    // Check Empty.
    if (unit.empty()) unit.assign("EmptyUnit");
    if (telephone.empty()) telephone.assign("EmptyTelephone");
    if (mobilePhone.empty()) mobilePhone.assign("EmptyMobilePhone");
    if (note.empty()) note.assign("EmptyNote");
    qDebug("Construct DataBean: name = %s, unit = %s, telephone = %s, mobilePhone = %s, note = %s", name.c_str(), unit.c_str(), telephone.c_str(), mobilePhone.c_str(), note.c_str());
    DataBean dataBean = DataBean(name, unit, telephone, mobilePhone, note);

    /* Add DataBean to SystemInterface. */
    SystemInterface::getInstance().addDataBean(dataBean);
    SystemInterface::getInstance().listDataBeans();
    // Resort Contacts.
    SystemInterface::getInstance().getDataBeans().sort();

    // Redraw and Reselect.
    this->on_pushButton_ListContact_clicked();
    ui->listWidget_databeans->setCurrentItem(ui->listWidget_databeans->findItems(QString(name.c_str()), Qt::MatchExactly).first());
}

void MainWindow::on_pushButton_ListContact_clicked()
{
    // Clear Old Contacts.
    ui->listWidget_databeans->clear();

    // Get All Contacts from SystemInterface.
    list<DataBean> dataBeans = SystemInterface::getInstance().getDataBeans();
    for (DataBean dataBean : dataBeans) {
        ui->listWidget_databeans->addItem(QString(dataBean.name.c_str()));
    }

    ui->label_ContactBook->setText("Contact Book: ");
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    ui->label_ContactBook->setPalette(pe);
}

void MainWindow::on_pushButton_DeleteContact_clicked()
{

    // Check Empty.
    if (!ui->listWidget_databeans->currentItem()->isSelected()) {
        QMessageBox::critical(NULL, NULL, "Select A Contact First.");
        return;
    }

    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(NULL, NULL, "Sure to delete it?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::No) {
        return;
    }


    // Get Selected Contact Name.
    string selectedContactName = ui->listWidget_databeans->currentItem()->text().toStdString();
    qDebug("Selected Contact Name: %s", selectedContactName.c_str());

    // Delete ListItem.
    ui->listWidget_databeans->takeItem(ui->listWidget_databeans->currentRow());

    // Delete DataBean.
    SystemInterface::getInstance().deleteDataBean(selectedContactName);

}

void MainWindow::on_listWidget_databeans_currentTextChanged(const QString &currentText)
{

    if (currentText.isEmpty() || currentText.isNull()) {
        return;
    }

    // Get New Value.
    string newSelectedContactName = currentText.toStdString();

    // Show ContactName.
    showContactInfo(newSelectedContactName);
}

void MainWindow::on_pushButton_ModifyContact_clicked()
{

    // Get Selected ListItem.
    string name = ui->listWidget_databeans->currentItem()->text().toStdString();
    string unit = ui->lineEdit_operateContact_Unit->text().toStdString();
    string telephone = ui->lineEdit_operateContact_Telephone->text().toStdString();
    string mobilePhone = ui->lineEdit_operateContact_MobilePhone->text().toStdString();
    string note = ui->lineEdit_operateContact_Note->text().toStdString();

    // Construct DataBean.
    DataBean dataBean = DataBean(name, unit, telephone, mobilePhone, note);
    SystemInterface::getInstance().modifyDataBean(dataBean);
    showContactInfo(name);
}


void MainWindow::showContactInfo(string contactName) {
    auto iter = SystemInterface::getInstance().searchDataBean(contactName);
    qDebug("ContactList item has been changed to: \n%s", iter->getFormatedInfo().c_str());

    // Redraw Contact Info.
    ui->label_contact_info->setText(QString(iter->getFormatedInfo().c_str()));

    // Redraw Contact Operator.
    ui->lineEdit_operateContact_Name->setText(QString(iter->name.c_str()));
    ui->lineEdit_operateContact_Unit->setText(QString(iter->unit.c_str()));
    ui->lineEdit_operateContact_Telephone->setText(QString(iter->telephone.c_str()));
    ui->lineEdit_operateContact_MobilePhone->setText(QString(iter->mobilePhone.c_str()));
    ui->lineEdit_operateContact_Note->setText(QString(iter->note.c_str()));
}




void MainWindow::on_pushButton_ImportContact_clicked()
{
    // Enter DataBeansFile Name.
    bool isOK;
    QString text = QInputDialog::getText(NULL, NULL, "Please input databeans file name.",
                                         QLineEdit::Normal, NULL, &isOK);
    if (!isOK || text.isEmpty()) {
        return;
    }

    // Load Databeans from Disk.
    SystemInterface::getInstance().loadDataBeansFromDisk(false, text.toStdString());
    SystemInterface::getInstance().saveDataBeansToDisk();

    QMessageBox::information(NULL, NULL, "Import Successfully.");

    // Redraw and Reselect.
    this->on_pushButton_ListContact_clicked();
}


void MainWindow::on_pushButton_SearchContact_clicked()
{

    // Chosse Primary Key for Search.
    QStringList items;
    items << QString("By Name") << QString("By Unit") << QString("By MobilePhone") << QString("By Note");
    bool OK;
    QString item = QInputDialog::getItem(this, NULL,
                                         "Choose Primary Key for Search.", items, 0, false, &OK);
    if (!OK || item.isEmpty()) {
        return;
    }

    // Input the value of Primary Key.
    QString searchValue = QInputDialog::getText(NULL, NULL, "Please input primary key.",
                                         QLineEdit::Normal, NULL, &OK);
    if (!OK || searchValue.isEmpty()) {
        return;
    }

    /* Do Search. */
    list<DataBean *> searchedDataBeans;

    // Filter: By Name.
    if (!item.toStdString().compare("By Name")) {
        searchedDataBeans = SystemInterface::getInstance().searchDataBeans([searchValue](const DataBean& dataBean){
            return LevenshteinAlgotithm::compare(searchValue.toStdString(), dataBean.name) >= TEXT_SIMILARITY_THRESHOLD;
        });
    }

    // Filter: By Unit.
    if (!item.toStdString().compare("By Unit")) {
        searchedDataBeans = SystemInterface::getInstance().searchDataBeans([searchValue](const DataBean& dataBean){
            return LevenshteinAlgotithm::compare(searchValue.toStdString(), dataBean.unit) >= TEXT_SIMILARITY_THRESHOLD;
        });
    }

    // Filter: By MobilePhone.
    if (!item.toStdString().compare("By MobilePhone")) {
        searchedDataBeans = SystemInterface::getInstance().searchDataBeans([searchValue](const DataBean& dataBean){
            return LevenshteinAlgotithm::compare(searchValue.toStdString(), dataBean.mobilePhone) >= TEXT_SIMILARITY_THRESHOLD;
        });
    }

    // Filter: By Note.
    if (!item.toStdString().compare("By Note")) {
        searchedDataBeans = SystemInterface::getInstance().searchDataBeans([searchValue](const DataBean& dataBean){
            return LevenshteinAlgotithm::compare(searchValue.toStdString(), dataBean.note) >= TEXT_SIMILARITY_THRESHOLD;
        });
    }
    qDebug("the size of SearchedDataBeans: %d", searchedDataBeans.size());

    /* Redraw List. */
    ui->listWidget_databeans->clear();
    for (DataBean * dataBeanPtr : searchedDataBeans) {
        ui->listWidget_databeans->addItem(QString(dataBeanPtr->name.c_str()));
    }
    ui->label_ContactBook->setText("Search Results: ");
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_ContactBook->setPalette(pe);



}

void MainWindow::on_lineEdit_operateContact_Name_textChanged(const QString &arg1)
{
    // RealTime Check.
    ui->pushButton_AddContact->setDisabled(SystemInterface::getInstance().existDataBean(DataBean(arg1.toStdString())));
}

string MainWindow::stringFilter(string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}
