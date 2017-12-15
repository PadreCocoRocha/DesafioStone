#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class SpotifyWrapper;
class QMediaPlayer;
class QPushButton;
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);

private:
    QMediaPlayer *m_player;
    QPushButton *m_searchButton;
    QLineEdit *m_searchInput;
    QTextEdit *m_userOutput;
    SpotifyWrapper *m_spotify;

signals:
    void searchSpotify(QString query);

public slots:
    void printInfo(QString message);
    void searchClicked();
    void clearOutput();

};

#endif // PLAYER_H
