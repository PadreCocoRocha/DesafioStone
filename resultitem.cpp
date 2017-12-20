#include "resultitem.h"
#include "player.h"
#include <QtWidgets>

ResultItem::ResultItem(QString title, QUrl previewUrl, QString artist, QUrl albumPictureUrl,
                       QString albumName, QWidget *parent, Player *player) :
    QWidget(parent)
{
    m_player = player;
    m_parent = parent;

    m_title = title;
    m_previewUrl = previewUrl;
    m_artist = artist;
    m_albumPictureUrl = albumPictureUrl;
    m_albumName = albumName;

    QHBoxLayout *vLayout = new QHBoxLayout;

    QLabel *txtLine = new QLabel(title, parent);

    QToolButton *m_playButton = new QToolButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    vLayout->addWidget(m_playButton);
    vLayout->addWidget(txtLine);

    setLayout(vLayout);

    connect(m_playButton, SIGNAL(clicked(bool)), this, SLOT(playThisPressed()));
    connect(this, SIGNAL(playThis(QUrl)), m_player, SLOT(play(QUrl)));
}


ResultItem::~ResultItem(){
    QTextStream out(stdout);
    out << "ResultItem " << m_title << " has been destroyed" << endl;
}

void ResultItem::playThisPressed(){
    emit playThis(m_previewUrl);
}
