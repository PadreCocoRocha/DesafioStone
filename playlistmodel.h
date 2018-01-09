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

    bool setBuffer(ResultItem *item);
    void clearBuffer();


public slots:
    void beginInsertItems(int start, int end);
    void beginRemoveItems(int start, int end);
    void changeItems(int start, int end);
    void endInsertItems();
    void endRemoveItems();

private:
    QMediaPlaylist *m_playlist;
    QVector<QString> *m_buffer;
    QVector<QVariant> m_data;

};

#endif // PLAYLISTMODEL_H
