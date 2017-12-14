#include "spotifywrapper.h"
#include <QAbstractOAuth>
#include <QtNetworkAuth>
#include <QDesktopServices>

SpotifyWrapper::SpotifyWrapper(QString clientId, QString clientSecret, QWidget *parent) :
    QWidget(parent)
{
    // Setup OAuth2 settings for Spotify authentication
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_oauth2.setReplyHandler(replyHandler);
    m_oauth2.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    m_oauth2.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    m_oauth2.setClientIdentifier(clientId);
    m_oauth2.setClientIdentifierSharedKey(clientSecret);

    // Spotify requires user authorization through the browser
    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
            this, &SpotifyWrapper::authStatusChanged);

    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::granted,
            this, &SpotifyWrapper::accessGranted);
}

void SpotifyWrapper::grant()
{
    m_oauth2.grant();
}

// private slots
void SpotifyWrapper::authStatusChanged(){
    QString state;

    switch (m_oauth2.status()){
        case QAbstractOAuth::Status::NotAuthenticated:
            state = "Failed authenticating, try again!";
            grantedAccess = false;
            break;
        case QAbstractOAuth::Status::TemporaryCredentialsReceived:
            state = "Temporary authentication received...";
            break;
        case QAbstractOAuth::Status::Granted:
            state = "Token credentials received,\
                     you can now make authenticated calls!";
            break;
        case QAbstractOAuth::Status::RefreshingToken:
            state = "Requesting new credentials!";
    }
    qInfo() << state;
}

void SpotifyWrapper::accessGranted()
{
    grantedAccess = true;

    qInfo() << "Successfully granted access! "
            << "The token is:" << endl
            << m_oauth2.token();
}
