#include "modele.h"

modele::modele(int inMapW, int inMapH)
    : _mapW(inMapW), _mapH(inMapH)
{

    // Crée la matrice qui contient la map
    _dataMap =new int*[inMapH];
    for(int i=0; i<_mapH; ++i)
        _dataMap[i] =new int[inMapW];

    // Crée la matrice qui contient les pointeurs vers les animaux
    _animalsMap =new animal**[inMapH];
    for(int i=0; i<_mapH; ++i)
        _animalsMap[i] =new animal*[inMapW];

    // Initialise tous les pointeurs à NULL
    for(int i=0; i<_mapH; ++i)
    {
        for(int j=0; j<_mapW; ++j)
        {
            _animalsMap[i][j] =NULL;
        }
    }

    // Génère une map aléatoire
    createRandomMap();
    populateRandomGnou();
}

void modele::createRandomMap()
{
    int i,j;

    srand(time(0));

    for(i=0; i<_mapH; ++i)
    {
        for(j=0; j<_mapW; ++j)
        {
            _dataMap[i][j] =rand()%NB_TEXTURES_HERBE +DEBUT_TEXTURES_HERBE;
        }
    }
}

void modele::addGnou(int inX, int inY)
{
    if(_animalsMap[inY][inX] == NULL)
    {
        gnou * newGnou = new gnou(inX, inY, this);
        _tabGnous.push_back(newGnou);
        _animalsMap[inY][inX] =newGnou;
    } else qDebug() << "Error: modele::addGnou() > impossible d'ajouter un animal a cet endroit, la case est deja occupee par un autre animal";
}

void modele::populateRandomGnou()
{
    srand(time(0));

    for(int i=0; i<50; ++i)
    {
        addGnou(rand()%_mapW, rand()%_mapH);
    }
}

void modele::moveAnimals()
{
    int j,k;

    for(int i=0; i<_tabGnous.size(); ++i)
    {
        // Le gnou libère sa case
        _animalsMap[_tabGnous[i]->getTileY()][_tabGnous[i]->getTileX()] =NULL;

        // Calcule où va se déplacer le gnou
        j=rand()%3-1; k=rand()%3-1;
        _tabGnous[i]->setDirX(j);
        _tabGnous[i]->setDirY(k);

        // On vérifie que la case est libre
        //qDebug() << qAbs(_tabGnous[i]->getTileY()+k)%_mapH << qAbs(_tabGnous[i]->getTileX()+j)%_mapW;
        if(_animalsMap[qAbs(_tabGnous[i]->getTileY()+k)%_mapH][qAbs(_tabGnous[i]->getTileX()+j)%_mapW] ==NULL)
        {
            // Le gnou occupe cette nouvelle case
            _tabGnous[i]->setTileX(qAbs(_tabGnous[i]->getTileX()+j)%_mapW);
            _tabGnous[i]->setTileY(qAbs(_tabGnous[i]->getTileY()+k)%_mapH);
            //qDebug() << qAbs(_tabGnous[i]->getTileY()+k)%_mapH << qAbs(_tabGnous[i]->getTileX()+j)%_mapW;
            _animalsMap[qAbs(_tabGnous[i]->getTileY()+k)%_mapH][qAbs(_tabGnous[i]->getTileX()+j)%_mapW] =_tabGnous[i];
        }
    }
}
