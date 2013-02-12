#include "qpushbuttoncustom.h"

qpushbuttoncustom::qpushbuttoncustom(int inBtnType, QString inNom)
    : _btnType(inBtnType)
{

    this->setText(inNom);
}

// Ceci nous permet de savoir quel bouton à été cliqué
void qpushbuttoncustom::buttonClicked()
{
    emit emitButtonClicked(_btnType);
}
