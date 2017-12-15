#ifndef SPOTIFYACCESS_H
#define SPOTIFYACCESS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Player;
class QOAuth2AuthorizationCodeFlow;
QT_END_NAMESPACE

class SpotifyWrapper : public QObject
{
    Q_OBJECT
public:
    SpotifyWrapper(QString clientId, QString clientSecret, Player *parent);

signals:
    void infoOutput(QString message);

public slots:
    void grant();

private slots:
    void authStatusChanged();
    void accessGranted();
    void searchParamChanged(QString query);
    void searchContent(QString query);

private:
    QOAuth2AuthorizationCodeFlow *m_oauth2;
    bool grantedAccess = false;
    bool lookForTracks = true;
    bool lookForAlbums = true;
    bool lookForArtists = true;

};

#endif // SPOTIFYACCESS_H
