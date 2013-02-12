#include <QDebug>
#include <QObject>
#include <QApplication>

#include "vue/vue.h"
#include "controleur.h"
#include "modele/modele.h"


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    modele modeleSimu(50,50);
    vue vueSimu(&modeleSimu);
    controleur controleurSimu;

    QObject::connect(vueSimu.getPtrScene(), SIGNAL(keyPressed(QKeyEvent*)), &controleurSimu, SLOT(keyEventManager(QKeyEvent*)));

    controleurSimu.runSimu(&modeleSimu, &vueSimu);


    return app.exec();
}
