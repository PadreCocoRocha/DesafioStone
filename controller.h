#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QToolButton;
class QSlider;
class QMediaPlayer;
QT_END_NAMESPACE

class Controller : public QWidget
{
    Q_OBJECT
public:
    explicit Controller(QMediaPlayer *player, QWidget *parent = nullptr);
    int linearVolume() const;

signals:
    void play();
    void pause();
    void stop(bool);
    void previous();
    void next();
    void setVolume(int volume);
    void sliderJumped(int value);

public slots:
    void playClicked();
    void volumeChanged();
    void setState(QMediaPlayer::State);
    void changeVolume(int linearVolume);
    void durationChanged(qint64);
    void positionChanged(qint64);
    void setPosition(int);

private:
    QMediaPlayer *m_player;

    qint64 m_duration;

    QToolButton *m_playButton;
    QToolButton *m_stopButton;
    QToolButton *m_previousButton;
    QToolButton *m_nextButton;

    QSlider *m_volumeSlider;
    QSlider *m_positionSlider;

    QMediaPlayer::State m_playerState;

    QMediaPlayer::State state() const;
};

#endif // CONTROLLER_H
