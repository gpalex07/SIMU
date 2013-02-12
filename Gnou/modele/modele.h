#ifndef MODELE_H
#define MODELE_H

#include <time.h>
#include <QDebug>
#include <vector>
#include <cstdlib>

#include "animaux/gnou.h"
#include "animaux/animal.h"

#define NB_TEXTURES_EAU      47
#define NB_TEXTURES_HERBE     9  // Le nombre de textures d'herbe
#define DEBUT_TEXTURES_EAU    9
#define DEBUT_TEXTURES_HERBE  0  // Le code de la première texture d'herbe

class modele
{
private:
    int                     _mapW, _mapH;
    int                  ** _dataMap;       // La matrice qui contient la map (i.e. le terrain)
    animal              *** _animalsMap;    // Matrice dont chaque case est un pointeur vers un animal. Il n'y a qu'un animal possible par case.
    std::vector<gnou*>      _tabGnous;      // Contient les pointeurs de tous les gnous


public:
    modele(int inMapW, int inMapH);

    int* &  operator[](const int inTileY) { return _dataMap[inTileY]; }

    void    createRandomMap();
    void    populateRandomGnou();

    void    addGnou(int inX, int inY);

    int                 getMapW(){ return _mapW; }
    int                 getMapH(){ return _mapH; }
    animal            * getAnimalsMap(int inColumn, int inLine){ return _animalsMap[inLine][inColumn]; }
    void                setAnimalsMap(int inColumn, int inLine, animal * inAnimal){ _animalsMap[inLine][inColumn]=inAnimal; }
    std::vector<gnou*>& getTabGnous(){ return _tabGnous; }
    bool                estDansLaMap(int inTileX, int inTileY){ return (inTileX>=0 && inTileX<_mapW && inTileY>=0 && inTileY<_mapH); }
    bool                isWater(int inValeurTexture){ return inValeurTexture>=DEBUT_TEXTURES_EAU && inValeurTexture<(DEBUT_TEXTURES_EAU+NB_TEXTURES_EAU); }
    bool                isTileWater(int inTileX, int inTileY){ return isWater(_dataMap[inTileY][inTileX]); }
    void                moveAnimals();
};

#endif // MODELE_H
