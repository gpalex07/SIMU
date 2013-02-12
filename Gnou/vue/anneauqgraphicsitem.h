#ifndef ANNEAUQGRAPHICSITEM_H
#define ANNEAUQGRAPHICSITEM_H

#include <QDebug>
#include <QGraphicsPixmapItem>

// Gestion d'une file de type anneau
class anneauQGraphicsItem
{
private:
    QGraphicsPixmapItem *** _data;

    //int _firstRow,    _lastRow;
    int _nbLignes,    _nbColonnes;
    //int _firstColumn, _lastColumn;

public:
    anneauQGraphicsItem(int inNbLignes, int inNbColonnes);
    QGraphicsPixmapItem**&  operator[](const int inIndex) { return _data[inIndex]; }
    //QGraphicsPixmapItem**   operator[](int inIndex) const { return _data[inIndex]; }
    QGraphicsPixmapItem *** & getData(){ return _data; }

    void insererTile(QGraphicsPixmapItem *inTile, int inTileX, int inTileY, int inTileOrigineX, int inTileOrigineY); // Insère la tile fournie au bon endroit dans l'anneau

    /*void deplacerBas();     // Modifie les pointeurs de début/fin de l'anneau
    void deplacerHaut();    // Utilisé lorsqu'on scrolle
    void deplacerDroite();
    void deplacerGauche();*/

    /*int getLastRow()    { return _lastRow;     }    // Accesseurs
    int getFirstRow()   { return _firstRow;    }
    int getLastColumn() { return _lastColumn;  }
    int getFirstColumn(){ return _firstColumn; }*/

    int getTileLine(int inTileY, int inTileOrigineY);   // Retourne la colonne/ligne d'une tile dans l'anneau, ou -1 si
    int getTileColumn(int inTileX, int inTileOrigineX); // la tile n'est pas dans l'anneau, i.e. pas affichée à l'écran

    void lineRemoved(int inLineNum, int inTileOrigineY);
    void columnRemoved(int inColumnNum, int inTileOrigineX);
};

#endif // ANNEAUQGRAPHICSITEM_H
