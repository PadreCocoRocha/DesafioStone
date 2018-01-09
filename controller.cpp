#include "controller.h"
#include "player.h"
#include <QMediaPlayer>
#include <QtWidgets>

Controller::Controller(QWidget *parent) :
    QWidget(parent)
{
    QBoxLayout *layout = new QHBoxLayout;

    m_playerState = QMediaPlayer::StoppedState;

    m_playButton = new QToolButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, SIGNAL(clicked(bool)), this, SLOT(playClicked()));

    m_stopButton = new QToolButton(this);
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setEnabled(false);

    connect(m_stopButton, SIGNAL(clicked(bool)), this, SIGNAL(stop(bool)));

    m_previousButton = new QToolButton(this);
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(m_previousButton, SIGNAL(clicked(bool)), this, SIGNAL(previous()));

    m_nextButton = new QToolButton(this);
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(m_nextButton, SIGNAL(clicked(bool)), this, SIGNAL(next()));

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setMinimumWidth(200);

    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged()));

    layout->setMargin(0);
    layout->addWidget(m_stopButton);
    layout->addWidget(m_previousButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_volumeSlider);
    layout->setAlignment(m_volumeSlider, Qt::AlignRight);

    setLayout(layout);
}

QMediaPlayer::State Controller::state() const
{
    return m_playerState;
}

void Controller::setState(QMediaPlayer::State state)
{
    if (state != m_playerState) {
        m_playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            m_stopButton->setEnabled(false);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

int Controller::linearVolume() const
{

    qreal linearVolume = QAudio::convertVolume(
        m_volumeSlider->value() / qreal(100),
        QAudio::LogarithmicVolumeScale,
        QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

void Controller::changeVolume(int linearVolume)
{
    qreal logarithmicVolume = QAudio::convertVolume(
        linearVolume / qreal(100),
        QAudio::LinearVolumeScale,
        QAudio::LogarithmicVolumeScale);

    m_volumeSlider->setValue(qRound(logarithmicVolume * 100));
}

// public slots
void Controller::volumeChanged(){
    emit setVolume(linearVolume());
}

void Controller::playClicked()
{
    switch (m_playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}
