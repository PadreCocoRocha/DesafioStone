#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QMediaPlaylist;
class ResultItem;
QT_END_NAMESPACE

class PlaylistModel : public QAbstractListModel
{
public:
    enum {
        Artist = 0,
        Title,
        PreviewUrl
    };

    PlaylistModel(QMediaPlaylist *playlist, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addToPlaylist(ResultItem *item);
    void deleteFromPlaylist(QModelIndex index);

private:
    QMediaPlaylist *m_playlist;
    QVector<QVariant> m_data;

};

#endif // PLAYLISTMODEL_H
