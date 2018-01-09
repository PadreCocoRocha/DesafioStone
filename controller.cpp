#include "controller.h"
#include "player.h"
#include <QMediaPlayer>
#include <QtWidgets>

// Added this custom slider here to avoid the creation of more files
// and because this class is used only here
class JumpingSlider : public QSlider
{
public:
    JumpingSlider() : QSlider(Qt::Horizontal) {}

    // Allows slider to jump on click
    void mousePressEvent(QMouseEvent *e){
        int value = QStyle::sliderValueFromPosition(
                    minimum(), maximum(), e->x(), width());
        setValue(value);
        emit sliderMoved(value);
    }
    // Allows (again) to move the slider - It was blocked by jump on click
    void mouseMoveEvent(QMouseEvent *e) { mousePressEvent(e); }
};

Controller::Controller(QMediaPlayer *player, QWidget *parent) :
    QWidget(parent)
{
    m_player = player;

    QBoxLayout *layout = new QVBoxLayout;

// position slider and duration
    m_positionSlider = new JumpingSlider();
    m_positionSlider->setRange(0, m_player->duration());

    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(m_positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPosition(int)));
    connect(m_positionSlider, SIGNAL(sliderJumped(int)), this, SLOT(setPosition(int)));
// end position slider ...

// player control widgets
    QHBoxLayout *lowerLayout = new QHBoxLayout;

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

    m_volumeSlider = new JumpingSlider();
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setMinimumWidth(200);

    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged()));
// end of player control widgets

    lowerLayout->setMargin(0);
    lowerLayout->addWidget(m_stopButton);
    lowerLayout->addWidget(m_previousButton);
    lowerLayout->addWidget(m_playButton);
    lowerLayout->addWidget(m_nextButton);
    lowerLayout->addWidget(m_volumeSlider);
    lowerLayout->setAlignment(m_volumeSlider, Qt::AlignRight);

    layout->addWidget(m_positionSlider);
    layout->addLayout(lowerLayout);

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

void Controller::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_positionSlider->setMaximum(m_duration);
}

void Controller::positionChanged(qint64 position)
{
    if (!m_positionSlider->isSliderDown())
        m_positionSlider->setValue(position / 1000);
}

void Controller::setPosition(int position){
    m_player->setPosition((qint64) position * 1000);
}
