#include "spotifywrapper.h"
#include "player.h"
#include "searchbar.h"

#include <QStatusBar>
#include <QWidget>
#include <QTextStream>
#include <QJsonObject>
#include <QtNetwork>
#include <QtNetworkAuth>
#include <QDesktopServices>

SpotifyWrapper::SpotifyWrapper(QString clientId, QString clientSecret, Player *parent)
{
    m_player = parent;

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

    connect(m_oauth2, SIGNAL(statusChanged(Status)), this, SLOT(authStatusChanged()));
    connect(parent->getSearchBarInstance(), SIGNAL(triggerSpotifySearch(QString,int)),
            this, SLOT(searchContent(QString,int)));
}

QUrl SpotifyWrapper::buildQueryURL(QString query){
    QString str;
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

// public slots

void SpotifyWrapper::searchContent(QString query, int offset){
    QUrl query_url = buildQueryURL(query);

    auto reply = m_oauth2->get(query_url);

    // wait for request finished signal and output error if any
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            m_player->showMessage(reply->errorString());
            return;
        }
        // Read reply data to ByteArray
        QByteArray raw_data = reply->readAll();
//       m_player->printInfo(raw_data);

        const QJsonDocument data =
                QJsonDocument::fromJson(raw_data);

        if (data.isNull()){
            m_player->showMessage("Failed parsing reply data");
            return;
        }

        QJsonObject result = data.object();

        // Convert JSON text to object and return data through signal
        emit searchDone(result);
    });
}

void SpotifyWrapper::grant()
{
    m_oauth2->grant();
}

void SpotifyWrapper::authStatusChanged(){
    QString state;
    grantedAccess = false;

    switch (m_oauth2->status()){
        case QAbstractOAuth::Status::NotAuthenticated:
            state = "Failed authenticating, try restarting!";
            break;
        case QAbstractOAuth::Status::TemporaryCredentialsReceived:
            state = "Authenticating Spotify...";
            break;
        case QAbstractOAuth::Status::Granted:
            state = "Spotify Authenticated!";
            grantedAccess = true;
            break;
        case QAbstractOAuth::Status::RefreshingToken:
            state = "Requesting new credentials...";
    }
    m_player->showMessage(state);
    m_player->setPlayerReadyStatus(grantedAccess);
}
