#ifndef QGRAPHICSSCENECUSTOM_H
#define QGRAPHICSSCENECUSTOM_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>


// Ajoute de la gestion des événements claviers et souris au QGraphicsScene
class QGraphicsSceneCustom : public QGraphicsScene
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

public:
    QGraphicsSceneCustom();

signals:
    void mouseClick(QGraphicsSceneMouseEvent *);
    void keyPressed(QKeyEvent *);
};

#endif // QGRAPHICSSCENECUSTOM_H
