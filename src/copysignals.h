#ifndef COPYSIGNALS_H
#define COPYSIGNALS_H

#include <QObject>
#include <filesystem>

class CopySignals : public QObject
{
        Q_OBJECT
    public:
        explicit CopySignals(QObject* parent = nullptr);

    Q_SIGNALS:
        void copyFinished();

        void copyInterrupted();

        void copyError(std::filesystem::filesystem_error);
};

#endif // COPYSIGNALS_H
