#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QListView>

QT_BEGIN_NAMESPACE
class PlaylistModel;
QT_END_NAMESPACE

class PlaylistView : public QListView
{
public:
    PlaylistView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);

private:
    PlaylistModel *m_model;
};

#endif // PLAYLISTVIEW_H
