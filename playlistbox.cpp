#include "playlistbox.h"

#include <QtWidgets>

PlaylistBox::PlaylistBox(QWidget *parent, Player *player) :
    QWidget(parent)
{
    m_player = player;

    m_container = new QVBoxLayout(this);

    setLayout(m_container);
}
