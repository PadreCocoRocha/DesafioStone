#ifndef RESULTSBOX_H
#define RESULTSBOX_H

#include <QWidget>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
class Player;
class QScrollArea;
class ResultItem;
class QVBoxLayout;
QT_END_NAMESPACE

class ResultsBox : public QWidget
{
    Q_OBJECT
public:
    explicit ResultsBox(QWidget *parent, Player *player);

signals:
    void addToPlaylist(QJsonObject track);
    void playThis(QJsonObject track);

public slots:
    void showResults(QJsonObject result);
    void clearResults();


private:
    Player *m_player;
    QScrollArea *m_scrollArea;
    QList<ResultItem*> *m_resultList;
    QVBoxLayout *m_container;

    int m_currentIndex;

    void parseResults(QJsonObject &result);

};

#endif // RESULTSBOX_H
