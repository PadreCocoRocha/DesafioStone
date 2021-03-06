#ifndef SPOTIFYACCESS_H
#define SPOTIFYACCESS_H

#include <QWidget>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
class Player;
class QNetworkReply;
class QOAuth2AuthorizationCodeFlow;
QT_END_NAMESPACE

class SpotifyWrapper : public QObject
{
    Q_OBJECT
public:
    SpotifyWrapper(QString clientId, QString clientSecret, Player *parent);
    void grant();

signals:
    void searchDone(QJsonObject result);

private slots:
    void authStatusChanged();
    void searchContent(QString query, int offset);

private:
    QOAuth2AuthorizationCodeFlow *m_oauth2;
    Player *m_player;

    bool grantedAccess = false;

    bool lookForTracks = true;
    bool lookForAlbums = false;
    bool lookForArtists = false;

    QUrl buildQueryURL(QString query);

};

#endif // SPOTIFYACCESS_H
