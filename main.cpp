#include "SystemInterface.h"
#include "LevenshteinAlgotithm.h"
#include "mainwindow.h"
#include <stdio.h>
#include <QApplication>
#include <QFile>
#include <QDebug>


int main(int argc, char *argv[])
{
  // Call SystemInstance.
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
