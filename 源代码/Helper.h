#ifndef HELPER_H
#define HELPER_H

#include <QObject>

class Helper: public QObject
{
    Q_OBJECT
public:
    Helper(QObject *parent=0);
public slots:
    void resetKey();
    void timing();
};

#endif // HELPER_H
