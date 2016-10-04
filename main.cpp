#include "GamePlay.h"
#include <QFile>

#define PROGNAME "GameBalls"

int main(int argc, char *argv[])
{
    QString fileName;

    if (argc > 1)
        fileName = argv[1];
    else
        printf("%s: bad command line argument\nUsage:\nGameBalls INPUTFILENAME\n", PROGNAME);

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            printf("%s: Error open file: %s", PROGNAME, fileName.toLocal8Bit().constData());
            return -1;
        }

        QByteArray inputOptions = file.readAll();

        file.close();
        GamePlay game;

        if (game.setInputOptions(inputOptions))
            game.play();
        else
            printf("%s: incorrect input data\n", PROGNAME);
    }

    return 0;
}
