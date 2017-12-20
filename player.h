#ifndef PLAYER_H
#define PLAYER_H

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
QT_END_NAMESPACE

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    SpotifyWrapper* getSpotifyInstance();
    SearchBar* getSearchBarInstance();

    void printInfo(QString message);
    void updateLayout();

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
    Controller *m_controller;

    QVBoxLayout *m_containerLayout;

    QPushButton *m_searchButton;
    QLineEdit *m_searchInput;
    QTextEdit *m_searchBox;

signals:
    void searchSpotify(QString query);

public slots:
    void play(QUrl track);

    void searchClicked();
};

#endif // PLAYER_H
