#include "mainwindow.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QDirIterator>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <QStorageInfo>
#include <QThreadPool>
#include "copychecksignals.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap sdPixmap = QPixmap(":/icons/assets/devices/Sycamoreent-Storage-Sd.png");
    QPixmap hddPixmap = QPixmap(":/icons/assets/devices/usb-black.png");
    QPixmap logoLabel = QPixmap(":/icons/assets/logos/alphaletter.png");
    ui->sdIconLb->setPixmap(sdPixmap.scaledToHeight(64));
    ui->hddIconLb->setPixmap(hddPixmap.scaledToHeight(64));
    ui->logoLabel->setPixmap(logoLabel.scaledToHeight(64));
    connect(ui->copyProgrB, &QProgressBar::valueChanged, this, &MainWindow::setCopyProgressBarStylesheet);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getFolderDiskUsage(std::string path)
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
            return atoll(readbuf) / pow(2, 20);
        }

        pclose(stream);
    }

    // return error val
    return -1;
}

float MainWindow::getDiskSize(QString path)
{
    QStorageInfo storage;
    storage.setPath(path);
    return storage.bytesTotal() / pow(2, 20);
}

void MainWindow::scan()
{
    std::string basePath = "/media/" + qgetenv("USER").toStdString();
    QVector<std::string> disks;

    for (const auto& entry : std::filesystem::directory_iterator(basePath)) {
        // std::cout << entry.path() << std::endl;
        disks.append(entry.path());
    }

    if (disks.length() < 2) {
        ui->stackedWidget->setCurrentIndex(1);
        return;
    }

    for (const auto& disk : disks) {
        float size = getDiskSize(QString::fromStdString(disk));

        if (size < 65000) {
            sdCardPath = QString::fromStdString(disk);
        } else {
            hddPath = QString::fromStdString(disk);
        }
    }

    ui->sdLE->setText(sdCardPath);
    ui->hddLE->setText(hddPath);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::setCopyProgressBar(int value)
{
    ui->copyProgrB->setValue(value);
}

void MainWindow::copy()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->copyProgrB->setMaximum(getFolderDiskUsage("/media/nicolas/SD32"));
    copyThread = new CopyThread;
    copyThread->src = ui->sdLE->text();
    copyThread->dest = ui->hddLE->text() + "/test3";
    copyCheck = new CopyCheckThread;
    copyCheck->dest = ui->hddLE->text() + "/test3";
    connect(copyThread->signal, &CopySignals::copyInterrupted, this, &MainWindow::done);
    connect(copyCheck->signal, &CopyCheckSignals::dataCopied, this, &MainWindow::setCopyProgressBar);

    copyThread->start();
    copyCheck->start();
}

void MainWindow::eject()
{
}

void MainWindow::done()
{
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::abortCopy()
{
    ui->copyingFileLb->setText("Waiting for file to finish...");
    copyThread->quit();
    copyCheck->quit();
}

void MainWindow::setCopyProgressBarStylesheet()
{
    ui->copyProgrB->setStyleSheet("QProgressBar { background-color: rgb(100, 130, 200); "
                                  "color: rgb(230, 230, 230); "
                                  "border-style: none; "
                                  "border-radius: 30px; "
                                  "text-align: center; } "
                                  "QProgressBar::chunk{ "
                                  "text-align: center; "
                                  "border-radius: 30px; "
                                  "background-color: "
                                  "qlineargradient(spread:pad, "
                                  "x1:0, y1:0, x2:1, y2:1, "
                                  "stop:0 rgba(0, 0, 0, 255), "
                                  "stop:0.0138249 rgba(0, 0, 0, 255), "
                                  "stop:0.0184332 rgba(143, 66, 231, 255), "
                                  "stop:0.976959 rgba(14, 218, 195, 255), "
                                  "stop:1 rgba(255, 255, 255, 255)); }");
}
