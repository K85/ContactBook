#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:

  void on_pushButton_AddContact_clicked();

  void on_pushButton_ListContact_clicked();

  void on_pushButton_DeleteContact_clicked();

  void on_listWidget_databeans_currentTextChanged(const QString& currentText);

  void on_pushButton_ModifyContact_clicked();

  void showContactInfo(string contactName);

  void on_pushButton_SearchContact_clicked();

  void on_lineEdit_operateContact_Name_textChanged(const QString& arg1);

  void on_lineEdit_operateContact_Unit_textChanged(const QString& arg1);

  void on_lineEdit_operateContact_Telephone_textChanged(const QString& arg1);

  void on_lineEdit_operateContact_MobilePhone_textChanged(const QString& arg1);

  void on_lineEdit_operateContact_Note_textChanged(const QString& arg1);

  bool validateDataBean(bool ignoreExistName, const string& name,
                        const string& unit, const string& telephone,
                        const string& mobilePhone, const string& note);

  void listContacts();

  void on_pushButton_ImportContact_clicked();

  void on_MainWindow_destroyed();

 private:
  Ui::MainWindow* ui;
};
#endif  // MAINWINDOW_H
