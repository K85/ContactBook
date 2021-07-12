#include <stdio.h>

#include <QApplication>
#include <QDebug>
#include <QFile>

#include "LevenshteinAlgorithm.h"
#include "SystemInterface.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  // Initialize SystemInstance.
  SystemInterface::getInstance().loadDataBeansFromDisk();
  SystemInterface::getInstance().getDataBeans().sort();

  /* Run Qt Application. */

  // High-DPI Support.
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);
  QFile qssFile(":/qss/light.qss");

  if (!qssFile.open(QFile::ReadOnly)) {
    qDebug("Fail to Open QSS File.");
  }
  qApp->setStyleSheet(qssFile.readAll());
  qssFile.close();
  MainWindow w;

  w.show();
  return a.exec();
}
