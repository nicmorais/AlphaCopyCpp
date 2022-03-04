#ifndef COPYCHECKTHREAD_H
#define COPYCHECKTHREAD_H

#include <QObject>
#include <QThread>
#include <iostream>
#include "copychecksignals.h"

class CopyCheckThread : public QThread
{

    public:
        CopyCheckThread();
        CopyCheckSignals* signal;

        QString dest;

        void run();

        int getFolderDiskUsage(std::string path);

      public Q_SLOTS:
        void quit();
      private:
        bool isRunning;
};

#endif // COPYCHECKTHREAD_H
