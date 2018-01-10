#include "player.h"
#include "clientkeys.h"
#include "spotifywrapper.h"
#include "searchbar.h"
#include "resultsbox.h"
#include "controller.h"
#include "playlistmodel.h"
#include "resultitem.h"
#include "playlistview.h"

#include <QStatusBar>
#include <QScrollArea>
#include <QMediaPlayer>
#include <QBoxLayout>
#include <QtNetwork>
#include <QJsonObject>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QMediaPlaylist>
#include <QListView>
#include <QSplitter>
#include <QKeyEvent>

Player::Player(QWidget *parent) :
    QWidget(parent)
{
// Outer container
    QBoxLayout *layout = new QVBoxLayout;

    m_searchBar = new SearchBar(this);
    m_searchBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

//  Service instances
    m_spotify = new SpotifyWrapper(clientId, clientSecret, this);
    m_player = new QMediaPlayer(this);

// Splitter containing results box and playlist view
    QSplitter *splitter = new QSplitter();

    m_resultsBox = new ResultsBox(this);

    QScrollArea *scrollAreaResults = new QScrollArea;
    scrollAreaResults->setBackgroundRole(QPalette::Base);
    scrollAreaResults->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaResults->setWidgetResizable(true);
    scrollAreaResults->setWidget(m_resultsBox);
    scrollAreaResults->setMinimumSize(300,200);

    m_playlist = new QMediaPlaylist();
    m_player->setPlaylist(m_playlist);

    connect(m_playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(playlistIndexChanged(int)));

    m_playlistModel = new PlaylistModel(m_playlist, this);

    m_playlistView = new PlaylistView();
    m_playlistView->setModel(m_playlistModel);
    m_playlistView->setCurrentIndex(
        m_playlistModel->index(m_playlist->currentIndex(), 0));
    m_playlistView->setMinimumSize(200,200);

    connect(m_playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));

    splitter->addWidget(scrollAreaResults);
    splitter->addWidget(m_playlistView);
// end Splitter

    Controller *m_controller = new Controller(m_player);

    connect(m_controller, SIGNAL(play()), m_player, SLOT(play()));
    connect(m_controller, SIGNAL(pause()), m_player, SLOT(pause()));
    connect(m_controller, SIGNAL(stop(bool)), m_player, SLOT(stop()));
    connect(m_controller, SIGNAL(previous()), m_playlist, SLOT(previous()));
    connect(m_controller, SIGNAL(next()), m_playlist, SLOT(next()));
    connect(m_controller, SIGNAL(setVolume(int)), m_player, SLOT(setVolume(int)));

    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            m_controller, SLOT(setState(QMediaPlayer::State)));
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(volumeChanged(int)), m_controller, SLOT(changeVolume(int)));



    m_player->setVolume(10);
    m_controller->setState(m_player->state());

    m_statusBar = new QStatusBar;

    layout->addWidget(m_searchBar);
    layout->addWidget(splitter);
    layout->addWidget(m_controller);
    layout->addWidget(m_statusBar);

    layout->setStretchFactor(splitter, 1);

    setLayout(layout);

    setPlayerReadyStatus(false);

    m_timer = new QTimer(this);

    m_counter = 14;
    connect(m_timer, &QTimer::timeout, this, [&](){
        if (m_counter == 0){
            m_statusBar->showMessage(
                "Authentication Failed! Try closing all open instances");
            m_timer->stop();
            return;
        }
        QString message = QString(
            "Starting Spotify authentication (Timeout: %1s)").arg(m_counter--);
        m_statusBar->showMessage(message);
    });

    m_statusBar->showMessage("Starting Spotify authentication (15s Timeout)");
    m_timer->start(1000);

    m_spotify->grant();
}

//public methods
SpotifyWrapper* Player::getSpotifyInstance(){
    return m_spotify;
}

SearchBar* Player::getSearchBarInstance(){
    return m_searchBar;
}

void Player::showMessage(QString msg){
    return m_statusBar->showMessage(msg);
}

// public slots
void Player::play(QUrl trackUrl){

    if (trackUrl.isEmpty()){
        m_statusBar->showMessage("No preview available for this song! =(");
        return;
    }

    m_player->setMedia(trackUrl);
    m_player->play();
}

void Player::searchClicked(){
    QString query = m_searchInput->text();

    if (query.isNull()) return;

    emit searchSpotify(query);
}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        // If resultItem play button is used, the playlist is discarded
        if (m_player->playlist() == NULL) m_player->setPlaylist(m_playlist);

        m_playlist->setCurrentIndex(index.row());
        m_player->play();
    }
}

void Player::connectionTimeout(){
    if (!playerReadyStatus){
        m_statusBar->showMessage(
            "Auth Timeout: close all running instances and try again!");
    }
}

void Player::mediaStatusChanged(QMediaPlayer::MediaStatus status){
    switch (status){
        case QMediaPlayer::BufferedMedia:
        case QMediaPlayer::LoadedMedia:
            showMessage("Done");
            break;
        case QMediaPlayer::NoMedia:
            showMessage("No media");
            break;
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::LoadingMedia:
            showMessage("Loading media...");
            break;
        case QMediaPlayer::InvalidMedia:
            showMessage("Invalid Media");
            break;
        case QMediaPlayer::EndOfMedia:
        case QMediaPlayer::StalledMedia:
        case QMediaPlayer::UnknownMediaStatus:
            showMessage(QString());
            break;
    }
}

void Player::addToPlaylist(ResultItem *item)
{
    m_playlistModel->addToPlaylist(item);
}

void Player::playlistIndexChanged(int currItem)
{
    m_playlistView->setCurrentIndex(m_playlistModel->index(currItem, 0));
}

// private methods
void Player::setPlayerReadyStatus(bool status){
    playerReadyStatus = status;

    if (status){
        m_timer->stop();
        m_searchBar->setEnabled(true);
    } else {
        m_searchBar->setEnabled(false);
    }
}
