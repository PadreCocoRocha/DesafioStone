#ifndef SPOTIFYACCESS_H
#define SPOTIFYACCESS_H

#include <QWidget>
#include <QtNetwork>
#include <QOAuth2AuthorizationCodeFlow>

class SpotifyWrapper : public QWidget
{
    Q_OBJECT
public:
    SpotifyWrapper(QString clientId, QString clientSecret, QWidget *parent);
    void grant();

private slots:
    void authStatusChanged();
    void accessGranted();

private:
    QOAuth2AuthorizationCodeFlow m_oauth2;
    bool grantedAccess = false;

};

#endif // SPOTIFYACCESS_H
