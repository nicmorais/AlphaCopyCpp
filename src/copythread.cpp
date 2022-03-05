#include "copythread.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <filesystem>

namespace fs = std::filesystem;

CopyThread::CopyThread() : QThread()
{
    signal = new CopySignals;
    isRunning = true;
}

void CopyThread::run()
{
    copy(src, dest);
    Q_EMIT signal->copyFinished();
}

// By ssendeavour - https://gist.github.com/ssendeavour/7324701
// Modifications by @nicmorais
bool CopyThread::copy(QString srcFilePath, QString tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    fs::copy_options options = fs::copy_options::overwrite_existing | fs::copy_options::update_existing;

    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();

        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName())) {
            return false;
        }

        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        foreach (const QString& fileName, fileNames) {
            const QString newSrcFilePath
                = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                = tgtFilePath + QLatin1Char('/') + fileName;

            if (!isRunning) {
                Q_EMIT signal->copyInterrupted();
                return false;
            } else {
                //fs::copy(newSrcFilePath.toStdString(), newTgtFilePath.toStdString());
                copy(newSrcFilePath, newTgtFilePath);
            }
        }
    } else {
        fs::copy(srcFilePath.toStdString(), tgtFilePath.toStdString(), options);
    }

    return true;
}

void CopyThread::quit()
{
    isRunning = false;
}
