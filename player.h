#ifndef PLAYER_H
#define PLAYER_H

#include "spotifywrapper.h"
#include <QWidget>
#include <QJsonObject>
#include <QUrl>

QT_BEGIN_NAMESPACE
class SpotifyWrapper;
class ResultsBox;
class SearchBar;
class QMediaPlayer;
class QPushButton;
class QLineEdit;
class QTextEdit;
class QVBoxLayout;
class Controller;
class QStatusBar;
class QMediaPlaylist;
class ResultItem;
class PlaylistModel;
class QListView;
class QModelIndex;
//class QKeyEvent;
QT_END_NAMESPACE

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);

    SpotifyWrapper* getSpotifyInstance();
    SearchBar* getSearchBarInstance();
    void showMessage(QString msg);

    enum class searchFilters {
        Tracks = 0,
        Albums,
        Artists
    };

private:
    SpotifyWrapper *m_spotify;
    QMediaPlayer *m_player;
    SearchBar *m_searchBar;
    ResultsBox *m_resultsBox;
    QMediaPlaylist *m_playlist;
    PlaylistModel *m_playlistModel;
    Controller *m_controller;
    QStatusBar *m_statusBar;
    QListView *m_playlistView;

    QVBoxLayout *m_containerLayout;

    QPushButton *m_searchButton;
    QLineEdit *m_searchInput;
    QTextEdit *m_searchBox;

    bool playerReadyStatus;

signals:
    void searchSpotify(QString query);
//    void deleteTrack(QList<QModelIndex> items);

public slots:
    void play(QUrl track);
    void searchClicked();
    void jump(const QModelIndex &index);
    void setPlayerReadyStatus(bool status);
    void connectionTimeout();
    void addToPlaylist(ResultItem* item);
    void playlistIndexChanged(int currItem);
//    void keyPressEvent(QKeyEvent *event);



};

#endif // PLAYER_H
