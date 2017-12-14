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
    bool isPermanent() const;
    void setPermanent(bool value);
    void grant();

public slots:
    void authStatusChanged();
    void granted();

private:
    QOAuth2AuthorizationCodeFlow m_oauth2;
    bool permanent = false;

};

#endif // SPOTIFYACCESS_H
