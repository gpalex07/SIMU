#include "animal.h"
#include "modele/modele.h"

animal::animal(int inX, int inY, modele * inPtrModele)
{
    _tileX        =inX;
    _tileY        =inY;
    _dirX         =0;
    _dirY         =0;
    _nextTileX    =inX;
    _nextTileY    =inY;
    _lastTileX    =inX;
    _lastTileY    =inY;
    _deplacementX =0;
    _deplacementY =0;
    _ptrModele    =inPtrModele;
    qDebug() << "Animal cree";
}

void animal::setPos(int inX, int inY)
{
    _tileX        =inX;
    _tileY        =inY;
}

// Lorsqu'un animal va sortir de la map, on le fait réapparaître de l'autre côté.
void animal::checkPosTore()
{
    this->_tileX=this->_tileX%_ptrModele->getMapW();
    this->_tileY=this->_tileY%_ptrModele->getMapH();
}

void animal::objectifAtteint()
{
    // Modifie notre position dans le tableau qui mémorise la position de tous les animaux
    //_ptrModele->setAnimalsMap(_tileX, _tileY, NULL);
    /*_tileX        =_nextTileX;
    _tileY        =_nextTileY;*/
    _tileX       +=_dirX;
    _tileY       +=_dirY;
    /*_deplacementX =0;
    _deplacementY =0;*/
    //_ptrModele->setAnimalsMap(_tileX, _tileY, this);
}

void animal::chooseDirection()
{
    const int ITER_MAX=15;
    int iter=0;
    do {
        _dirX      = rand()%3-1;
        _dirY      = rand()%3-1;
        _nextTileX = _tileX +_dirX;
        _nextTileY = _tileY +_dirY;
        ++iter;//qDebug() << _nextTileX << _nextTileY;
    } while (iter< ITER_MAX && (!_ptrModele->estDansLaMap(_nextTileX, _nextTileY) || _ptrModele->isTileWater(_nextTileX, _nextTileY) || _ptrModele->getAnimalsMap(_nextTileX, _nextTileY) != NULL));

    if(iter==ITER_MAX){
        qDebug() << "Warning: animal::chooseDirection() > (animal"<< this << ") Au bout de" << ITER_MAX << "tentatives, on n'a pas trouve d'emplacement qui ne soit pas de l'eau. Le gnou est probablement DANS l'eau";
        _nextTileX=_tileX;  // On n'a pas trouvé où se déplacer, donc on ne bouge pas.
        _nextTileY=_tileY;

        _dirX      = 0;
        _dirY      = 0;
    } else {    // On libère la case où on était, et on occupe la case où l'on va
        _ptrModele->setAnimalsMap(_tileX, _tileY, NULL);
        _ptrModele->setAnimalsMap(_nextTileX, _nextTileY, this);
    }

    updateOrientation();
}

void animal::updateOrientation()
{
    if(_dirX>0)
        {
            if(_dirY>0)
                _orientation =3;
            else if(_dirY==0)
                _orientation =2;
            else _orientation =1;
        } else if(_dirX==0)
        {
            if(_dirY>0)
                _orientation =4;
            else if(_dirX<0)
                _orientation =0;
        } else
        {
            if(_dirY>0)
                _orientation =5;
            else if(_dirY==0)
                _orientation =6;
            else _orientation =7;
        }


    /*if(_nextTileX>_tileX)
    {
        if(_nextTileY>_tileY)
            _orientation =3;
        else if(_nextTileY==_tileY)
            _orientation =2;
        else _orientation =1;
    } else if(_nextTileX ==_tileX)
    {
        if(_nextTileY>_tileY)
            _orientation =4;
        else if(_nextTileY<_tileY)
            _orientation =0;
    } else
    {
        if(_nextTileY>_tileY)
            _orientation =5;
        else if(_nextTileY==_tileY)
            _orientation =6;
        else _orientation =7;
    }*/
}
