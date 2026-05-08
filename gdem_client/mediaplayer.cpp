#include <QtWidgets>
#include <QUrl>
#include "mediaplayer.h"

MediaVideoWidget::MediaVideoWidget(MediaPlayer *player, QWidget *parent) :
    QVideoWidget(parent), m_player(player), m_action(this)
{
    m_action.setCheckable(true);
    m_action.setChecked(false);
    m_action.setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_Return));
    m_action.setShortcutContext(Qt::WindowShortcut);
    connect(&m_action, SIGNAL(toggled(bool)), SLOT(setFullScreen(bool)));
    addAction(&m_action);
    setAcceptDrops(true);
}

void MediaVideoWidget::setFullScreen(bool enabled)
{
    QVideoWidget::setFullScreen(enabled);
    emit fullScreenChanged(enabled);
}

void MediaVideoWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    QVideoWidget::mouseDoubleClickEvent(e);
    setFullScreen(!isFullScreen());
}

void MediaVideoWidget::keyPressEvent(QKeyEvent *e)
{
    if (!e->modifiers() && e->key() == Qt::Key_Space) {
        m_player->playPause();
        e->accept();
        return;
    }
    if (!e->modifiers() && e->key() == Qt::Key_Escape) {
        setFullScreen(false);
        e->accept();
        return;
    }
    QVideoWidget::keyPressEvent(e);
}

bool MediaVideoWidget::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Close:
        e->ignore();
        return true;
    case QEvent::MouseMove:
        unsetCursor();
        break;
    case QEvent::WindowStateChange:
        m_action.setChecked(windowState() & Qt::WindowFullScreen);
        if (windowState() & Qt::WindowFullScreen) {
            m_timer.start(1000, this);
        } else {
            m_timer.stop();
            unsetCursor();
        }
        break;
    default:
        break;
    }

    return QVideoWidget::event(e);
}

void MediaVideoWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timer.timerId()) {
        setCursor(Qt::BlankCursor);
    }
    QVideoWidget::timerEvent(e);
}

MediaPlayer::MediaPlayer(QWidget* parent) : QWidget(parent),
        playButton(0), nextEffect(0), settingsDialog(0), ui(0),
        m_videoWidget(new MediaVideoWidget(this)), m_smallScreen(false)
{
    QSize buttonSize(34, 28);

    rewindButton = new QPushButton(this);
    rewindButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    forwardButton = new QPushButton(this);
    forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    forwardButton->setEnabled(false);

    playButton = new QPushButton(this);
    playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
    pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
    playButton->setIcon(playIcon);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 0);
    volume = new QSlider(Qt::Horizontal, this);
    volume->setRange(0, 100);
    volume->setValue(100);
 
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(5, 5, 5, 5);    

    QHBoxLayout *layout = new QHBoxLayout();

    info = new QLabel(this);
    info->setMinimumHeight(70);
    info->setAcceptDrops(false);
    info->setMargin(2);
    info->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    info->setLineWidth(2);
    info->setAutoFillBackground(true);

    QPalette palette;
    palette.setBrush(QPalette::WindowText, Qt::white);
    rewindButton->setMinimumSize(buttonSize);
    forwardButton->setMinimumSize(buttonSize);
    playButton->setMinimumSize(buttonSize);
    info->setStyleSheet("border-image:url(:/image/screen.png) ; border-width:3px");
    info->setPalette(palette);
    info->setText(tr("<center></center>"));

    volume->setFixedWidth(120);

    layout->addWidget(rewindButton);
    layout->addWidget(playButton);
    layout->addWidget(forwardButton);

    layout->addStretch();
    layout->addWidget(volume);

    vLayout->addWidget(info);
    initVideoWindow();
    vLayout->addWidget(&m_videoWindow);
    QVBoxLayout *buttonPanelLayout = new QVBoxLayout();
    m_videoWindow.hide();
    buttonPanelLayout->addLayout(layout);
    
    timeLabel = new QLabel(this);
    progressLabel = new QLabel(this);
    QWidget *sliderPanel = new QWidget(this);
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(timeLabel);    
    sliderLayout->addWidget(progressLabel);    
    sliderLayout->setContentsMargins(0, 0, 0, 0);
    sliderPanel->setLayout(sliderLayout);

    buttonPanelLayout->addWidget(sliderPanel);
    buttonPanelLayout->setContentsMargins(0, 0, 0, 0);
#ifdef Q_OS_MAC
    layout->setSpacing(4);
    buttonPanelLayout->setSpacing(0);
    info->setMinimumHeight(100);
    info->setFont(QFont("verdana", 15));
 #endif
    QWidget *buttonPanelWidget = new QWidget(this);
    buttonPanelWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed); 
    buttonPanelWidget->setLayout(buttonPanelLayout);    
    vLayout->addWidget(buttonPanelWidget);
 
    QHBoxLayout *labelLayout = new QHBoxLayout();

    vLayout->addLayout(labelLayout);
    setLayout(vLayout);

     // Setup signal connections:
    connect(rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));

    connect(playButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(forwardButton, SIGNAL(clicked()), this, SLOT(forward()));
 
    connect(&m_MediaObject, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
    connect(&m_MediaObject, SIGNAL(durationChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_MediaObject, SIGNAL(positionChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_MediaObject, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(finished()));
    connect(&m_MediaObject, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
    connect(&m_MediaObject, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferStatus(int)));
    connect(&m_MediaObject, SIGNAL(videoAvailableChanged(bool)), this, SLOT(hasVideoChanged(bool)));
    connect(slider, SIGNAL(sliderMoved(int)), &m_MediaObject, SLOT(setPosition(qint64)));
    connect(volume, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

    rewindButton->setEnabled(false);
    playButton->setEnabled(false);
    setAcceptDrops(true);

    m_MediaObject.setVideoOutput(m_videoWidget);
	setStyleSheet("QPushButton { border: 2px solid #8f8f91;}");
}

void MediaPlayer::stateChanged(QMediaPlayer::State newstate)
{
    switch (newstate)
    {
        case QMediaPlayer::StoppedState:
            m_videoWidget->setFullScreen(false);
        case QMediaPlayer::PausedState:
            playButton->setIcon(playIcon);
            if (!m_MediaObject.media().isNull()) {
                playButton->setEnabled(true);
                rewindButton->setEnabled(true);
            } else {
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
            }
            break;
        case QMediaPlayer::PlayingState:
            playButton->setEnabled(true);
            playButton->setIcon(pauseIcon);
            if (m_MediaObject.isVideoAvailable())
                m_videoWindow.show();
            rewindButton->setEnabled(true);
            break;
    }
}

void MediaPlayer::setVolume(int value)
{
    m_MediaObject.setVolume(value);
}

void MediaPlayer::setSmallScreen(bool smallScreen)
{
    m_smallScreen = smallScreen;
}

void MediaPlayer::initVideoWindow()
{
    QVBoxLayout *videoLayout = new QVBoxLayout();
    videoLayout->addWidget(m_videoWidget);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    m_videoWindow.setLayout(videoLayout);
  //  m_videoWindow.setMinimumSize(100, 100);
}

void MediaPlayer::playPause()
{
    if (m_MediaObject.state() == QMediaPlayer::PlayingState)
        m_MediaObject.pause();
    else {
        if (m_MediaObject.position() == m_MediaObject.duration())
            m_MediaObject.setPosition(0);
        m_MediaObject.play();
    }
}

void MediaPlayer::setFile(const QString &fileName)
{
    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1));
    m_MediaObject.setMedia(QUrl::fromLocalFile(fileName));
    m_MediaObject.play();
}

void MediaPlayer::setLocation(const QString& location)
{
    setWindowTitle(location.right(location.length() - location.lastIndexOf('/') - 1));
    m_MediaObject.setMedia(QUrl(location));
    m_MediaObject.play();
}

bool MediaPlayer::playPauseForDialog()
{
    // If we're running on a small screen, we want to pause the video when
    // popping up dialogs. We neither want to tamper with the state if the
    // user has paused.
    if (m_smallScreen && m_MediaObject.isVideoAvailable()) {
        if (QMediaPlayer::PlayingState == m_MediaObject.state()) {
            m_MediaObject.pause();
            return true;
        }
    }
    return false;
}

void MediaPlayer::bufferStatus(int percent)
{
    if (percent == 100)
        progressLabel->setText(QString());
    else {
        QString str = QString::fromLatin1("(%1%)").arg(percent);
        progressLabel->setText(str);
    }
}
void MediaPlayer::updateInfo()
{
    int maxLength = 30;
    QString font = "<font color=#ffeeaa>";
    QString fontmono = "<font family=\"monospace,courier new\" color=#ffeeaa>";

    QString trackArtist = m_MediaObject.metaData(QStringLiteral("Author")).toString();
    if (trackArtist.length() > maxLength)
        trackArtist = trackArtist.left(maxLength) + "...";
    
    QString trackTitle = m_MediaObject.metaData(QStringLiteral("Title")).toString();
    int trackBitrate = m_MediaObject.metaData(QStringLiteral("AudioBitRate")).toInt();

    QString fileName;
    fileName = m_MediaObject.currentMedia().canonicalUrl().toString();
    
    QString title;    
    if (!trackTitle.isEmpty()) {
        if (trackTitle.length() > maxLength)
            trackTitle = trackTitle.left(maxLength) + "...";
        title = "Title: " + font + trackTitle + "<br></font>";
    } else if (!fileName.isEmpty()) {
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
        title = font + fileName + "</font>";
        title.prepend("Media: ");
    }
        
    QString artist;
    if (!trackArtist.isEmpty())
        artist = "Artist:  " + font + trackArtist + "</font>";

    QString bitrate;
    if (trackBitrate != 0)
        bitrate = "<br>Bitrate:  " + font + QString::number(trackBitrate/1000) + "kbit</font>";

    info->setText(title + artist + bitrate);
}

void MediaPlayer::updateTime()
{
    long len = m_MediaObject.duration();
    long pos = m_MediaObject.position();
    QString timeString;    
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";        
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    timeLabel->setText(timeString);
    slider->setMaximum((int)len);
    slider->setValue((int)pos);
}

void MediaPlayer::rewind()
{
    m_MediaObject.setPosition(0);
}

void MediaPlayer::forward()
{
    m_MediaObject.setPosition(m_MediaObject.duration());
}

/*!
 \since 4.6
 */
void MediaPlayer::finished()
{
    if (m_MediaObject.mediaStatus() == QMediaPlayer::EndOfMedia) {
        emit videoFinished();
    }
}

void MediaPlayer::hasVideoChanged(bool bHasVideo)
{
    info->setVisible(!bHasVideo);
    m_videoWindow.setVisible(bHasVideo);
 }

void MediaPlayer::play()
{
	if (m_MediaObject.state() == QMediaPlayer::PlayingState)
		return;
	m_MediaObject.play();
}
void MediaPlayer::pause()
{
	if (m_MediaObject.state() == QMediaPlayer::PausedState)
		return;
	m_MediaObject.pause();
}

QString MediaPlayer::getCurrentTitle()
{
	QString fileName = m_MediaObject.currentMedia().canonicalUrl().fileName();
	return fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1) ;
}

