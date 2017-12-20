#include "resultsbox.h"
#include "player.h"
#include "spotifywrapper.h"
#include "resultitem.h"

#include <QtWidgets>
#include <QJsonObject>

ResultsBox::ResultsBox(QWidget *parent, Player *player) :
    QWidget(parent)
{
    m_player = player;
    m_parent = parent;

    m_container = new QVBoxLayout(this);

    m_resultList = new QList<ResultItem*>;

    setMinimumSize(200,200);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(parent);

    setLayout(m_container);

    connect(player->getSpotifyInstance(), SIGNAL(searchDone(QJsonObject)),
            this, SLOT(showResults(QJsonObject)));
}

void ResultsBox::parseResults(QJsonObject &result){
    int total_tracks = 0,
        limit = 20,
        counter = 0;

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

//                QLabel *currentItem = new QLabel(name);
                ResultItem *currentItem = new ResultItem(
                            name, previewUrl, artistName, albumPictureUrl, albumName, this, m_player);

                m_player->printInfo(
                    QString("Got to parseResults, name: " + name
                            + " - Artist name: " + artistName
                            + " - Album Name: " + albumName));

                // Add new results to resultsBox
//                m_resultList->append(currentItem);
                m_container->addWidget(currentItem);
//                currentItem->show();
            }
        }
    }
}

//public slots
void ResultsBox::showResults(QJsonObject result){
    if (result.isEmpty()) return;

    QVBoxLayout* ql = m_container;
    QList<ResultItem*> *ri = m_resultList;

    // remove all current results
    clearResults();

    parseResults(result);

    m_player->printInfo("Got to show results");
}

//public slots
void ResultsBox::clearResults(){
    QLayoutItem *child;
    while ((child = m_container->takeAt(0)) != NULL){
        delete child->widget();
        delete child;
    }
}
