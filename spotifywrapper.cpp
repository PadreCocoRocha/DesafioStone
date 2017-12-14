#include "spotifywrapper.h"
#include <QtNetworkAuth>
#include <QDesktopServices>

SpotifyWrapper::SpotifyWrapper(QString clientId, QString clientSecret, QWidget *parent) :
    QWidget(parent)
{
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_oauth2.setReplyHandler(replyHandler);
    m_oauth2.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    m_oauth2.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    m_oauth2.setClientIdentifier(clientId);
    m_oauth2.setClientIdentifierSharedKey(clientSecret);

    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged,
            this, &SpotifyWrapper::authStatusChanged);

    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
             &QDesktopServices::openUrl);

    connect(&m_oauth2, &QOAuth2AuthorizationCodeFlow::granted,
            this, &SpotifyWrapper::granted);
}

void SpotifyWrapper::grant()
{
    m_oauth2.grant();
}

// public slots
void SpotifyWrapper::authStatusChanged(){}
void SpotifyWrapper::granted(){}
