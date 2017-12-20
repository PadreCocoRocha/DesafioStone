#ifndef PLAYLISTBOX_H
#define PLAYLISTBOX_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Player;
class QVBoxLayout;
QT_END_NAMESPACE

class PlaylistBox : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistBox(QWidget *parent = nullptr, Player *player = nullptr);

signals:

public slots:

private:
    Player *m_player;
    QVBoxLayout *m_container;

};

#endif // PLAYLISTBOX_H
