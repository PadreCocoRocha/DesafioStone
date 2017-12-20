#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSlider;
class Player;
QT_END_NAMESPACE

class Controller : public QWidget
{
    Q_OBJECT
public:
    explicit Controller(QWidget *parent = nullptr);
    int linearVolume() const;

signals:
    void play();
    void pause();
    void stop(bool);
    void setVolume(int volume);

public slots:
    void playClicked();
    void volumeChanged();
    void setState(QMediaPlayer::State);
    void changeVolume(int linearVolume);

private:
    QToolButton *m_playButton;
    QToolButton *m_stopButton;
    QSlider *m_volumeSlider;

    QMediaPlayer::State m_playerState;

    QMediaPlayer::State state() const;
};

#endif // CONTROLLER_H
