#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("AlphaCopyCpp");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString& locale : uiLanguages) {
        const QString baseName = "AlphaCopyCpp_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();

    if (argc > 1) {
        if (std::string(argv[1]) == "--fullscreen") {
            w.switchScreenMode();
        }
    }

    return a.exec();
}
