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
class PlaylistBox;
QT_END_NAMESPACE

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    SpotifyWrapper* getSpotifyInstance();
    SearchBar* getSearchBarInstance();
    QStatusBar* getStatusBarInstance();

    void printInfo(QString message);

    enum class searchFilters {
        Tracks,
        Albums,
        Artists
    };

private:
    SpotifyWrapper *m_spotify;
    QMediaPlayer *m_player;
    SearchBar *m_searchBar;
    ResultsBox *m_resultsBox;
    PlaylistBox *m_playlistBox;
    Controller *m_controller;
    QStatusBar *m_statusBar;

    QVBoxLayout *m_containerLayout;

    QPushButton *m_searchButton;
    QLineEdit *m_searchInput;
    QTextEdit *m_searchBox;

    bool playerReadyStatus;

signals:
    void searchSpotify(QString query);

public slots:
    void play(QUrl track);
    void searchClicked();
    void setPlayerReadyStatus(bool status);
    void connectionTimeout();
};

#endif // PLAYER_H
