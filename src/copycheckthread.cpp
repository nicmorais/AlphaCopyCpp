#include "copycheckthread.h"
#include<QDebug>
#include <QThread>
#include <math.h>
#include <QStorageInfo>

CopyCheckThread::CopyCheckThread() : QThread()
{
    signal = new CopyCheckSignals(nullptr);
    isRunning = true;
}

void CopyCheckThread::run()
{
    int size = 0;

    while (isRunning && size < srcSize) {
        size = getFolderDiskUsage(dest.toStdString());
        Q_EMIT signal->dataCopied(size);
        QThread::sleep(1);
    }
}

int CopyCheckThread::getFolderDiskUsage(std::string path)
{
    // command to be executed
    std::string cmd("du -sb ");
    cmd.append(path);
    cmd.append(" | cut -f1 2>&1");
    // execute above command and get the output
    FILE* stream = popen(cmd.c_str(), "r");

    if (stream) {
        const int max_size = 256;
        char readbuf[max_size];

        if (fgets(readbuf, max_size, stream) != NULL) {
            int size =  atoll(readbuf) / pow(2, 10);
            return size;
        }

        pclose(stream);
    }

    // return error val
    return -1;
}

void CopyCheckThread::quit()
{
    isRunning = false;
}
