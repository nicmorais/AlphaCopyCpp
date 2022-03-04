#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <QString>
#include <QObject>
#include <QRunnable>
#include "copysignals.h"

class CopyThread : public QThread
{
public:
  CopyThread();
  QString src;
  QString dest;
  CopySignals* signal;
  void run();

  bool copy(QString srcFilePath, QString tgtFilePath);

  void quit();

private:
  bool isRunning;
};

#endif // COPYTHREAD_H
