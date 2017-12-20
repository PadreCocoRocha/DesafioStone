#include "player.h"
#include "spotifywrapper.h"
#include "clientkeys.h"
#include "searchbar.h"
#include "resultsbox.h"
#include "controller.h"

#include <QScrollArea>
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

    QScrollArea *m_scrollArea = new QScrollArea;
    m_scrollArea->setBackgroundRole(QPalette::AlternateBase);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_resultsBox);
    m_scrollArea->setMinimumSize(200,200);

    vMainLayout->addWidget(m_scrollArea);
//    vMainLayout->addWidget(m_playlistBox);

    Controller *m_controller = new Controller(this);

    connect(m_controller, SIGNAL(play()), m_player, SLOT(play()));
    connect(m_controller, SIGNAL(pause()), m_player, SLOT(pause()));
    connect(m_controller, SIGNAL(stop(bool)), m_player, SLOT(stop()));
    connect(m_controller, SIGNAL(setVolume(int)), m_player, SLOT(setVolume(int)));

    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            m_controller, SLOT(setState(QMediaPlayer::State)));
    connect(m_player, SIGNAL(volumeChanged(int)),
            m_controller, SLOT(changeVolume(int)));

    m_player->setVolume(10);
    m_controller->setState(m_player->state());

    vContainerLayout->addWidget(m_searchBar);
    vContainerLayout->addLayout(vMainLayout);
    vContainerLayout->addWidget(m_controller);

    setLayout(vContainerLayout);

    // Make authentication steps
    m_spotify->grant();

}

//public methods
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
    m_player->play();
}


void Player::searchClicked(){
    QString query = m_searchInput->text();
    if (query.isNull()) return;

    emit searchSpotify(query);
}
