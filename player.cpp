#include "player.h"
#include "clientkeys.h"
#include "spotifywrapper.h"
#include "searchbar.h"
#include "resultsbox.h"
#include "controller.h"
#include "playlistbox.h"

#include <QStatusBar>
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

    QScrollArea *scrollAreaResults = new QScrollArea;
    scrollAreaResults->setBackgroundRole(QPalette::Base);
    scrollAreaResults->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaResults->setWidgetResizable(true);
    scrollAreaResults->setWidget(m_resultsBox);
    scrollAreaResults->setMinimumSize(300,200);

    m_playlistBox = new PlaylistBox(parent, this);

    QScrollArea *scrollAreaPlaylist = new QScrollArea;
    scrollAreaPlaylist->setBackgroundRole(QPalette::Base);
    scrollAreaPlaylist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaPlaylist->setWidgetResizable(true);
    scrollAreaPlaylist->setWidget(m_playlistBox);
    scrollAreaPlaylist->setMinimumSize(100,200);

    vMainLayout->addWidget(scrollAreaResults);
    vMainLayout->addWidget(scrollAreaPlaylist);

    vMainLayout->setStretchFactor(scrollAreaResults, 3);
    vMainLayout->setStretchFactor(scrollAreaPlaylist, 1);

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

    m_statusBar = new QStatusBar;

    vContainerLayout->addWidget(m_searchBar);
    vContainerLayout->addLayout(vMainLayout);
    vContainerLayout->addWidget(m_controller);
    vContainerLayout->addWidget(m_statusBar);

    setLayout(vContainerLayout);

    setPlayerReadyStatus(false);

    QTimer::singleShot(15000, this, SLOT(connectionTimeout()));

    m_statusBar->showMessage("Starting Spotify Auth (10sec Timeout)");
    m_spotify->grant();

}

//public methods
SpotifyWrapper* Player::getSpotifyInstance(){
    return m_spotify;
}

SearchBar* Player::getSearchBarInstance(){
    return m_searchBar;
}

QStatusBar* Player::getStatusBarInstance(){
    return m_statusBar;
}

void Player::printInfo(QString message){
    QTextStream console(stdout);
    console << message << endl;
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

void Player::connectionTimeout(){
    if (!playerReadyStatus){
        m_statusBar->showMessage(
            "Auth Timeout: close all running instances and try again!");
    }
}

// private methods
void Player::setPlayerReadyStatus(bool status){
    playerReadyStatus = status;

    if (status){
        m_searchBar->setEnabled(true);
    } else {
        m_searchBar->setEnabled(false);
    }
}
