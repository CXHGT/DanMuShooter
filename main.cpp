#include "GameWidget.h"
#include <QApplication>
#include "BilibiliGetCodeDialog.h"
#include "managers/GameSettings.h"
#include <iostream>

using namespace danMuGame;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *settings = GameSettings::GetInstance();
    settings->initialization();

    BilibiliGetCodeDialog bDialog;
    bDialog.setIsSaveCode(settings->getGameCodeIsSave());
    bDialog.setCodeText(settings->getGameCode());

    if (bDialog.exec() == QDialog::Accepted)
    {
        settings->setGameCode(bDialog.getCodeText());
        std::cout << bDialog.getCodeText() << " : " <<  bDialog.isSaveCode() << std::endl;
        settings->setGameCodeIsSave(bDialog.isSaveCode());
        settings->save();

        GameWidget w;
        w.show();
        return a.exec();
    }
    return 0;
}
