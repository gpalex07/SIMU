#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsSceneMouseEvent>

#include "vue/vue.h"
#include "modele/modele.h"

class controleur : public QObject
{
    Q_OBJECT

private:
    modele      * _ptrModele;
    vue         * _ptrVue;

    int           _tileOrigineX, _tileOrigineY;
    int           _iterDeplacement;                 // Mémorise le nombre de pixel dont on s'est déplacé (sert pour l'interpolation de mouvement)
    int           _tool;

public:
    controleur();
    void runSimu(modele * inPtrModele, vue * inPtrVue=NULL);    // La vue est facultative
    void drawWater(int inTileX, int inTileY);
    bool isVisibleOnScreen(int inTileX, int inTileY);

public slots:
    void moveAnimals();
    void keyEventManager(QKeyEvent * event);
    void mouseEvent(QGraphicsSceneMouseEvent *event);
    void changeTool();
};

#endif // CONTROLEUR_H
