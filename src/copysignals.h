#ifndef COPYSIGNALS_H
#define COPYSIGNALS_H

#include <QObject>

class CopySignals : public QObject
{
        Q_OBJECT
    public:
        explicit CopySignals(QObject* parent = nullptr);

    Q_SIGNALS:
        void copyFinished();

        void copyInterrupted();

};

#endif // COPYSIGNALS_H
