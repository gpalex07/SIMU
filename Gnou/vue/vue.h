#ifndef VUE_H
#define VUE_H
#include <map>
#include <QDebug>
#include <math.h>
#include <QPixmap>
#include <algorithm>
#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>

#include "modele/modele.h"
#include "anneauqgraphicsitem.h"
#include "qgraphicsscenecustom.h"



class vue : public QObject
{
    Q_OBJECT

protected:
    QGraphicsView         * _view;
    QGraphicsSceneCustom  * _scene;              // La scène, où tout s'affiche
    QMainWindow           * _mainWindow;

    QGraphicsItemGroup    * _layerEnvironnement; // Contient toutes les tiles de terrain affichées à l'écran
    QGraphicsItemGroup    * _layerAnimaux;       // Contient tous les sprites d'animaux

    QPixmap               * _textures;           // Tableau qui contient toutes les textures (sert de cache)

    anneauQGraphicsItem   * _anneau;             // Anneau qui mémorise les pointeurs vers chaque tile affichée ; nécessaire pour ensuite les supprimer
    QGraphicsPixmapItem *** _tabPtrPixmapAnimaux;// Tableau à 2 dimensions qui contient pour chaque case affichée à l'écran un pointeur vers l'animal présent sur cette case

    modele                * _ptrModele;          // Utilisé pour récupérer les données de la map, lorsqu'on doit l'afficher

    std::map<animal*,QGraphicsPixmapItem*>      _vectPixmapGnouDansScene;           // Un vecteur qui contient les pointeurs vers les gnous affichés à l'écran

    QPushButton           * _btnEau, * _btnHerbe;

    int                     _width, _height;
    int                     _tileOrigineX, _tileOrigineY;   // Coordonnées dans la matrice de la tile la plus en haut à gauche affichée à l'écran
    int                     _tileW, _tileH;
    int                     _nbColonnes, _nbLignes;
    int                     _menuW;
    int                     _iterDeplacement;

public:
    vue(modele * inPtrModele);
    void drawLine(int inLineNum);
    void drawColumn(int inColumnNum);
    void drawMap(int inTileOrigineX, int inTileOrigineY);
    void drawTile(int inTileX, int inTileY);
    void reDrawTile(int inTileX, int inTileY);

    void removeLine(int inLineNum);
    void removeColumn(int inColumnNum);
    void removeTile(int inTileX, int inTileY);
    void cleanMap();

    void loadTextures();

    int                    getTileW()      const { return _tileW;      }
    int                    getTileH()      const { return _tileH;      }
    int                    getNbLignes()   const { return _nbLignes;   }
    int                    getNbColonnes() const { return _nbColonnes; }
    QGraphicsSceneCustom * getPtrScene()         { return _scene;      }

    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();

    void cleanAllAnimals();
    void drawAnimal(animal * inAnimal, int inIterDeplacement=-1);
    void reDrawAnimal(animal * inAnimal);
    void removeAnimal(animal * inAnimal);
    void drawAllAnimals(int inTileOrigineX, int inTileOrigineY);
    void redrawAllAnimals(int inTileOrigineX, int inTileOrigineY, int inIterDeplacement);
    void redrawAllAnimals2(int inTileOrigineX, int inTileOrigineY, int inIterDeplacement);
    void drawAnimal2(animal * inAnimal, int inIterDeplacement=-1);
    void reDrawAnimal2(animal * inAnimal);
    void removeAnimal2(animal * inAnimal);
    void removeAnimalLine(int inLineNum);
    void removeAnimalColumn(int inColumnNum);

    void setUpMenu();

    QPushButton * getPtrBtnEau()  { return _btnEau;   }
    QPushButton * getPtrBtnHerbe(){ return _btnHerbe; }


};

#endif // VUE_H
