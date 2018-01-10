#include "playlistmodel.h"
#include "resultitem.h"

#include <QAbstractListModel>
#include <QMediaPlaylist>
//#include <QDebug>

PlaylistModel::PlaylistModel(QMediaPlaylist *playlist, QObject *parent) :
    QAbstractListModel(parent)
{
    m_playlist = playlist;

    connect(m_playlist, &QMediaPlaylist::mediaInserted, this, &PlaylistModel::endInsertRows);
    connect(m_playlist, &QMediaPlaylist::mediaRemoved, this, &PlaylistModel::endRemoveRows);
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

//public slots
void PlaylistModel::addToPlaylist(ResultItem *item)
{
    int index = m_playlist->mediaCount();

    beginInsertRows(QModelIndex(), index, index);

    QVector<QString> trackData(3);

    trackData[Title] = item->getTitle();
    trackData[Artist] = item->getArtist();
    trackData[PreviewUrl] = item->getPreviewUrl().toString();

    m_data.append(QVariant::fromValue(trackData));

    m_playlist->addMedia(item->getPreviewUrl());
}

void PlaylistModel::deleteFromPlaylist(QModelIndex index)
{
    int idx = index.row();
    beginRemoveRows(QModelIndex(), idx, idx);

    m_data.removeAt(idx);
    m_playlist->removeMedia(idx);
}
