#include "qgraphicsscenecustom.h"
#include <QDebug>

QGraphicsSceneCustom::QGraphicsSceneCustom()
{
}

// Lorsqu'une touche est appuyée, on regarde si c'est une flèche directionnelle,
// si oui, on scroll dans cette direction
void QGraphicsSceneCustom::keyPressEvent(QKeyEvent * event)
{
    emit keyPressed(event);

    //int key = event->key();

    //qDebug() << "Recu " << event->key();

    /*if(key == Qt::Key_Right)
    {
        _ptrMap->scrollRIGHT();
    } else if(key == Qt::Key_Left)
    {
        _ptrMap->scrollLEFT();
    } else if(key == Qt::Key_Up)
    {
        _ptrMap->scrollUP();
    } else if(key == Qt::Key_Down)
    {
        _ptrMap->scrollDOWN();
    }*/

}

// On se contente d'émettre un signal pour que le contrôleur sache où on a cliqué sur la scène
// c'est ensuite le contrôleur qui appelle les fonction de dessin (d'eau, ...) qui vont bien
// (il choisit laquelle appeler en fonction de l'outil qu'on a sélectionné)
void QGraphicsSceneCustom::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    emit mouseClick(event);
}

void QGraphicsSceneCustom::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    emit mouseClick(event);
}
