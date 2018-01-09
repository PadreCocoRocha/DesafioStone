#include "playlistmodel.h"
#include "resultitem.h"

#include <QAbstractListModel>
#include <QMediaPlaylist>
//#include <QDebug>

PlaylistModel::PlaylistModel(QMediaPlaylist *playlist, QObject *parent) :
    QAbstractListModel(parent)
{
    m_playlist = playlist;

    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeInserted, this, &PlaylistModel::beginInsertItems);
    connect(m_playlist, &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertItems);
//    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlaylistModel::beginRemoveItems);
//    connect(m_playlist, &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveItems);
    connect(m_playlist, &QMediaPlaylist::mediaChanged, this, &PlaylistModel::changeItems);
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    if (index.isValid() && role == Qt::DisplayRole) {
        QVariant data = m_data[index.row()];

        if (data.isValid()) {
            QVector<QString> values = data.value<QVector<QString>>();
            return values[Artist] + " - " + values[Title];
        }
    }

    // return invalid QVariant if there's nothing else to be returned
    return QVariant();
}

bool PlaylistModel::setBuffer(ResultItem *item){
    if (item != nullptr){

        m_buffer = new QVector<QString>(3);

        QVector<QString>& buffer = *m_buffer;

        buffer[Title] = item->getTitle();
        buffer[Artist] = item->getArtist();
        buffer[PreviewUrl] = item->getPreviewUrl().toString();

        return true;
    }
    return false;
}

void PlaylistModel::clearBuffer(){
    if (m_buffer != nullptr){
        delete m_buffer;
    }
}

//public slots

void PlaylistModel::beginInsertItems(int start, int end)
{
    beginInsertRows(QModelIndex(), start, end);

    QVector<QString> trackData(3), &buffer = *m_buffer;

    trackData[Title] = buffer[Title];
    trackData[Artist] = buffer[Artist];
    trackData[PreviewUrl] = buffer[PreviewUrl];

    m_data.append(QVariant::fromValue(trackData));
}

//void PlaylistModel::beginRemoveItems(int start, int end)
//{
////    m_data.clear();
//    beginRemoveRows(QModelIndex(), start, end);
//    qDebug() << "Removing" << start << "to" << end;
//    m_data.remove(start);
//}

void PlaylistModel::changeItems(int start, int end)
{
    emit dataChanged(index(start,0), index(end,0));
}

void PlaylistModel::endInsertItems()
{
    endInsertRows();
}

//void PlaylistModel::endRemoveItems()
//{
//    endInsertRows();
//}
