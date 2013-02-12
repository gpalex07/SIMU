#include "vue.h"

vue::vue(modele *inPtrModele)
    : _ptrModele(inPtrModele)
{
    // Récupère la taille de la zone d'affichage
    _width      = 1000;
    _height     = 800;

    // Initialisation diverses
    _tileOrigineX =  0;
    _tileOrigineY =  0;
    _tileW        = 40;
    _tileH        = 40;
    _menuW        = 150;
    _nbColonnes   = ceil((float)_width/(float)_tileW)+2;    // On affiche une colonne de marge à gauche/droite et haut/bas, d'où le +2
    _nbLignes     = ceil((float)_height/(float)_tileH)+2;
    _textures     = new QPixmap[NB_TEXTURES_HERBE+NB_TEXTURES_EAU+NB_TEXTURES_GNOUS];
    _iterDeplacement = 0;


    qDebug() << "Affichage de:" << _nbLignes << "et" << _nbColonnes << "colonnes";

    // Crée l'anneau, qui stocke les pointeurs vers chaque tile affichée à l'écran
    _anneau = new anneauQGraphicsItem(_nbLignes, _nbColonnes);

    // Création des Widgets de l'affichage
    _mainWindow = new QMainWindow();
    _view       = new QGraphicsView();
    _scene      = new QGraphicsSceneCustom();

    // Paramètre la fenêtre
    _mainWindow->setGeometry(50, 50, _width+_menuW, _height);

    // Paramètre la vue
    _view->setScene(_scene);
    _view->setParent(_mainWindow);
    _view->setGeometry(0,0,_width,_height);
    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // On cache les scrollbar
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setMouseTracking(true);
    _view->show();

    // Paramétrage de la scène
    _view->setSceneRect(_tileW,_tileH,_view->geometry().width(),_view->geometry().height()); // On n'affiche ni la première ni la dernière ligne/colonne
    //_scene->setSceneRect(0,0,_view->geometry().width()-2,_view->geometry().height()-2); // -2 car il y a une bordure de 1px

    // Initialise les layers
    _layerAnimaux       = new QGraphicsItemGroup();
    _layerEnvironnement = new QGraphicsItemGroup();
    _scene->addItem(_layerAnimaux);
    _scene->addItem(_layerEnvironnement);

    // Crée le tableau qui contiendra le pointeurs des textures d'animaux affichées
    _tabPtrPixmapAnimaux=new QGraphicsPixmapItem**[_nbLignes+2];    // 1 case de marge de chaque côté donc +2
    for (int i=0; i<_nbLignes+2; ++i)
    {
        _tabPtrPixmapAnimaux[i] = new QGraphicsPixmapItem*[_nbColonnes+2];
        for (int j=0; j<_nbColonnes+2; ++j)
            _tabPtrPixmapAnimaux[i][j] = NULL;
    }

    // Chargement de toutes les textures
    loadTextures();
    //_view->scale(0.8,0.8);

    // Initialise l'interface
    _btnEau  =NULL;
    _btnHerbe=NULL;
    setUpMenu();

    // Tout est prêt
    _mainWindow->show();
}

void vue::setUpMenu()
{
    // Crée le menu et l'affiche
    _btnEau = new QPushButton("Eau");
    _btnEau->setParent(_mainWindow);
    _btnEau->setGeometry(_width, 0, _menuW, 30);
    _btnEau->setStyleSheet("background:#4372c1;border:1px solid black");
    _btnEau->show();

    _btnHerbe = new QPushButton("Herbe");
    _btnHerbe->setParent(_mainWindow);
    _btnHerbe->setGeometry(_width, 29, _menuW, 30);
    _btnHerbe->setStyleSheet("background:#4ec44b;border:1px solid black");
    _btnHerbe->show();
}

// Dessine une tile à la position indiquée
// inTileX/inTileY: sont les coordonnées dans la matrice de la tile qu'on veut afficher
// inTextureId: est l'identifiant de la texture qu'on va charger dans la tile
void vue::drawTile(int inTileX, int inTileY)
{
    if(inTileX>=0 && inTileX<_ptrModele->getMapW() && inTileY>=0 && inTileY<_ptrModele->getMapH())
    {
        // On cherche à déduire la position de la tile dans le layer
        // Pour rappel, le layer se déplace à l'écran (quand on scrolle), donc l'origine
        // du calque ne coincide pas avec l'angle supérieur gauche de l'affichage
        /*int x = inTileX-_tileOrigineX;
        int y = inTileY-_tileOrigineY;*/


        int valeurTile =(*_ptrModele)[inTileY][inTileX];

        // Crée la tile et l'ajoute au calque qui contient toutes les tiles de l'environnement
        QGraphicsPixmapItem * tile = _scene->addPixmap(_textures[valeurTile]);
        _layerEnvironnement->addToGroup(tile);
        //tile->setPos(x*_tileW, y*_tileH);
        tile->setPos(inTileX*_tileW, inTileY*_tileH);
        tile->show();

        // Ajoute la tile dans l'anneau, ainsi on sauvegarde son pointeur et on pourra la supprimer par la suite
        _anneau->insererTile(tile, inTileX, inTileY, _tileOrigineX, _tileOrigineY);
    } else qDebug() << "Error: vue::drawTile() > On nous demande d'afficher une tile qui n'existe pas dans la map.\nVerifier que la zone d'affichage demandee n'est pas superieure a la taille de la map.";

}

void vue::reDrawTile(int inTileX, int inTileY)
{
    removeTile(inTileX, inTileY);

    drawTile(inTileX, inTileY);
}

// Dessine la ligne demandée (le numéro de ligne est le numéro de la ligne, dans la matrice)
void vue::drawLine(int inLineNum)
{
    /*if(inLineNum-_tileOrigineY >= _nbLignes)
        qDebug() << "Warning: vue::drawLine() > On dessine une ligne en dehors de l'espace visible par l'utilisateur (suspect)";*/

    for(int i=_tileOrigineX; i<_tileOrigineX+_nbColonnes; ++i)
    {
        drawTile(i, inLineNum);
    }
}

// Dessine la colonne demandée (le numéro de colonne est le numéro de la colonne, dans la matrice)
void vue::drawColumn(int inColumnNum)
{
    /*if(inColumnNum-_tileOrigineX >= _nbColonnes)
        qDebug() << "Warning: vue::drawColumn() > On dessine une colonne en dehors de l'espace visible par l'utilisateur (suspect)";*/

    for(int i=_tileOrigineY; i<_tileOrigineY+_nbLignes; ++i)
    {
        drawTile(inColumnNum, i);
    }
}

void vue::drawMap(int inTileOrigineX, int inTileOrigineY)
{
    // Point supérieur gauche de l'affichage
    _tileOrigineX =inTileOrigineX;
    _tileOrigineY =inTileOrigineY;

    // Affiche la map
    for(int i=0;i<_nbLignes;++i)
        drawLine(i);
}

// Efface une tile ; inTileX et inTileY sont les coordonnées de la tile dans la matrice de la map
// (ce ne sont pas les coordonnées à l'écran donc)
void vue::removeTile(int inTileX, int inTileY)
{
    // Récupère la position de la tile dans l'anneau
    int x =_anneau->getTileColumn(inTileX, _tileOrigineX);
    int y =_anneau->getTileLine(inTileY, _tileOrigineY);

    if((*_anneau)[y] != NULL && ((*_anneau)[y][x]) != NULL)
    {
        delete (*_anneau)[y][x];
        (*_anneau)[y][x] =NULL;
    } else
        qDebug() << "Erreur: vue::removeTile() > tentative de suppression d'une tile inexistante (probablement deja supprimee)";
}

// Suppression de toutes les tiles d'une ligne affichée à l'écran
// inLineNum est le numéro de la ligne dans la matrice de la map
void vue::removeLine(int inLineNum)
{
    int end = _tileOrigineX+_nbColonnes;

    for(int i=_tileOrigineX; i<end; ++i)
        removeTile(i, inLineNum);

}

// Suppression de toutes les tiles d'une colonne affichée à l'écran
// inColumnNum est le numéro de la colonne dans la matrice de la map
void vue::removeColumn(int inColumnNum)
{
    int end = _tileOrigineY+_nbLignes;

    for(int i=_tileOrigineY; i<end; ++i)
        removeTile(inColumnNum, i);

    // Met à jour les pointeurs de début/fin de l'anneau
    //_anneau->columnRemoved(inColumnNum, _tileOrigineX);
}

// Supprime toutes les tiles affichées, et recrée des calques, vides
void vue::cleanMap()
{
    /*if(_layerAnimaux != NULL)
    {
        delete _layerAnimaux;
        _layerAnimaux =new QGraphicsItemGroup();
        _scene->addItem(_layerAnimaux);
    }*/

    if(_layerEnvironnement != NULL)
    {
        delete _layerEnvironnement;
        _layerEnvironnement =new QGraphicsItemGroup();
        _scene->addItem(_layerEnvironnement);
    }
}

void vue::cleanAllAnimals()
{
    if(_layerAnimaux != NULL)
    {
        // Sauvegarde la position de l'ancien calque (mémorise sa position de scroll, sinon bug)
        int x=_layerAnimaux->x();
        int y=_layerAnimaux->y();

        delete _layerAnimaux;
        _layerAnimaux =new QGraphicsItemGroup();
        _layerAnimaux->setPos(x,y);
        _scene->addItem(_layerAnimaux);

        // Remet à zéro le tableau des pointeurs
        for (int i=0; i<_nbLignes+2; ++i)
        {
            for (int j=0; j<_nbColonnes+2; ++j)
                _tabPtrPixmapAnimaux[i][j] = NULL;
        }
    }
}

void vue::drawAnimal(animal *inAnimal, int inIterDeplacement)
{
    //if(inAnimal->getTileX()>=0 && inAnimal->getTileX()<_ptrModele->getMapW() && inAnimal->getTileY()>=0 && inAnimal->getTileY()<_ptrModele->getMapH())
    {
        // On cherche à déduire la position de l'animal dans le layer
        // Pour rappel, le layer se déplace à l'écran (quand on scrolle), donc l'origine
        // du calque ne coincide pas avec l'angle supérieur gauche de l'affichage
        /*int x = inAnimal->getTileX()-_tileOrigineX;
        int y = inAnimal->getTileY()-_tileOrigineY;*/

        if(inIterDeplacement!=-1)
            _iterDeplacement=inIterDeplacement;


        int textureAnimal =inAnimal->getTextureNumber();

        /*int deplacementX=_iterDeplacement *((inAnimal->getNextTileX()>inAnimal->getTileX())?1:(inAnimal->getNextTileX()<inAnimal->getTileX())?-1:0);
        int deplacementY=_iterDeplacement *((inAnimal->getNextTileY()>inAnimal->getTileY())?1:(inAnimal->getNextTileY()<inAnimal->getTileY())?-1:0);*/

        int deplacementX=_iterDeplacement *inAnimal->getDirX();
        int deplacementY=_iterDeplacement *inAnimal->getDirY();

        // Crée la tile et l'ajoute au calque qui contient toutes les tiles de l'environnement
        QGraphicsPixmapItem * tile = _scene->addPixmap(_textures[textureAnimal]);
        _layerAnimaux->addToGroup(tile);
        _vectPixmapGnouDansScene[inAnimal] =tile; // Sauvegarde le pointeur sur le pixmap correspondant à cet animal dans la scène

        // Calcule les coordonnées à l'écran
        int sX=inAnimal->getNextTileX()-_tileOrigineX;
        int sY=inAnimal->getNextTileY()-_tileOrigineY;

        //if(sX>=0 && sX<_nbColonnes && sY>=0 && sY<_nbLignes)
            _tabPtrPixmapAnimaux[sY][sX]=tile;
        //else qDebug() << "L'animal est en dehors de l'ecran";

        //qDebug() << inAnimal->getNextTileY()-_tileOrigineY << inAnimal->getNextTileX()-_tileOrigineX << "," << _nbColonnes << _nbLignes;
        /*if(inAnimal->getNextTileY()-_tileOrigineY<0 || inAnimal->getNextTileY()-_tileOrigineY>_nbLignes)
            qDebug() << "Y=" << inAnimal->getNextTileY()-_tileOrigineY;
        if(inAnimal->getNextTileX()-_tileOrigineX<0 || inAnimal->getNextTileX()-_tileOrigineX>_nbColonnes)
            qDebug() << "X=" << inAnimal->getNextTileX()-_tileOrigineX;*/
        //_tabPtrPixmapAnimaux[inAnimal->getNextTileY()-_tileOrigineY+1][inAnimal->getNextTileX()-_tileOrigineX+1] = tile;


        //tile->setPos(x*_tileW, y*_tileH);
        tile->setPos(inAnimal->getTileX()*_tileW+deplacementX, inAnimal->getTileY()*_tileH+deplacementY);
        tile->show();

        // Ajoute la tile dans l'anneau, ainsi on sauvegarde son pointeur et on pourra la supprimer par la suite
        //_anneau->insererTile(tile, inAnimal->getTileX(), inAnimal->getTileY(), _tileOrigineX, _tileOrigineY);
    }// else qDebug() << "Error: vue::drawAnimal() > On nous demande d'afficher un animal qui est en dehors de la map.";

}

void vue::reDrawAnimal(animal *inAnimal)
{
    removeAnimal(inAnimal);
    drawAnimal(inAnimal);
}

void vue::removeAnimal(animal *inAnimal)
{
    // Calcule les coordonnées à l'écran
    int sX=inAnimal->getNextTileX()-_tileOrigineX;
    int sY=inAnimal->getNextTileY()-_tileOrigineY;
    /*int sX=inAnimal->getTileX()-_tileOrigineX+inAnimal->getDirX();
    int sY=inAnimal->getTileY()-_tileOrigineY+inAnimal->getDirY();*/

    // Regarde si on a bien supprimé la texture de cet animal sur la case où il était avant
    /*int sXX=inAnimal->getLastTileX()-_tileOrigineX;
    int sYY=inAnimal->getLastTileY()-_tileOrigineY;*/

    if(!(sX>=0 && sX<_nbColonnes && sY>=0 && sY<_nbLignes))
        qDebug() << "En dehors de l'ecran" << sX<<sY<<_nbColonnes<<_nbLignes;

    /*if(sX>=0 && sX<_nbColonnes && sY>=0 && sY<_nbLignes && _tabPtrPixmapAnimaux[sY][sX]!=NULL)
    {*/
        delete _tabPtrPixmapAnimaux[sY][sX];
        _tabPtrPixmapAnimaux[sY][sX]=NULL;
    //}// else qDebug() << "On ne supprime pas" << _tabPtrPixmapAnimaux[sY][sX];

    /*if(_tabPtrPixmapAnimaux[sYY][sXX]==inAnimal)
    {
        delete _tabPtrPixmapAnimaux[sYY][sXX];
        _tabPtrPixmapAnimaux[sYY][sXX]=NULL;
    }*/


    /*std::map<animal*,QGraphicsPixmapItem*>::iterator it =_vectPixmapGnouDansScene.find(inAnimal);
    if(it != _vectPixmapGnouDansScene.end())
    {
        delete (*it).second; // Suppression du QGraphicsItemPixmap associé à inAnimal
        _vectPixmapGnouDansScene.erase(it);
    }*/// else qDebug() << "Warning: vue::removeAnimal() > L'animal a supprimer n'a pas ete trouve";
}

// Dessine tous les animaux visibles dans la fenêtre
void vue::drawAllAnimals(int inTileOrigineX, int inTileOrigineY)
{
    // Met à jour l'origine
    _tileOrigineX =inTileOrigineX;
    _tileOrigineY =inTileOrigineY;


    for(int i=_tileOrigineY; i<_tileOrigineY+_nbLignes; ++i)
    {
        for(int j=_tileOrigineX; j<_tileOrigineX+_nbColonnes; ++j)
        {
            if(_ptrModele->getAnimalsMap(j,i) != NULL)  // S'il y a un animal
            {
                drawAnimal(_ptrModele->getAnimalsMap(j,i));
            }
        }
    }
}

void vue::redrawAllAnimals2(int inTileOrigineX, int inTileOrigineY, int inIterDeplacement)
{
    // Place le calque des animaux tout en haut (par dessus la map donc). La valeur par défaut étant 0.
    _layerAnimaux->setZValue(10);

    // Met à jour l'origine
    _tileOrigineX =inTileOrigineX;
    _tileOrigineY =inTileOrigineY;
    _iterDeplacement = inIterDeplacement;

    //int k=0;

    for(int i=_tileOrigineY; i<_tileOrigineY+_nbLignes; ++i)
    {
        for(int j=_tileOrigineX; j<_tileOrigineX+_nbColonnes; ++j)
        {
            if(_ptrModele->getAnimalsMap(j,i) != NULL)  // S'il y a un animal
            {
                reDrawAnimal2(_ptrModele->getAnimalsMap(j,i));
                //++k;
            }
        }
    }
}

void vue::reDrawAnimal2(animal *inAnimal)
{
    removeAnimal2(inAnimal);
    drawAnimal2(inAnimal);
}

void vue::removeAnimal2(animal *inAnimal)
{
    int sX, sY;

    if(_iterDeplacement==0) // La position du gnou vient de changer (il vient de passer sur une nouvelle case). Sa texture à supprimer est donc sur l'ancienne case.
    {
        sX=inAnimal->getTileX() -_tileOrigineX -inAnimal->getDirX();
        sY=inAnimal->getTileY() -_tileOrigineY -inAnimal->getDirY();
    } else { // Cas classique
        sX=inAnimal->getTileX() -_tileOrigineX;
        sY=inAnimal->getTileY() -_tileOrigineY;
    }

    if(!(sX>=0 && sX<_nbColonnes && sY>=0 && sY<_nbLignes))
        qDebug() << "En dehors de l'ecran" << sX<<sY<<_nbColonnes<<_nbLignes;

    delete _tabPtrPixmapAnimaux[sY][sX];
    _tabPtrPixmapAnimaux[sY][sX]=NULL;
}

void vue::drawAnimal2(animal *inAnimal, int inIterDeplacement)
{

        if(inIterDeplacement!=-1)
            _iterDeplacement=inIterDeplacement;

        int textureAnimal =inAnimal->getTextureNumber();
        int deplacementX=_iterDeplacement *inAnimal->getDirX();
        int deplacementY=_iterDeplacement *inAnimal->getDirY();

        // Crée la tile et l'ajoute au calque qui contient toutes les tiles de l'environnement
        QGraphicsPixmapItem * tile = _scene->addPixmap(_textures[textureAnimal]);
        _layerAnimaux->addToGroup(tile);

        int sX =inAnimal->getTileX()-_tileOrigineX;
        int sY =inAnimal->getTileY()-_tileOrigineY;

        //qDebug() << sX << sY;

        if(sX>=0 && sX<_nbColonnes && sY>=0 && sY<_nbLignes)
            _tabPtrPixmapAnimaux[sY][sX]=tile;
        else qDebug() << "L'animal est en dehors de l'ecran";


        //tile->setPos(x*_tileW, y*_tileH);
        tile->setPos(inAnimal->getTileX()*_tileW+deplacementX, inAnimal->getTileY()*_tileH+deplacementY);
        tile->show();

}





void vue::redrawAllAnimals(int inTileOrigineX, int inTileOrigineY, int inIterDeplacement)
{
    /*cleanAllAnimals();
    drawAllAnimals(_tileOrigineX, _tileOrigineY);*/
    //cleanAllAnimals();

    // Place le calque des animaux tout en haut (par dessus la map donc). La valeur par défaut étant 0.
    _layerAnimaux->setZValue(10);

    // Met à jour l'origine
    _tileOrigineX =inTileOrigineX;
    _tileOrigineY =inTileOrigineY;
    _iterDeplacement = inIterDeplacement;

    //int k=0;

    for(int i=_tileOrigineY; i<_tileOrigineY+_nbLignes; ++i)
    {
        for(int j=_tileOrigineX; j<_tileOrigineX+_nbColonnes; ++j)
        {
            if(_ptrModele->getAnimalsMap(j,i) != NULL)  // S'il y a un animal
            {
                reDrawAnimal(_ptrModele->getAnimalsMap(j,i));
                //++k;
            }
        }
    }

    //qDebug() << "On affiche" << k << "animaux";
}

// Lorsqu'on scroll, des animaux qui étaient affichés vont passer hors de la zone d'affichage,
// il faut donc supprimer leurs textures
void vue::removeAnimalLine(int inLineNum)
{
    for(int i=_tileOrigineX; i<_tileOrigineX+_nbColonnes; ++i)
        removeAnimal(_ptrModele->getAnimalsMap(i, inLineNum));
}

// Lorsqu'on scroll, des animaux qui étaient affichés vont passer hors de la zone d'affichage,
// il faut donc supprimer leurs textures
void vue::removeAnimalColumn(int inColumnNum)
{
    animal * animal=NULL;
    for(int i=_tileOrigineY; i<_tileOrigineY+_nbLignes; ++i)
    {
        animal=_ptrModele->getAnimalsMap(inColumnNum, i);

        if(animal!=NULL)
            removeAnimal(animal);
    }
}

void vue::scrollRight()
{
    // On supprime la colonne la plus à gauche (qui va sortir de l'écran à la suite du scroll)
    // puis on crée une nouvelle colonne à droite (elle va apparaître à la suite du scroll)
    removeColumn(_tileOrigineX);
    drawColumn(_tileOrigineX+_nbColonnes);
    cleanAllAnimals();
    ++_tileOrigineX;


    // On décale les calques pour décaler toutes les autres colonnes/lignes
    _layerAnimaux->setPos(_layerAnimaux->x()-_tileW, _layerAnimaux->y());
    _layerEnvironnement->setPos(_layerEnvironnement->x()-_tileW, _layerEnvironnement->y());
}

void vue::scrollLeft()
{
    // On supprime la colonne la plus à gauche (qui va sortir de l'écran à la suite du scroll)
    // puis on crée une nouvelle colonne à droite (elle va apparaître à la suite du scroll)
    --_tileOrigineX;
    removeColumn(_tileOrigineX+_nbColonnes);
    drawColumn(_tileOrigineX);
    cleanAllAnimals();

    // On décale les calques pour décaler toutes les autres colonnes/lignes
    _layerAnimaux->setPos(_layerAnimaux->x()+_tileW, _layerAnimaux->y());
    _layerEnvironnement->setPos(_layerEnvironnement->x()+_tileW, _layerEnvironnement->y());
}

void vue::scrollUp()
{
    // On supprime la colonne la plus à gauche (qui va sortir de l'écran à la suite du scroll)
    // puis on crée une nouvelle colonne à droite (elle va apparaître à la suite du scroll)
    --_tileOrigineY;
    removeLine(_tileOrigineY+_nbLignes);
    drawLine(_tileOrigineY);
    cleanAllAnimals();

    // On décale les calques pour décaler toutes les autres colonnes/lignes
    _layerAnimaux->setPos(_layerAnimaux->x(), _layerAnimaux->y()+_tileH);
    _layerEnvironnement->setPos(_layerEnvironnement->x(), _layerEnvironnement->y()+_tileH);
    //removeAnimalLine(_tileOrigineY+_nbLignes);
}

void vue::scrollDown()
{
    // On supprime la colonne la plus à gauche (qui va sortir de l'écran à la suite du scroll)
    // puis on crée une nouvelle colonne à droite (elle va apparaître à la suite du scroll)
    removeLine(_tileOrigineY);
    drawLine(_tileOrigineY+_nbLignes);
    cleanAllAnimals();
    //removeAnimalLine(_tileOrigineY);
    ++_tileOrigineY;

    // On décale les calques pour décaler toutes les autres colonnes/lignes
    _layerAnimaux->setPos(_layerAnimaux->x(), _layerAnimaux->y()-_tileH);
    _layerEnvironnement->setPos(_layerEnvironnement->x(), _layerEnvironnement->y()-_tileH);
}



// Charge les textures dans un tableau, pour avoir un cache des images, gardé en mémoire
void vue::loadTextures()
{
    int i, cpt =0;
    QString path;

    // Textures d'herbe
    for(i=DEBUT_TEXTURES_HERBE; i< DEBUT_TEXTURES_HERBE +NB_TEXTURES_HERBE; ++i)
    {
        path="TEXTURES/HERBE/";
        path.append(QString::number(cpt));
        path.append(".png");
        ++cpt;
        _textures[i].load(path);

        if(_textures[i].isNull())
            qDebug() << "Erreur: vue::loadTextures() > la texture" << path << "n'a pas ete chargee\nVerifier la presence du dossier TEXTURES/HERBE";
    }

    // Textures d'eau
    cpt =1;
    for(i=DEBUT_TEXTURES_EAU; i< DEBUT_TEXTURES_EAU +NB_TEXTURES_EAU; ++i)
    {
        path="TEXTURES/EAU/";
        path.append(QString::number(cpt));
        path.append(".png");
        ++cpt;
        _textures[i].load(path);

        if(_textures[i].isNull())
            qDebug() << "Erreur:  vue::loadTextures() > Erreur: la texture" << path << "n'a pas ete chargee\nVerifier la presence du dossier TEXTURES/EAU";
    }

    // Textures de gnous
    cpt=0;
    for(i=DEBUT_TEXTURES_GNOUS; i< DEBUT_TEXTURES_GNOUS +NB_TEXTURES_GNOUS; ++i)
    {
        path="TEXTURES/GNOUS/";
        path.append(QString::number(cpt));
        path.append(".png");
        ++cpt;
        _textures[i].load(path);

        if(_textures[i].isNull())
            qDebug() << "Erreur:  vue::loadTextures() > Erreur: la texture" << path << "n'a pas ete chargee\nVerifier la presence du dossier TEXTURES/GNOUS";
    }


}
