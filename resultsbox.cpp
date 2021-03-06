#include "resultsbox.h"
#include "player.h"
#include "spotifywrapper.h"
#include "resultitem.h"

#include <QtWidgets>
#include <QJsonObject>

ResultsBox::ResultsBox(Player *player, QWidget *parent) :
    QWidget(parent)
{
    m_player = player;

    m_container = new QVBoxLayout(this);
    m_container->setAlignment(Qt::AlignTop);

    setLayout(m_container);

    connect(player->getSpotifyInstance(), SIGNAL(searchDone(QJsonObject)),
            this, SLOT(showResults(QJsonObject)));
}

void ResultsBox::parseResults(QJsonObject &result){
    int total_tracks = 0,
        limit = 20,
        foundResults = 0;

    if (result.contains("tracks")){
        QJsonObject tracks = result["tracks"].toObject();
        if ((total_tracks = tracks["total"].toInt()) > 0){
            QJsonArray items = tracks["items"].toArray();

            limit = (limit < total_tracks) ? limit : total_tracks;

            for (int i = 0; i < limit; i++){
                QJsonObject currentJson = items[i].toObject();

                QUrl previewUrl = currentJson.value("preview_url").toString();
                // Do now show songs without preview tracks
                if (previewUrl.isEmpty()) continue;

                QString name = currentJson.value("name").toString();
                QJsonObject artist = currentJson["artists"].toArray()[0].toObject();
                QString artistName = artist["name"].toString();

                QString albumName = "";
                QUrl albumPictureUrl;

                if (currentJson.contains("album")){
                    QJsonObject albumJson = currentJson["album"].toObject();
                    albumName = albumJson["name"].toString();

                    if (albumJson.contains("images")){
                        QJsonArray images = albumJson["images"].toArray();
                        QJsonObject smallestPic = images[images.size()-1].toObject();
                        albumPictureUrl = QUrl(smallestPic["url"].toString());
                    }
                }

                foundResults++;
                ResultItem *currentItem = new ResultItem(
                            name, previewUrl, artistName, albumPictureUrl, albumName, this, m_player);

                // Add new results to resultsBox
                m_container->addWidget(currentItem);
            }
        }
    }
    if (foundResults == 0){
        m_player->showMessage("No results found!");
    }
}

//public slots
void ResultsBox::showResults(QJsonObject result){
    if (result.isEmpty()) return;

    // remove all current results
    clearResults();
    // Parse useful data and show it
    parseResults(result);
}

//public slots
void ResultsBox::clearResults(){
    QLayoutItem *child;
    while ((child = m_container->takeAt(0)) != NULL){
        delete child->widget();
        delete child;
    }
}
