#include "gnou.h"
#include "modele/modele.h"

gnou::gnou(int inTileX, int inTileY, modele *inPtrModele) : animal(inTileX, inTileY, inPtrModele)
{
    _orientation =rand()%8;
    _gnouQuonSuit = NULL;
}

int gnou::getTextureNumber() const
{
    return DEBUT_TEXTURES_GNOUS +_orientation;
}

// Permet de vérifier que le gnou qu'on suit suit des gnous qui suivent le leader, et qu'il n'y a pas de cycle ni rien
bool gnou::verifierQuOnSuitLeader()
{
    gnou * cour = this;
    bool suitLeader =false;

    while(cour!=NULL && cour->_gnouQuonSuit!=cour && cour->flag!=1)     // On met à 1 le flags des gnous qu'on a déjà recontrés pour savoir s'il y a un cycle
    {
        cour->flag =1;
        cour =cour->_gnouQuonSuit;
    }

    if(cour !=NULL && cour->_gnouQuonSuit ==cour)
        suitLeader =true;

    // On remet les flags à 0
    cour =this;
    while(cour !=NULL && cour->flag ==1)
    {
        cour->flag =0;
        cour =cour->_gnouQuonSuit;
    }

    return suitLeader;
}

// Permet d'aller à un point donné (inObjX et inObjY sont les coordonnées du point objectif dans la map)
void gnou::moveTo(int inObjX, int inObjY)
{
    if(inObjX!=_tileX || inObjY!=_tileY)
    {
        _dirX=_dirY=0;    // Par défaut, on ne fait rien (pas de déplacement)

        int Dx=abs(inObjX-_tileX);
        int Dy=abs(inObjY-_tileY);

        if(inObjX < _tileX)
        {
            if(inObjY < _tileY) {
                if(Dx<Dy){ _dirY=-1; if(Dx!=0 && rand()%((int)((float)Dy/(float)Dx))==0)_dirX=-1; }
                else if(Dx>Dy){ _dirX=-1; if(Dy!=0 && rand()%((int)((float)Dx/(float)Dy))==0)_dirY=-1; }
                else { _dirX=_dirY=-1; }
            } else if(inObjY > _tileY) {
                if(Dx<Dy){ _dirY=1; if(Dx!=0 && rand()%((int)((float)Dy/(float)Dx))==0)_dirX=-1; }
                else if(Dx>Dy){ _dirX=-1; if(Dy!=0 && rand()%((int)((float)Dx/(float)Dy))==0)_dirY=1; }
                else { _dirX=-1; _dirY=1; }
            } else {
                _dirX=-1;
            }
        } else if(inObjX > _tileX) {
            if(inObjY < _tileY) {
                if(Dx<Dy){ _dirY=-1; if(Dx!=0 && rand()%((int)((float)Dy/(float)Dx))==0)_dirX=1; }
                else if(Dx>Dy){ _dirX=1; if(Dy!=0 && rand()%((int)((float)Dx/(float)Dy))==0)_dirY=-1; }  // Plus on est proche de la diagonale, plus on a de chance de remonter. Permet de fluidifier les mouvements.
                else { _dirX=1; _dirY=-1; }
            } else if(inObjY > _tileY) {
                if(Dx<Dy){ _dirY=1; if(Dx!=0 && rand()%((int)((float)Dy/(float)Dx))==0)_dirX=1; }
                else if(Dx>Dy){ _dirX=1; if(Dy!=0 && rand()%((int)((float)Dx/(float)Dy))==0)_dirY=1; }
                else { _dirX=_dirY=1; }
            } else {
                _dirX=1;
            }
        } else {
            if(inObjY < _tileY) {
                _dirY=-1;
            } else if(inObjY > _tileY) {
                _dirY=1;
            } else {
                qDebug() << "gnou::moveTo() > Erreur : deux gnous sont sur la même case";
            }
        }


        // On vérifie qu'on peut se déplacer là où on voudrait
        /*if(voisinage[(int)(posy+diry)][(int)(posx+dirx)]!=NULL)
            contournerObstacle();   // Recalcule dirx et diry pour contourner l'obstacle (si on n'y arrive pas dirx=diry=0)*/

        // Si on a trouvé une case où contourner
        if(_ptrModele->estDansLaMap(_tileX+_dirX,_tileY+_dirY) && _ptrModele->getAnimalsMap(_tileY+_dirY,_tileX+_dirX)==NULL) // Si la case où on veut aller est libre on se déplace, sinon on ne bouge pas
        {
            qDebug() << "Deplacement fait";
            _ptrModele->setAnimalsMap(_tileY,_tileX,NULL);
            _nextTileX+=_tileX+_dirX;
            _nextTileY+=_tileY+_dirY;
            qDebug() <<_nextTileX <<  _nextTileY;
            _ptrModele->setAnimalsMap(_tileY,_tileX,this);
        } else { /*dirx=diry=0; On laisse les valeurs de dirx pour savoir vers où on se dirigeait */ /*changerQuiOnSuit(20);*/ qDebug() << "on peut pas suivre sinon on sort";_dirX=_dirY=0; } // Si on se retrouve coincé on choisit de suivre un autre gnou

    } else qDebug() << "gnou::moveTo() > Warning: on nous demande de nous deplacer vers une case objectif, alors qu'on y est deja.";
}

void gnou::suivreGnouQuonSuit()
{
    if(_gnouQuonSuit!=NULL)
    {
        moveTo(_gnouQuonSuit->_tileX, _gnouQuonSuit->_tileY);

    } else qDebug() << "On nous demande de suivre le gnou qu'on suit alors qu'on n'en suit aucun";
}

// Appelé pour s'insérer dans la file
void gnou::choisirQuiOnsuit(int inVision)
{
    //qDebug() << "debut choisir";
    // On remet _gnouQuonSuit à NULL pour ne suivre personne, si on n'arrive pas à s'insérer
    _gnouQuonSuit =NULL;
    _dirX=_dirY=0;

    // Compte le nombre de gnous du voisinage (cercle de vision) qui sont dans une file en mouvement
    // Les gnous trouvés n'appartiennent pas forcément à la même file
    int nbVoisins=0;
    int i,j;
    int min1=_tileY-inVision;    // Détermine les bornes du voisinage qu'on va explorer
    int max1=_tileY+inVision;
    int min2=_tileX-inVision;
    int max2=_tileX+inVision;

    if(min1<0) min1=0;
    if(max1>_ptrModele->getMapH()) max1=_ptrModele->getMapH();
    if(min2<0) min2=0;
    if(max2>_ptrModele->getMapW()) max2=_ptrModele->getMapW();

    for(i=min1; i<max1; ++i)    // Parcours le voisinage (du regard) et détermine le nombre de voisins qui appartiennent à une file en mouvement
        for(j=min2; j<max2; ++j)
            if(_ptrModele->getAnimalsMap(i,j) != NULL && _ptrModele->getAnimalsMap(i,j) != this && (_ptrModele->getAnimalsMap(i,j)->getDirX()!=0 || _ptrModele->getAnimalsMap(i,j)->getDirY()!=0)) // Il faut aussi que le voisin bouge
                ++nbVoisins;

    if(nbVoisins>0) // S'il y a plusieurs voisins qui appartiennent à une file, on va devoir en choisir un au hasard
    {
        // Choisit le gnou qu'on va suivre au hasard
        int voisinChoisit=rand()%nbVoisins;
        int tmp=-1;

        /*min1=_tileY-inVision;    // Détermine les bornes du voisinage qu'on va explorer
        max1=_tileY+inVision;
        min2=_tileX-inVision;
        max2=_tileX+inVision;

        if(min1<0) min1=0;
        if(max1>_ptrModele->getMapH()) max1=_ptrModele->getMapH();
        if(min2<0) min2=0;
        if(max2>_ptrModele->getMapW()) max2=_ptrModele->getMapW();*/

        for(i=min1; i<max1; ++i)    // Parcours le voisinage (du regard) et trouve quel est le gnou qu'on a choisit
            for(j=min2; j<max2; ++j)
                if(voisinChoisit==tmp)
                {
                    break;
                } else if(_ptrModele->getAnimalsMap(i,j) != NULL && _ptrModele->getAnimalsMap(i,j) != this && (_ptrModele->getAnimalsMap(i,j)->getDirX()!=0 || _ptrModele->getAnimalsMap(i,j)->getDirY()!=0)) // Il faut aussi que le voisin bouge
                {
                    _gnouQuonSuit =(gnou*)_ptrModele->getAnimalsMap(i,j);
                    ++tmp;
                }

        //_dansFile=true; // On est maintenant dans la file
    } qDebug() << nbVoisins << _gnouQuonSuit;
    //qDebug() << "fin choisir";
}

void gnou::calcNextMove()
{
    /*if( !this->isLeader())
    {
        if(predateurProche())
            this->strategy =new StrategieFuire();
        else if(groupeEnMigrationVisible())
            this->strategy =new StrategieSuivre();
        else
            this->strategy =new StrategieRechercheNourriture();
    } else {
        suivreParcours(); // Le leader suit le chemin (approximativement) de la migration
    }*/
}
