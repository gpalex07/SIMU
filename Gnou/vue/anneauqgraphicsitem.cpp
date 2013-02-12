#include "anneauqgraphicsitem.h"

anneauQGraphicsItem::anneauQGraphicsItem(int inNbLignes, int inNbColonnes)
{
    _nbLignes    = inNbLignes;
    _nbColonnes  = inNbColonnes;
    /*_firstRow    = 0;
    _firstColumn = 0;
    _lastRow     = _nbLignes-1;
    _lastColumn  = _nbColonnes-1;*/


    // Initialisation du tableau, i.e. de l'anneau lui-même
    _data = new QGraphicsPixmapItem**[_nbLignes];
    for (int i=0; i<_nbLignes; ++i)
        _data[i] = new QGraphicsPixmapItem*[_nbColonnes];

}

// Ajoute la tile aux coordonnées (dans la matrice de la map) inTileX/inTileY au bon endroit dans l'anneau
void anneauQGraphicsItem::insererTile(QGraphicsPixmapItem *inTile, int inTileX, int inTileY, int inTileOrigineX, int inTileOrigineY)
{
    // Calcule la position de la tile dans l'anneau
    int x=getTileColumn(inTileX, inTileOrigineX);
    int y=getTileLine(inTileY, inTileOrigineY);


    //qDebug() << "Tile inseree en:" << x << y;
    _data[y][x] =inTile;
}

void anneauQGraphicsItem::columnRemoved(int inColumnNum, int inTileOrigineX)
{
    // Récupère le numéro de la colonne dans l'affichage
    int x =getTileColumn(inColumnNum, inTileOrigineX);

    // On regarde si on a supprimé la première ou la dernière colonne
    // si oui on met à jour les pointeurs de début/fin de l'anneau
    /*if(_firstColumn ==x)
        ++_firstColumn;
    else if(_lastColumn ==x)
        --_firstColumn;
    else qDebug() << "anneauQGraphicsItem::columnRemoved() > Warning: Suppression suspecte d'une colonne non situee a une extremite.";*/
    // En principe, on ne supprimera que les colonnes situées tout à gauche ou tout à droite
}

void anneauQGraphicsItem::lineRemoved(int inLineNum, int inTileOrigineY)
{
    // Récupère le numéro de la ligne dans l'affichage
    int y =getTileLine(inLineNum, inTileOrigineY);

    // On regarde si on a supprimé la première ou la dernière colonne
    // si oui on met à jour les pointeurs de début/fin de l'anneau
    /*if(_firstRow ==y)
        ++_firstRow;
    else if(_lastRow ==y)
        --_firstRow;
    else qDebug() << "anneauQGraphicsItem::lineRemoved() > Warning: Suppression suspecte d'une ligne non situee a une extremite.";*/
    // En principe, on ne supprimera que les lignes situées tout en haut ou tout en bas
}


// Retourne la colonne, dans l'anneau, de la tile à la colonne inTileX dans la matrice de la map
int anneauQGraphicsItem::getTileColumn(int inTileX, int inTileOrigineX)
{
    //return ((inTileX-inTileOrigineX)+_firstColumn) %_nbColonnes;
    return inTileX %_nbColonnes;
}

// Retourne la ligne, dans l'anneau, de la tile à la colonne inTileY dans la matrice de la map
int anneauQGraphicsItem::getTileLine(int inTileY, int inTileOrigineY)
{
    //return ((inTileY-inTileOrigineY)+_firstRow) %_nbLignes;
    return inTileY %_nbLignes;
}




/*void anneauQGraphicsItem::deplacerDroite()
{
    ++_lastColumn;
    ++_firstColumn;


    _lastColumn  = _lastColumn %_nbColonnes;
    _firstColumn = _firstColumn%_nbColonnes;
}

void anneauQGraphicsItem::deplacerGauche()
{
    --_lastColumn;
    --_firstColumn;

    _lastColumn  = _lastColumn %_nbColonnes;
    _firstColumn = _firstColumn%_nbColonnes;
}

void anneauQGraphicsItem::deplacerHaut()
{
    --_firstRow;
    --_lastRow;

    _lastRow  = _lastRow %_nbLignes;
    _firstRow = _firstRow%_nbLignes;
}

void anneauQGraphicsItem::deplacerBas()
{
    ++_firstRow;
    ++_lastRow;

    _lastRow  = _lastRow %_nbLignes;
    _firstRow = _firstRow%_nbLignes;
}*/
