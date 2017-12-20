#ifndef RESULTITEM_H
#define RESULTITEM_H

#include <QWidget>
#include <QUrl>

QT_BEGIN_NAMESPACE
//class QUrl;
class QToolButton;
class QPushButton;
class Player;
QT_END_NAMESPACE

class ResultItem : public QWidget
{
    Q_OBJECT
public:
    explicit ResultItem(QString title, QUrl previewUrl, QString artist, QUrl albumPictureUrl,
                        QString albumName, QWidget *parent = nullptr, Player *player = nullptr);
    ~ResultItem();

signals:
    void playThis(QUrl trackUrl);

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
