#include "player.h"
#include "spotifywrapper.h"
#include "clientkeys.h"
#include "searchbar.h"
#include "resultsbox.h"

#include <QMediaPlayer>
#include <QBoxLayout>
#include <QtNetwork>
#include <QJsonObject>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

Player::Player(QWidget *parent) :
    QWidget(parent)
{
    // Outer container
    QVBoxLayout *vContainerLayout = new QVBoxLayout;
    m_containerLayout = vContainerLayout;

    m_searchBar = new SearchBar(parent, this);

    //  Service instances
    m_spotify = new SpotifyWrapper(clientId, clientSecret, this);
    m_player = new QMediaPlayer(this);

    QHBoxLayout *vMainLayout = new QHBoxLayout;

    m_resultsBox = new ResultsBox(parent, this);
    vMainLayout->addWidget(m_resultsBox);

    QPushButton *m_clearButton = new QPushButton("ClearScreen", this);

    vMainLayout->addWidget(m_clearButton);

    vContainerLayout->addWidget(m_searchBar);
    vContainerLayout->addLayout(vMainLayout);

    connect(m_clearButton, SIGNAL(clicked(bool)), m_resultsBox, SLOT(clearResults()));

    setLayout(vContainerLayout);

    m_spotify->grant();

}

SpotifyWrapper* Player::getSpotifyInstance(){
    return m_spotify;
}

SearchBar* Player::getSearchBarInstance(){
    return m_searchBar;
}

void Player::printInfo(QString message){
    QTextStream console(stdout);
    console << message << endl;
}

void Player::updateLayout(){
    m_containerLayout->update();
}

// public slots
void Player::play(QUrl trackUrl){

    if (trackUrl.isEmpty()){
        printInfo("No preview available for this song! =(");
        return;
    }

    printInfo(trackUrl.toString());
    m_player->setMedia(trackUrl);
    m_player->setVolume(20);
    m_player->play();
}


void Player::searchClicked(){
    QString query = m_searchInput->text();
    if (query.isNull()) return;

    emit searchSpotify(query);
}
