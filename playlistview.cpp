#include "playlistview.h"
#include "playlistmodel.h"

#include <QListView>
#include <QKeyEvent>

PlaylistView::PlaylistView(QWidget *parent)
    : QListView(parent)
{
}

void PlaylistView::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete){
        if (selectionModel()->hasSelection()){
            QModelIndexList selected = selectionModel()->selectedRows();
            // Only one row selectable at a time
            QModelIndex index = selected[0];
            PlaylistModel *model = (PlaylistModel*) index.model();
            model->deleteFromPlaylist(index);
        }
        return;
    }
    QListView::keyPressEvent(event);
}
