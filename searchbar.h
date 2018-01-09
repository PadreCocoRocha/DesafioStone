#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Player;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QCheckBox;
QT_END_NAMESPACE

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(Player *player, QWidget *parent = nullptr);

signals:
    void triggerSpotifySearch(QString query, int offset);

public slots:
    void searchTriggered();

private:
    Player *m_player;

    QLineEdit *m_searchBox;
    QPushButton *m_searchButton;
    QGroupBox *m_typeFilters;

    QCheckBox *m_artists;
    QCheckBox *m_albums;
    QCheckBox *m_tracks;
};

#endif // SEARCHBAR_H
