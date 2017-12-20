#include "searchbar.h"
#include "spotifywrapper.h"
#include "player.h"
#include <QtWidgets>

SearchBar::SearchBar(QWidget *parent, Player *player) :
    QWidget(parent)
{
    m_player = player;

    QHBoxLayout *hFilterLayout = new QHBoxLayout;

    m_artists = new QCheckBox("Artists");
    m_albums = new QCheckBox("Albums");
    m_tracks = new QCheckBox("Tracks");
    m_tracks->setChecked(true);
    m_tracks->setEnabled(false);

    hFilterLayout->addWidget(m_artists);
    hFilterLayout->addWidget(m_albums);
    hFilterLayout->addWidget(m_tracks);
    hFilterLayout->setAlignment(Qt::AlignLeft);
    hFilterLayout->stretch(0);

    QHBoxLayout *hSearchLayout = new QHBoxLayout;

    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText("Search for songs here!");

    m_searchButton = new QPushButton("Search", this);

    hSearchLayout->addWidget(m_searchBox);
    hSearchLayout->addWidget(m_searchButton);
    hSearchLayout->stretch(0);

    QVBoxLayout *vContainerLayout = new QVBoxLayout;
    vContainerLayout->addLayout(hFilterLayout);
    vContainerLayout->addLayout(hSearchLayout);
    vContainerLayout->stretch(0);

//    connect(m_tracks, SIGNAL(clicked(bool)), player, SLOT(changeFilters(bool, Player::searchFilters::Tracks)));
//    connect(m_albums, SIGNAL(clicked(bool)), player, SLOT(changeFilters(bool, Player::searchFilters::Albums)));
//    connect(m_artists, SIGNAL(clicked(bool)), player, SLOT(changeFilters(bool, Player::searchFilters::Artists)));

    connect(m_searchButton, SIGNAL(clicked(bool)), this, SLOT(searchTriggered()));
    connect(m_searchBox, SIGNAL(returnPressed()), this, SLOT(searchTriggered()));

    setLayout(vContainerLayout);
}



// public slots
void SearchBar::searchTriggered(){
    QString query = m_searchBox->text();
    emit triggerSpotifySearch(query, 0);
}

