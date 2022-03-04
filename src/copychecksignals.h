#ifndef COPYCHECKSIGNALS_H
#define COPYCHECKSIGNALS_H

#include <QObject>

class CopyCheckSignals : public QObject
{
        Q_OBJECT
    public:
        CopyCheckSignals(QObject* parent);
    Q_SIGNALS:
        void dataCopied(double);
};

#endif // COPYCHECKSIGNALS_H
