#ifndef RESULTITEM_H
#define RESULTITEM_H

#include <QWidget>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QToolButton;
class QPushButton;
class Player;
class PlaylistItem;
QT_END_NAMESPACE

class ResultItem : public QWidget
{
    Q_OBJECT
public:
    explicit ResultItem(QString title, QUrl previewUrl, QString artist, QUrl albumPictureUrl = QUrl(),
                        QString albumName = QString(), QWidget *parent = nullptr, Player *player = nullptr);
    QString getTitle();
    QString getArtist();
    QUrl getPreviewUrl();
//    ~ResultItem();

signals:
    void play(QUrl trackUrl);
    void addSongToPlaylist(ResultItem *thisItem);

public slots:
    void playThisPressed();
    void addToPlaylistPressed();

private:
    QString m_title;
    QUrl m_previewUrl;
    QString m_artist;
    QUrl m_albumPictureUrl;
    QString m_albumName;

    Player *m_player;
    QToolButton *m_playButton;
    QToolButton *m_addButton;
};

#endif // RESULTITEM_H
