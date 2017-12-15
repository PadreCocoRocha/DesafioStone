#include "spotifywrapper.h"
#include <QWidget>
#include <QTextStream>
#include <QtNetworkAuth>
#include <QDesktopServices>
#include <player.h>

SpotifyWrapper::SpotifyWrapper(QString clientId, QString clientSecret, Player *parent)
{
    // Setup OAuth2 settings for Spotify authentication
    m_oauth2 = new QOAuth2AuthorizationCodeFlow();
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_oauth2->setReplyHandler(replyHandler);
    m_oauth2->setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    m_oauth2->setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    m_oauth2->setClientIdentifier(clientId);
    m_oauth2->setClientIdentifierSharedKey(clientSecret);

    // Spotify requires user authorization through the browser
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    connect(m_oauth2, SIGNAL(statusChanged(Status)),
            this, SLOT(authStatusChanged()));

    connect(m_oauth2, SIGNAL(granted()),
            this, SLOT(accessGranted()));

    connect(parent, SIGNAL(searchSpotify(QString)),
            this, SLOT(searchContent(QString)));
}

//public slots
void SpotifyWrapper::grant()
{
    m_oauth2->grant();
}

QUrl SpotifyWrapper::buildQueryURL(QString query){
    QString str, extra;
    QTextStream out(&str);

    out << "https://api.spotify.com/v1/search"
        << "?q=" << query;

    if (lookForAlbums || lookForArtists || lookForTracks){
        out << "&type=" << (lookForTracks ? "track," : "")
                        << (lookForAlbums ? "album," : "")
                        << (lookForArtists ? "artist," : "");

        if (*(str.end()-1) == ',') str.chop(1);
    }

    return QUrl(str);
}

// private slots
void SpotifyWrapper::authStatusChanged(){
    QString state;

    switch (m_oauth2->status()){
        case QAbstractOAuth::Status::NotAuthenticated:
            state = "Failed authenticating, try again!";
            grantedAccess = false;
            break;
        case QAbstractOAuth::Status::TemporaryCredentialsReceived:
            state = "Temporary authentication received...";
            break;
        case QAbstractOAuth::Status::Granted:
            state = "Token credentials received,"
                    " you can now make authenticated calls!";
            break;
        case QAbstractOAuth::Status::RefreshingToken:
            state = "Requesting new credentials...";
    }
    emit infoOutput(state);
}

void SpotifyWrapper::searchContent(QString query){
    query_url = buildQueryURL(query);

    auto reply = m_oauth2->get(query_url);

    // wait for request finished signal and output error if any
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit infoOutput(reply->errorString());
            return;
        }
        auto raw_data = reply->readAll();
//        emit infoOutput(raw_data);

        const QJsonDocument data =
                QJsonDocument::fromJson(raw_data);

        if (data.isNull()){
            emit infoOutput("Failed parsing reply data");
            return;
        }

        const auto searchResultsJSON = data.object();

    });
}

void SpotifyWrapper::searchParamChanged(QString query){}

void SpotifyWrapper::accessGranted()
{
    grantedAccess = true;

    QString str = "The token is:\n" + m_oauth2->token();

    emit infoOutput(str);
}
