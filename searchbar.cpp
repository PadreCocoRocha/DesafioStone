#include "searchbar.h"
#include "spotifywrapper.h"
#include "player.h"
#include <QtWidgets>

SearchBar::SearchBar(Player *player, QWidget *parent) :
    QWidget(parent)
{
    m_player = player;

    QHBoxLayout *hSearchLayout = new QHBoxLayout;

    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText("Search for songs here!");

    m_searchButton = new QPushButton("Search", this);

    hSearchLayout->addWidget(m_searchBox);
    hSearchLayout->addWidget(m_searchButton);

    QVBoxLayout *vContainerLayout = new QVBoxLayout;
    vContainerLayout->addLayout(hSearchLayout);

    connect(m_searchButton, SIGNAL(clicked(bool)), this, SLOT(searchTriggered()));
    connect(m_searchBox, SIGNAL(returnPressed()), this, SLOT(searchTriggered()));

    setLayout(vContainerLayout);
}

// public slots
void SearchBar::searchTriggered(){
    QString query = m_searchBox->text();
    emit triggerSpotifySearch(query, 0);
}
