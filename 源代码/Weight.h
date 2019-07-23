#ifndef WEIGHT_H
#define WEIGHT_H

#include <QGraphicsTextItem>

class Weight: public QGraphicsTextItem{
public:
    Weight(QGraphicsItem * parent=0);
    void showWeight();
};

#endif
