#ifndef GNOU_H
#define GNOU_H

#include "animal.h"

class gnou : public animal
{
private:
    gnou * _gnouQuonSuit; // En toute rigueur, on suit un gnou (pas un animal), mais pour des raisons techniques, on doit mettre animal. Ca ne change rien.
    int flag;

public:
    gnou(int inTileX, int inTileY, modele * inPtrModele);
    int getTextureNumber() const;
    bool verifierQuOnSuitLeader();
    void moveTo(int inObjX, int inObjY);
    void choisirQuiOnsuit(int inVision);
    bool suitUnGnou(){ return _gnouQuonSuit!=NULL; }
    void suivreGnouQuonSuit();
    void calcNextMove();
};

#endif // GNOU_H
