#include <QApplication>
#include <QWidget>
//#include "player.h"
#include "spotifywrapper.h"
#include "clientKeys.h"


int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QWidget window;

//    Player player(&window);

    SpotifyWrapper spotify(clientId, clientSecret), &window);

    spotify.grant();

    return app.exec();
}
