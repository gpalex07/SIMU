#include "controleur.h"

controleur::controleur()
{
    _tileOrigineX    =0;
    _tileOrigineY    =0;
    _iterDeplacement =0;
    _tool            =1;
}

void controleur::runSimu(modele * inPtrModele, vue * inPtrVue)
{
    _ptrVue    =inPtrVue;
    _ptrModele =inPtrModele;


    _ptrVue->drawMap(_tileOrigineX, _tileOrigineY);
    //_ptrVue->drawAllAnimals(_tileOrigineX, _tileOrigineY);


    // On choisit où vont se déplacer les gnous
    std::vector<gnou*>::iterator it;
    std::vector<gnou*>& ptrGnou =_ptrModele->getTabGnous();

    for(it =ptrGnou.begin(); it!=ptrGnou.end(); ++it)
    {
        (*it)->chooseDirection();
    }

    // On écoute les évènements clic sur le widget qui contient la map
    QObject::connect(_ptrVue->getPtrScene(), SIGNAL(mouseClick(QGraphicsSceneMouseEvent*)), this, SLOT(mouseEvent(QGraphicsSceneMouseEvent*)));


    // Organise le déplacement des animaux
    QTimer * timer =new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(moveAnimals()));
    timer->start(25);

    _ptrVue->redrawAllAnimals(0, 0, 0);

    if(_ptrVue != NULL)
        QObject::connect(_ptrVue->getPtrBtnEau(), SIGNAL(clicked()), this, SLOT(changeTool()));
    else qDebug() << "controleur::runSimu() > Warning: _ptrVue = NULL";
}

void controleur::changeTool()
{
    qDebug() << "clic" << sender();
    //QPushButton * btn = sender();
    //btn->setStyleSheet("background:black");
}

void controleur::mouseEvent(QGraphicsSceneMouseEvent *event)
{
    // On regarde si la bouton gauche de la souris est cliqué
    bool boutonGauchePressed=event->buttons() & Qt::LeftButton;


    int tileClickedX=event->scenePos().x()/_ptrVue->getTileW()+_tileOrigineX;
    int tileClickedY=event->scenePos().y()/_ptrVue->getTileH()+_tileOrigineY;


    // C'est un clic gauche (avec ou sans mouvement de la souris)
    if(boutonGauchePressed)
    {
        if(_tool == 1)          // Outil dessin eau
        {
            drawWater(tileClickedX,   tileClickedY);
            drawWater(tileClickedX+1, tileClickedY);
            drawWater(tileClickedX,   tileClickedY+1);
            drawWater(tileClickedX+1, tileClickedY+1);
            /*drawWater(tileClickedX,   tileClickedY-1);
            drawWater(tileClickedX-1, tileClickedY);
            drawWater(tileClickedX+1, tileClickedY);
            drawWater(tileClickedX,   tileClickedY+1);*/

            // Rafraichit l'affichage de la zone
            for(int i=-2; i<3; ++i)
            {
                for(int j=-2; j<3; ++j)
                {
                    if(_ptrModele->estDansLaMap(tileClickedX+i, tileClickedY+j) && _ptrModele->isTileWater(tileClickedX+i, tileClickedY+j))
                        drawWater(tileClickedX+i, tileClickedY+j);
                }
            }
        }
    }

//    if(_ptrModele->estDansLaMap())
}

void controleur::moveAnimals()
{
    std::vector<gnou*>::iterator it;
    std::vector<gnou*>& ptrGnou =_ptrModele->getTabGnous();


    if(_iterDeplacement == _ptrVue->getTileW()) // Si on est arrivé à destination
    {
        _iterDeplacement=0;

            /*if(isVisibleOnScreen((*it)->getNextTileX(), (*it)->getNextTileY())) // Si le gnou est visible à l'écran, on doit le supprimer car ensuite on va modifier sa position, donc on pourra plus le retrouver pour le supprimer
                _ptrVue->removeAnimal(*it);*/

            /*(*it)->setDirX(1);
            (*it)->setDirY(1);
                                        // La case courante devient la case vers laquelle on vient de choisir de se déplacer.
            (*it)->objectifAtteint();   // Remet le déplacement à 0 suivant les 2 directions.

            (*it)->checkPosTore();      // On s'assure qu'on reste dans la map.
            (*it)->updateOrientation();*/



            // On regarde où était le gnou et on supprime sa texture sur cette case là

            // On calcule où va aller le gnou

            // On affiche le gnou sur sa nouvelle position

    }
        //_ptrVue->redrawAllAnimals(_tileOrigineX, _tileOrigineY, _iterDeplacement);    // Redessine les animaux à l'écran
        ++_iterDeplacement;
        _ptrModele->moveAnimals();


}

void controleur::keyEventManager(QKeyEvent *event)
{
    int key = event->key();

        //qDebug() << "Recu " << event->key();

    if(key == Qt::Key_Right && _tileOrigineX <_ptrModele->getMapW() -_ptrVue->getNbColonnes())
    {
        /*_ptrVue->cleanMap();
        _ptrVue->drawMap(_tileOrigineX, _tileOrigineY);*/
        ++_tileOrigineX;
        _ptrVue->scrollRight();
    } else if(key == Qt::Key_Left && _tileOrigineX > 0)
    {
        --_tileOrigineX;
        _ptrVue->scrollLeft();
        //_ptrVue->scrollLeft();
    } else if(key == Qt::Key_Up && _tileOrigineY > 0)
    {
        --_tileOrigineY;
        _ptrVue->scrollUp();
    } else if(key == Qt::Key_Down && _tileOrigineY <_ptrModele->getMapH() -_ptrVue->getNbLignes())
    {
        ++_tileOrigineY;
        _ptrVue->scrollDown();
    }
}

bool controleur::isVisibleOnScreen(int inTileX, int inTileY)
{
    return inTileX>=_tileOrigineX && inTileX<_tileOrigineX+_ptrVue->getNbColonnes()
                && inTileY>=_tileOrigineY && inTileY<_tileOrigineY+_ptrVue->getNbLignes();
}

void controleur::drawWater(int inTileX, int inTileY)
{
    // Controler qu'on est dans les limites de la map
    if(_ptrModele->estDansLaMap(inTileX-1, inTileY-1) && _ptrModele->estDansLaMap(inTileX+1, inTileY+1))
    {
    /*int a=_dataMap[inTileX-1][inTileY-1];
    int b=_dataMap[inTileX]  [inTileY-1];
    int c=_dataMap[inTileX+1][inTileY-1];
    int d=_dataMap[inTileX-1][inTileY];
    int e=_dataMap[inTileX+1][inTileY];
    int f=_dataMap[inTileX-1][inTileY+1];
    int g=_dataMap[inTileX]  [inTileY+1];
    int h=_dataMap[inTileX+1][inTileY+1];*/

    int a=(*_ptrModele)[inTileY-1][inTileX-1];
    int b=(*_ptrModele)[inTileY-1][inTileX];
    int c=(*_ptrModele)[inTileY-1][inTileX+1];
    int d=(*_ptrModele)[inTileY]  [inTileX-1];
    int e=(*_ptrModele)[inTileY]  [inTileX+1];
    int f=(*_ptrModele)[inTileY+1][inTileX-1];
    int g=(*_ptrModele)[inTileY+1][inTileX];
    int h=(*_ptrModele)[inTileY+1][inTileX+1];

    //qDebug() << a << b << c;


    // _dataMap[][]: <10 => Herbe

    int valueTile;

    if(_ptrModele->isWater(b))
    {
        if(_ptrModele->isWater(d))
        {
            if(_ptrModele->isWater(e))
            {
                if(_ptrModele->isWater(g))
                {
                    if(_ptrModele->isWater(a))
                    {
                        if(_ptrModele->isWater(c))
                        {
                            if(_ptrModele->isWater(f))
                            {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=47;
                                } else valueTile=19;
                            } else {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=20;
                                } else valueTile=39;
                            }
                        } else { // c n'est pas de l'eau
                            if(_ptrModele->isWater(f))
                            {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=18;
                                } else valueTile=38;
                            } else {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=42;
                                } else valueTile=43;
                            }
                        }
                    } else { // a n'est pas de l'eau
                        if(_ptrModele->isWater(c))
                        {
                            if(_ptrModele->isWater(f))
                            {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=17;
                                } else valueTile=41;
                            } else {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=40;
                                } else valueTile=45;
                            }
                        } else { // c n'est pas de l'eau
                            if(_ptrModele->isWater(f))
                            {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=37;
                                } else valueTile=44;
                            } else {
                                if(_ptrModele->isWater(h))
                                {
                                    valueTile=46;
                                } else valueTile=12;
                            }
                        }
                    }
                } else { // g n'est pas de l'eau
                    if(_ptrModele->isWater(a))
                    {
                        if(_ptrModele->isWater(c))
                        {
                            valueTile=23;
                        } else valueTile=31;
                    } else {
                        if(_ptrModele->isWater(c))
                        {
                            valueTile=35;
                        } else valueTile=8;
                    }
                }
            } else { // e n'est pas de l'eau
                if(_ptrModele->isWater(g))
                {
                    if(_ptrModele->isWater(a))
                    {
                        if(_ptrModele->isWater(f))
                        {
                            valueTile=22;
                        } else valueTile=34;
                    } else {
                        if(_ptrModele->isWater(f))
                        {
                            valueTile=30;
                        } else valueTile=11;
                    }
                } else { // g n'est pas de l'eau
                    if(_ptrModele->isWater(a))
                    {
                        valueTile=27;
                    } else valueTile=16;
                }
            }
        } else { // d n'est pas de l'eau
            if(_ptrModele->isWater(e))
            {
                if(_ptrModele->isWater(g))
                {
                    if(_ptrModele->isWater(c))
                    {
                        if(_ptrModele->isWater(h))
                        {
                            valueTile=24;
                        } else valueTile=32;
                    } else {
                        if(_ptrModele->isWater(h))
                        {
                            valueTile=36;
                        } else valueTile=9;
                    }
                } else { // g n'est pas de l'eau
                    if(_ptrModele->isWater(c))
                    {
                        valueTile=28;
                    } else valueTile=13;
                }
            } else { // e n'est pas de l'eau
                if(_ptrModele->isWater(g))
                {
                    valueTile=7;
                } else valueTile=6;
            }
        }
    } else { // b n'est pas de l'eau
        if(_ptrModele->isWater(d))
        {
            if(_ptrModele->isWater(e))
            {
                if(_ptrModele->isWater(g))
                {
                    if(_ptrModele->isWater(f))
                    {
                        if(_ptrModele->isWater(h))
                        {
                            valueTile=21;
                        } else valueTile=33;
                    } else {
                        if(_ptrModele->isWater(h))
                            valueTile=29;
                        else valueTile=10;
                    }
                } else { // g n'est pas de l'eau
                    valueTile=4;
                }
            } else { // e n'est pas de l'eau
                if(_ptrModele->isWater(g))
                {
                    if(_ptrModele->isWater(f))
                    {
                        valueTile=26;
                    } else valueTile=15;
                } else valueTile=3;
            }
        } else { // d n'est pas de l'eau
            if(_ptrModele->isWater(e))
            {
                if(_ptrModele->isWater(g))
                {
                    if(_ptrModele->isWater(h))
                    {
                        valueTile=25;
                    } else valueTile=14;
                } else valueTile=2;
            } else {
                if(_ptrModele->isWater(g))
                {
                    valueTile=5;
                } else valueTile=1;
            }
        }
    }

    //qDebug() << "On ecrit la valeur" << valueTile;
    (*_ptrModele)[inTileY][inTileX]=valueTile+DEBUT_TEXTURES_EAU-1;

    //qDebug() << "On redessine";
    // Si la tile est visible à l'écran, on la redessine
    if(isVisibleOnScreen(inTileX, inTileY))
        _ptrVue->reDrawTile(inTileX,inTileY);

    //qDebug() << "Termine";
    } else {
        qDebug() << "Eloignez vous du bord";
    }

}
