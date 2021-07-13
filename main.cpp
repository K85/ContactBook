#include <stdio.h>

#include <QApplication>
#include <QDebug>
#include <QFile>

#include "LevenshteinAlgorithm.h"
#include "SystemInterface.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  // Write
  FILE *file = fopen("test.dat", "w+");
  if (file == NULL) {
    printf("Cannot Open This File\n");
    return false;
  }
  DataBean dataBean1("zhangsan", "unit1", "telephone1", "moiblePhone1",
                     "note1");
  DataBean dataBean2("lisi", "unit2", "telephone2", "moiblePhone2", "note2");
  fwrite(&dataBean1, sizeof(DataBean), 1, file);

  fseek(file, sizeof(DataBean), SEEK_CUR);
  fwrite(&dataBean2, sizeof(DataBean), 1, file);
  fclose(file);

  // Read
  file = fopen("test.dat", "r");
  if (file == NULL) {
    printf("Cannot Open This File\n");
    return false;
  }

  qDebug("=====Print Load Objects=====");
  DataBean tempDataBean;
  fread(&tempDataBean, sizeof(DataBean), 1, file);
  tempDataBean.printDataBean();

  fseek(file, sizeof(DataBean), SEEK_CUR);

  fread(&tempDataBean, sizeof(DataBean), 1, file);
  tempDataBean.printDataBean();

  fclose(file);

  return 0;

  // Initialize SystemInstance.
  SystemInterface::getInstance().loadDataBeansFromDisk();
  SystemInterface::getInstance().getDataBeans().sort();

  /* Run Qt Application. */

  // High-DPI Support.
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);
  QFile qssFile(":/dev/qss/light.qss");

  if (!qssFile.open(QFile::ReadOnly)) {
    qDebug("Fail to Open QSS File.");
  }
  qApp->setStyleSheet(qssFile.readAll());
  qssFile.close();
  MainWindow w;

  w.show();
  return a.exec();
}
