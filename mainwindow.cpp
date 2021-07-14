#include "mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <string>

#include "DataBean.h"
#include "LevenshteinAlgorithm.h"
#include "SystemInterface.cpp"
#include "ui_mainwindow.h"

#define TEXT_SIMILARITY_THRESHOLD 0.2

static const string STRING_EMPTY = "Empty";
using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setFixedSize(this->width(), this->height());

  // Redraw ContactList.
  ui->pushButton_ModifyContact->setDisabled(true);
  ui->pushButton_ListContact->setVisible(false);
  on_pushButton_ListContact_clicked();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_AddContact_clicked() {
  /* Name Already Exist ? */
  string name = SystemInterface::filter(
      ui->lineEdit_operateContact_Name->text().toStdString());

  /* Construct DataBean. */
  string unit = SystemInterface::filter(
      ui->lineEdit_operateContact_Unit->text().toStdString());
  string telephone = SystemInterface::filter(
      ui->lineEdit_operateContact_Telephone->text().toStdString());
  string mobilePhone = SystemInterface::filter(
      ui->lineEdit_operateContact_MobilePhone->text().toStdString());
  string note = SystemInterface::filter(
      ui->lineEdit_operateContact_Note->text().toStdString());

  // Validate
  if (!validateDataBean(false, name, unit, telephone, mobilePhone, note))
    return;

  // Check Empty.
  if (unit.empty()) unit.assign(STRING_EMPTY);
  if (telephone.empty()) telephone.assign(STRING_EMPTY);
  if (mobilePhone.empty()) mobilePhone.assign(STRING_EMPTY);
  if (note.empty()) note.assign(STRING_EMPTY);
  DataBean dataBean(name, unit, telephone, mobilePhone, note);

  /* Add DataBean to SystemInterface. */
  SystemInterface::getInstance().addDataBean(dataBean);

  // Resort Contacts.
  SystemInterface::getInstance().getDataBeans().sort();

  /* Redraw and Reselect. */
  this->on_pushButton_ListContact_clicked();
  ui->listWidget_databeans->setCurrentItem(
      ui->listWidget_databeans
          ->findItems(QString(name.c_str()), Qt::MatchExactly)
          .first());
  ui->pushButton_AddContact->setDisabled(true);
}

void MainWindow::listContacts() {
  // Clear Old Contacts.
  ui->listWidget_databeans->clear();

  // Get All Contacts from SystemInterface.
  list<DataBean> dataBeans = SystemInterface::getInstance().getDataBeans();

  for (DataBean dataBean : dataBeans) {
    ui->listWidget_databeans->addItem(QString(dataBean.name.c_str()));
  }
}

void MainWindow::on_pushButton_ListContact_clicked() {
  /* List Constacts. */
  listContacts();

  /* Redraw. */

  // Set Label.
  ui->label_ContactBook->setText("Contact Book: ");
  QPalette pe;
  pe.setColor(QPalette::WindowText, Qt::black);
  ui->label_ContactBook->setPalette(pe);

  // Set Back Button.
  ui->pushButton_ListContact->setVisible(false);
}

void MainWindow::on_pushButton_DeleteContact_clicked() {
  // Check Empty.
  if (ui->listWidget_databeans->currentItem() == NULL) {
    QMessageBox::critical(this, NULL, "Select A Contact First.");
    return;
  }

  QMessageBox::StandardButton btn;

  btn = QMessageBox::question(this, NULL, "Sure to delete it?",
                              QMessageBox::Yes | QMessageBox::No);

  if (btn == QMessageBox::No) {
    return;
  }

  // Get Selected Contact Name.
  string selectedContactName =
      ui->listWidget_databeans->currentItem()->text().toStdString();

  qDebug("Selected Contact Name: %s", selectedContactName.c_str());

  // Delete ListItem.
  ui->listWidget_databeans->takeItem(ui->listWidget_databeans->currentRow());

  // Delete DataBean.
  DataBean dataBean(selectedContactName);

  SystemInterface::getInstance().deleteDataBean(dataBean);
}

void MainWindow::on_listWidget_databeans_currentTextChanged(
    const QString& currentText) {
  // Check Empty.
  if (currentText.isEmpty() || currentText.isNull()) {
    return;
  }

  // Show ContactName.
  showContactInfo(currentText.toStdString());
}

void MainWindow::on_pushButton_ModifyContact_clicked() {
  // Check Empty.
  if (ui->listWidget_databeans->currentItem() == NULL) {
    return;
  }

  // Get Selected ListItem.
  string oldName =
      ui->listWidget_databeans->currentItem()->text().toStdString();
  DataBean dataBean(oldName);
  list<DataBean>::iterator iter =
      SystemInterface::getInstance().searchDataBean(dataBean);

  /* Construct DataBean. */
  string newName = SystemInterface::filter(
      ui->lineEdit_operateContact_Name->text().toStdString());
  string unit = SystemInterface::filter(
      ui->lineEdit_operateContact_Unit->text().toStdString());
  string telephone = SystemInterface::filter(
      ui->lineEdit_operateContact_Telephone->text().toStdString());
  string mobilePhone = SystemInterface::filter(
      ui->lineEdit_operateContact_MobilePhone->text().toStdString());
  string note = SystemInterface::filter(
      ui->lineEdit_operateContact_Note->text().toStdString());

  // Validate
  if (!validateDataBean(true, newName, unit, telephone, mobilePhone, note))
    return;

  // Check Empty.
  if (unit.empty()) unit.assign(STRING_EMPTY);
  if (telephone.empty()) telephone.assign(STRING_EMPTY);
  if (mobilePhone.empty()) mobilePhone.assign(STRING_EMPTY);
  if (note.empty()) note.assign(STRING_EMPTY);

  dataBean = DataBean(newName, unit, telephone, mobilePhone, note);
  *iter = dataBean;
  SystemInterface::getInstance().saveDataBeansToDisk();

  /* Redraw and Reselect. */
  SystemInterface::getInstance().getDataBeans().sort();
  this->on_pushButton_ListContact_clicked();
  showContactInfo(newName);
  ui->listWidget_databeans->setCurrentItem(
      ui->listWidget_databeans
          ->findItems(QString(newName.c_str()), Qt::MatchExactly)
          .first());
  ui->pushButton_AddContact->setDisabled(true);
}

void MainWindow::showContactInfo(string contactName) {
  DataBean dataBean(contactName);
  auto iter = SystemInterface::getInstance().searchDataBean(dataBean);

  /* Redraw. */

  // Redraw Contact Info.
  ui->label_contact_info->setText(QString(iter->getFormatedInfo().c_str()));

  /* Redraw Contact Operator. */

  // Name.
  ui->lineEdit_operateContact_Name->setText(QString(iter->name.c_str()));
  if (!iter->unit.compare(STRING_EMPTY)) {
    ui->lineEdit_operateContact_Unit->setText(QString(""));
  } else {
    ui->lineEdit_operateContact_Unit->setText(QString(iter->unit.c_str()));
  }

  // Telephone
  if (!iter->telephone.compare(STRING_EMPTY)) {
    ui->lineEdit_operateContact_Telephone->setText(QString(""));
  } else {
    ui->lineEdit_operateContact_Telephone->setText(
        QString(iter->telephone.c_str()));
  }

  // MobilePhone
  if (!iter->mobilePhone.compare(STRING_EMPTY)) {
    ui->lineEdit_operateContact_MobilePhone->setText(QString(""));
  } else {
    ui->lineEdit_operateContact_MobilePhone->setText(
        QString(iter->mobilePhone.c_str()));
  }

  // Note.
  if (!iter->note.compare(STRING_EMPTY)) {
    ui->lineEdit_operateContact_Note->setText(QString(""));
  } else {
    ui->lineEdit_operateContact_Note->setText(QString(iter->note.c_str()));
  }

  // Set ModifyButton.
  ui->pushButton_ModifyContact->setDisabled(true);
}

void MainWindow::on_pushButton_SearchContact_clicked() {
  // Chosse PrimaryKeyType for Search.
  QStringList searchPrimaryKeyTypes;

  searchPrimaryKeyTypes << "By Name"
                        << "By Unit"
                        << "By Telephone"
                        << "By MobilePhone"
                        << "By Note";
  bool isOK;
  QString searchPrimaryKeyType =
      QInputDialog::getItem(this, NULL, "Choose Primary Key for Search.",
                            searchPrimaryKeyTypes, 0, false, &isOK);

  if (!isOK || searchPrimaryKeyType.isEmpty()) {
    return;
  }

  // Input the value of PrimaryKey.
  QString searchValue = QInputDialog::getText(
      this, NULL, "Please input primary key.", QLineEdit::Normal, NULL, &isOK);

  if (!isOK || searchValue.isEmpty()) {
    return;
  }

  /* Do Search. */
  list<list<DataBean>::iterator> searchedDataBeans;

  // Filter: By Name.
  if (!searchPrimaryKeyType.toStdString().compare("By Name")) {
    searchedDataBeans = SystemInterface::getInstance().searchDataBeans(
        [searchValue](const DataBean& dataBean) {
          return LevenshteinAlgorithm::compare(searchValue.toStdString(),
                                               dataBean.name) >=
                 TEXT_SIMILARITY_THRESHOLD;
        });
  }

  // Filter: By Unit.
  if (!searchPrimaryKeyType.toStdString().compare("By Unit")) {
    searchedDataBeans = SystemInterface::getInstance().searchDataBeans(
        [searchValue](const DataBean& dataBean) {
          return LevenshteinAlgorithm::compare(searchValue.toStdString(),
                                               dataBean.unit) >=
                 TEXT_SIMILARITY_THRESHOLD;
        });
  }

  // Filter: By Telephone.
  if (!searchPrimaryKeyType.toStdString().compare("By Telephone")) {
    searchedDataBeans = SystemInterface::getInstance().searchDataBeans(
        [searchValue](const DataBean& dataBean) {
          return LevenshteinAlgorithm::compare(searchValue.toStdString(),
                                               dataBean.telephone) >=
                 TEXT_SIMILARITY_THRESHOLD;
        });
  }

  // Filter: By MobilePhone.
  if (!searchPrimaryKeyType.toStdString().compare("By MobilePhone")) {
    searchedDataBeans = SystemInterface::getInstance().searchDataBeans(
        [searchValue](const DataBean& dataBean) {
          return LevenshteinAlgorithm::compare(searchValue.toStdString(),
                                               dataBean.mobilePhone) >=
                 TEXT_SIMILARITY_THRESHOLD;
        });
  }

  // Filter: By Note.
  if (!searchPrimaryKeyType.toStdString().compare("By Note")) {
    searchedDataBeans = SystemInterface::getInstance().searchDataBeans(
        [searchValue](const DataBean& dataBean) {
          return LevenshteinAlgorithm::compare(searchValue.toStdString(),
                                               dataBean.note) >=
                 TEXT_SIMILARITY_THRESHOLD;
        });
  }
  qDebug("the size of SearchedDataBeans: %zu", searchedDataBeans.size());

  /* Redraw. */
  // Redraw List.
  ui->listWidget_databeans->clear();

  for (list<DataBean>::iterator dataBeanPtr : searchedDataBeans) {
    ui->listWidget_databeans->addItem(QString(dataBeanPtr->name.c_str()));
  }

  // Set Label.
  ui->label_ContactBook->setText("Search Results: ");
  QPalette pe;

  pe.setColor(QPalette::WindowText, Qt::red);
  ui->label_ContactBook->setPalette(pe);

  // Set Back Button.
  ui->pushButton_ListContact->setVisible(true);
}

void MainWindow::on_lineEdit_operateContact_Name_textChanged(
    const QString& arg1) {
  // RealTime Check.
  DataBean dataBean(arg1.toStdString());

  if (SystemInterface::getInstance().existDataBean(dataBean)) {
    ui->pushButton_AddContact->setDisabled(true);
    showContactInfo(dataBean.name);
  } else {
    ui->pushButton_AddContact->setDisabled(false);
    // Set Other Attributes Empty.
    ui->lineEdit_operateContact_Unit->clear();
    ui->lineEdit_operateContact_Telephone->clear();
    ui->lineEdit_operateContact_MobilePhone->clear();
    ui->lineEdit_operateContact_Note->clear();

    // Toggle Modify Button.
    ui->pushButton_ModifyContact->setDisabled(true);
  }
}

void MainWindow::on_lineEdit_operateContact_Unit_textChanged(
    const QString& arg1) {
  // Toggle Modify Button.
  ui->pushButton_ModifyContact->setDisabled(false);
}

void MainWindow::on_lineEdit_operateContact_Telephone_textChanged(
    const QString& arg1) {
  // Toggle Modify Button.
  ui->pushButton_ModifyContact->setDisabled(false);
}

void MainWindow::on_lineEdit_operateContact_MobilePhone_textChanged(
    const QString& arg1) {
  // Toggle Modify Button.
  ui->pushButton_ModifyContact->setDisabled(false);
}

void MainWindow::on_lineEdit_operateContact_Note_textChanged(
    const QString& arg1) {
  // Toggle Modify Button.
  ui->pushButton_ModifyContact->setDisabled(false);
}

bool MainWindow::validateDataBean(bool ignoreExistName, const string& name,
                                  const string& unit, const string& telephone,
                                  const string& mobilePhone,
                                  const string& note) {
  /* Validate */

  // Check Length.
  if (name.size() > 30 || unit.size() > 50 || note.size() > 200) {
    QMessageBox::critical(this, NULL, "Values too Long.");
    return false;
  }

  // Validate Name.
  if (name.empty()) {
    QMessageBox::warning(this, NULL, "Contact Name Can't be Empty!");
    return false;
  }

  if (!ignoreExistName) {
    DataBean condition(name);
    if (SystemInterface::getInstance().existDataBean(condition)) {
      QMessageBox::warning(this, NULL, "Contact Name Already Exist.");
      return false;
    }
  }

  // Validate Telephone & MobilePhone.
  if ((telephone.empty() || !telephone.compare(STRING_EMPTY)) &&
      (mobilePhone.empty() || !mobilePhone.compare(STRING_EMPTY))) {
    QMessageBox::warning(this, NULL,
                         "Telephone and MobilePhone Can't Both be Empty.");
    return false;
  }

  if (!telephone.empty() && telephone.compare(STRING_EMPTY) &&
      !SystemInterface::matchRegex(
          telephone,
          "((\\d{11})|^((\\d{7,8})|(\\d{4}|\\d{3})-(\\d{7,8})|(\\d{4}|\\d{3})-("
          "\\d{7,8})-(\\d{4}|\\d{3}|\\d{2}|\\d{1})|(\\d{7,8})-(\\d{4}|\\d{3}|"
          "\\d{2}|\\d{1}))$)")) {
    QMessageBox::critical(this, NULL, "Wrong Telephone");
    return false;
  }

  if (!mobilePhone.empty() && mobilePhone.compare(STRING_EMPTY) &&
      !SystemInterface::matchRegex(mobilePhone,
                                   "^(13[0-9]|14[5|7]|15[0|1|2|3|4|5|6|7|8|9]|"
                                   "18[0|1|2|3|5|6|7|8|9])\\d{8}$")) {
    QMessageBox::critical(this, NULL, "Wrong MobilePhone");
    return false;
  }

  return true;
}

void MainWindow::on_pushButton_ImportContact_clicked() {
  // Enter Import DataBeansFile Name.
  bool isOK;
  QString text =
      QInputDialog::getText(this, NULL, "Please input databeans file name.",
                            QLineEdit::Normal, NULL, &isOK);

  if (!isOK || text.isEmpty()) {
    return;
  }

  // Load Databeans from Disk.
  bool flag = true;
  if (!SystemInterface::getInstance().loadDataBeansFromDisk(false,
                                                            text.toStdString()))
    flag = false;
  if (!SystemInterface::getInstance().saveDataBeansToDisk()) flag = false;

  if (flag) {
    QMessageBox::information(this, NULL, "Import Successfully.");
  } else {
    QMessageBox::critical(this, NULL,
                          "Import Failed. Is the file name correct?");
  }

  // Redraw.
  this->on_pushButton_ListContact_clicked();
}

void MainWindow::on_MainWindow_destroyed() {
  // Release Memory.
  SystemInterface::getInstance().getDataBeans().clear();
}
