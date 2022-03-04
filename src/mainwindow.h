#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "copycheckthread.h"
#include "copythread.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* ui;
        QString sdCardPath;
        QString hddPath;

        int getFolderDiskUsage(std::string path);

        float getDiskSize(QString path);

        bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);

        CopyThread* copyThread;

        CopyCheckThread* copyCheck;

    private Q_SLOTS:
        void scan();

        void copy();

        void eject();

        void done();

        void abortCopy();

        void setCopyProgressBar(int value);

        void setCopyProgressBarStylesheet();
};
#endif // MAINWINDOW_H
