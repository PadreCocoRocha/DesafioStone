#include "player.h"
#include "spotifywrapper.h"
#include "clientkeys.h"
#include <QMediaPlayer>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>

Player::Player(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(1024,800);

    m_userOutput = new QTextEdit(this);
    m_userOutput->setGeometry(12,52,994,650);

    m_spotify = new SpotifyWrapper(clientId, clientSecret, this);

    connect(m_spotify, SIGNAL(infoOutput(QString)),
            this, SLOT(printInfo(QString)));

    m_searchInput = new QLineEdit(this);
    m_searchInput->setGeometry(12,12,934,30);
    m_searchInput->setPlaceholderText("Search for songs!");

    m_searchButton = new QPushButton("Search", this);
    m_searchButton->setGeometry(946,12,60,30);

    connect(m_searchButton, SIGNAL(clicked(bool)),
            this, SLOT(searchClicked()));

    // Temporary (maybe not)
    QPushButton *m_connectButton = new QPushButton("Connect", this);
    m_connectButton->setGeometry(12,710,994,20);

    connect(m_connectButton, SIGNAL(clicked(bool)),
            m_spotify, SLOT(grant()));

    QPushButton *m_clearButton = new QPushButton("ClearScreen", this);
    m_clearButton->setGeometry(12,730,994,20);

    connect(m_clearButton, SIGNAL(clicked(bool)),
            this, SLOT(clearOutput()));
    // end temp

    m_player = new QMediaPlayer(this);

//    connect(m_player, SIGNAL(positionChanged(qint64)),
//            parent, SLOT(positionChanged(qint64)));

//    m_player->setMedia(QUrl::fromLocalFile("/home/lucas/Music/2-10 O barquinho 1.mp3"));
//    m_player->setVolume(20);
//    m_player->play();
}


// public slots
void Player::searchClicked(){
    QString query = m_searchInput->text();
    if (query.isNull()) return;

    emit searchSpotify(query);
}

void Player::printInfo(QString message){
    m_userOutput->append(message);
}

void Player::clearOutput(){
    m_userOutput->clear();
}
