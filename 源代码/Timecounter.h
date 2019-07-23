#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

#include <QGraphicsTextItem>

class TimeCounter : public QGraphicsTextItem{
    Q_OBJECT
public:
    TimeCounter(QGraphicsItem * parent=0);
    int count;
public slots:
    void showtime();
};

#endif // TIMECOUNTER_H
