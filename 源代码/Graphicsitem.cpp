#include "Game.h"
#include "Graphicsitem.h"

#include <QGraphicsItem>

extern Game *game;

GraphicsItem::GraphicsItem(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    setFlags(QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsMovable);
}
