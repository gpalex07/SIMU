#ifndef ANIMAL_H
#define ANIMAL_H

#define NB_TEXTURES_GNOUS     8
#define DEBUT_TEXTURES_GNOUS 56

#include <time.h>
#include <QDebug>
#include <cstdlib>

class modele;


class animal
{
protected:
    int _tileX, _tileY;                 // La position de l'animal, dans la matrice de la map (i.e. sur le terrain)
    int _nextTileX, _nextTileY;         // La position vers laquelle se déplace l'animal
    int _lastTileX, _lastTileY;         // La position précédente de l'animal
    int _orientation;                   // L'orientation de l'animal (NORD, NORD-EST, EST ..)
    int _deplacementX, _deplacementY;   // La distance déjà parcourue (en pixels) jusqu'à la prochaine case
    int _dirX, _dirY;                   // La direction de déplacement

    modele  * _ptrModele;

public:
    animal(int inX, int inY, modele * inPtrModele);
    virtual ~animal(){}
    void setPos(int inX, int inY);                          // Setteurs

    int getTileX()        const { return _tileX;        }   // Accesseurs
    int getTileY()        const { return _tileY;        }
    int getNextTileX()    const { return _nextTileX;    }
    int getNextTileY()    const { return _nextTileY;    }
    int getLastTileX()    const { return _lastTileX;    }
    int getLastTileY()    const { return _lastTileY;    }
    int getDeplacementX() const { return _deplacementX; }
    int getDeplacementY() const { return _deplacementY; }
    virtual int getTextureNumber() const =0;

    int getDirX() const { return _dirX; }
    int getDirY() const { return _dirY; }
    void setDirX(int inDirX){ _dirX=inDirX; }
    void setDirY(int inDirY){ _dirY=inDirY; }
    void setTileX(int inTileX){ _tileX=inTileX; }
    void setTileY(int inTileY){ _tileY=inTileY; }

    void incrementerDeplacementX(){ ++_deplacementX; }
    void incrementerDeplacementY(){ ++_deplacementY; }

    void objectifAtteint();         // La case objectif devient la case courante (car on est arrivé dessus)
    void chooseDirection();
    void updateOrientation();       // Met à jour l'orientation de la texture d'animal
    void checkPosTore();            // Tiens compte du fait que la map est un tore
    virtual void calcNextMove()=0;
};

#endif // ANIMAL_H
