#include <QtGui>

#define SLIDER_RANGE 8

#include "mediaplayer.h"

#ifdef Q_OS_SYMBIAN
#include <cdbcols.h>
#include <cdblen.h>
#include <commdb.h>
#endif

MediaVideoWidget::MediaVideoWidget(MediaPlayer *player, QWidget *parent) :
    Phonon::VideoWidget(parent), m_player(player), m_action(this)
{
    m_action.setCheckable(true);
    m_action.setChecked(false);
    m_action.setShortcut(QKeySequence( Qt::AltModifier + Qt::Key_Return));
    m_action.setShortcutContext(Qt::WindowShortcut);
    connect(&m_action, SIGNAL(toggled(bool)), SLOT(setFullScreen(bool)));
    addAction(&m_action);
    setAcceptDrops(true);
}

void MediaVideoWidget::setFullScreen(bool enabled)
{
    Phonon::VideoWidget::setFullScreen(enabled);
    emit fullScreenChanged(enabled);
}

void MediaVideoWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    Phonon::VideoWidget::mouseDoubleClickEvent(e);
    setFullScreen(!isFullScreen());
}

void MediaVideoWidget::keyPressEvent(QKeyEvent *e)
{
    if(!e->modifiers()) {
        // On non-QWERTY Symbian key-based devices, there is no space key.
        // The zero key typically is marked with a space character.
        if (e->key() == Qt::Key_Space || e->key() == Qt::Key_0) {
            m_player->playPause();
            e->accept();
            return;
        }

        // On Symbian devices, there is no key which maps to Qt::Key_Escape
        // On devices which lack a backspace key (i.e. non-QWERTY devices),
        // the 'C' key maps to Qt::Key_Backspace
        else if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Backspace) {
            setFullScreen(false);
            e->accept();
            return;
        }
    }
    Phonon::VideoWidget::keyPressEvent(e);
}

bool MediaVideoWidget::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Close:
        //we just ignore the cose events on the video widget
        //this prevents ALT+F4 from having an effect in fullscreen mode
        e->ignore();
        return true;
    case QEvent::MouseMove:
#ifndef QT_NO_CURSOR
        unsetCursor();
#endif
        //fall through
    case QEvent::WindowStateChange:
        {
            //we just update the state of the checkbox, in case it wasn't already
            m_action.setChecked(windowState() & Qt::WindowFullScreen);
            const Qt::WindowFlags flags = m_player->windowFlags();
            if (windowState() & Qt::WindowFullScreen) {
                m_timer.start(1000, this);
            } else {
                m_timer.stop();
#ifndef QT_NO_CURSOR
                unsetCursor();
#endif
            }
        }
        break;
    default:
        break;
    }

    return Phonon::VideoWidget::event(e);
}

void MediaVideoWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timer.timerId()) {
        //let's store the cursor shape
#ifndef QT_NO_CURSOR
        setCursor(Qt::BlankCursor);
#endif
    }
    Phonon::VideoWidget::timerEvent(e);
}

MediaPlayer::MediaPlayer(QWidget* parent) :QWidget(parent),
        playButton(0), nextEffect(0), settingsDialog(0), ui(0),
            m_AudioOutput(Phonon::VideoCategory),
            m_videoWidget(new MediaVideoWidget(this))
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

    slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(&m_MediaObject);
    volume = new Phonon::VolumeSlider(&m_AudioOutput);
 
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
#ifndef Q_WS_MAC
    rewindButton->setMinimumSize(buttonSize);
    forwardButton->setMinimumSize(buttonSize);
    playButton->setMinimumSize(buttonSize);
#endif
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
    // QStyle *flatButtonStyle = new QWindowsStyle;
    openButton->setFocusPolicy(Qt::NoFocus);
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
    connect(&m_MediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&m_MediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&m_MediaObject, SIGNAL(finished()), this, SLOT(finished()));
    connect(&m_MediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(&m_MediaObject, SIGNAL(bufferStatus(int)), this, SLOT(bufferStatus(int)));
    connect(&m_MediaObject, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

    rewindButton->setEnabled(false);
    playButton->setEnabled(false);
    setAcceptDrops(true);

    m_audioOutputPath = Phonon::createPath(&m_MediaObject, &m_AudioOutput);
    Phonon::createPath(&m_MediaObject, m_videoWidget);
	
	setStyleSheet("QPushButton { border: 2px solid #8f8f91;}");
}

void MediaPlayer::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);

    if (oldstate == Phonon::LoadingState)
	{
        QRect videoHintRect = QRect(QPoint(0, 0), m_videoWindow.sizeHint());
        QRect newVideoRect = QApplication::desktop()->screenGeometry().intersected(videoHintRect);
        if (!m_smallScreen) {
            if (m_MediaObject.hasVideo()) {
                // Flush event que so that sizeHint takes the
                // recently shown/hidden m_videoWindow into account:
                qApp->processEvents();
                resize(sizeHint());
            } else
                resize(minimumSize());
        }
    }

    switch (newstate) 
	{
        case Phonon::ErrorState:
            if (m_MediaObject.errorType() == Phonon::FatalError) 
			{
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
            } 
			else 
			{
                m_MediaObject.pause();
            }
            QMessageBox::warning(this, "Phonon Mediaplayer", m_MediaObject.errorString(), QMessageBox::Close);
            break;

        case Phonon::StoppedState:
            m_videoWidget->setFullScreen(false);
            // Fall through
        case Phonon::PausedState:
            playButton->setIcon(playIcon);
            if (m_MediaObject.currentSource().type() != Phonon::MediaSource::Invalid)
			{
                playButton->setEnabled(true);
                rewindButton->setEnabled(true);
            }
			else
			{
                playButton->setEnabled(false);
                rewindButton->setEnabled(false);
            }
            break;
        case Phonon::PlayingState:
            playButton->setEnabled(true);
            playButton->setIcon(pauseIcon);
            if (m_MediaObject.hasVideo())
                m_videoWindow.show();
            // Fall through
        case Phonon::BufferingState:
            rewindButton->setEnabled(true);
            break;
        case Phonon::LoadingState:
            rewindButton->setEnabled(false);
            break;
    }

}

void MediaPlayer::setVolume(qreal volume)
{
    m_AudioOutput.setVolume(volume);
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
    if (m_MediaObject.state() == Phonon::PlayingState)
        m_MediaObject.pause();
    else {
        if (m_MediaObject.currentTime() == m_MediaObject.totalTime())
            m_MediaObject.seek(0);
        m_MediaObject.play();
    }
}

void MediaPlayer::setFile(const QString &fileName)
{
    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1));
    m_MediaObject.setCurrentSource(Phonon::MediaSource(fileName));
    m_MediaObject.play();
}

void MediaPlayer::setLocation(const QString& location)
{
    setWindowTitle(location.right(location.length() - location.lastIndexOf('/') - 1));
    m_MediaObject.setCurrentSource(Phonon::MediaSource(QUrl::fromEncoded(location.toUtf8())));
    m_MediaObject.play();
}

bool MediaPlayer::playPauseForDialog()
{
    // If we're running on a small screen, we want to pause the video when
    // popping up dialogs. We neither want to tamper with the state if the
    // user has paused.
    if (m_smallScreen && m_MediaObject.hasVideo()) {
        if (Phonon::PlayingState == m_MediaObject.state()) {
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

    QMap <QString, QString> metaData = m_MediaObject.metaData();
    QString trackArtist = metaData.value("ARTIST");
    if (trackArtist.length() > maxLength)
        trackArtist = trackArtist.left(maxLength) + "...";
    
    QString trackTitle = metaData.value("TITLE");
    int trackBitrate = metaData.value("BITRATE").toInt();

    QString fileName;
    if (m_MediaObject.currentSource().type() == Phonon::MediaSource::Url) {
        fileName = m_MediaObject.currentSource().url().toString();
    } else {
        fileName = m_MediaObject.currentSource().fileName();
        fileName = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
    }
    
    QString title;    
    if (!trackTitle.isEmpty()) {
        if (trackTitle.length() > maxLength)
            trackTitle = trackTitle.left(maxLength) + "...";
        title = "Title: " + font + trackTitle + "<br></font>";
    } else if (!fileName.isEmpty()) {
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
        title = font + fileName + "</font>";
        if (m_MediaObject.currentSource().type() == Phonon::MediaSource::Url) {
            title.prepend("Url: ");
        } else {
            title.prepend("File: ");
        }
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
    long len = m_MediaObject.totalTime();
    long pos = m_MediaObject.currentTime();
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
}

void MediaPlayer::rewind()
{
    m_MediaObject.seek(0);
}

void MediaPlayer::forward()
{
    QList<Phonon::MediaSource> queue = m_MediaObject.queue();
    if (queue.size() > 0) {
        m_MediaObject.setCurrentSource(queue[0]);
        forwardButton->setEnabled(queue.size() > 1);
        m_MediaObject.play();
    }
}

/*!
 \since 4.6
 */
void MediaPlayer::finished()
{
	emit videoFinished();
}

void MediaPlayer::hasVideoChanged(bool bHasVideo)
{
    info->setVisible(!bHasVideo);
    m_videoWindow.setVisible(bHasVideo);
 }

void MediaPlayer::play()
{
	if (m_MediaObject.state() == Phonon::PlayingState)
		return;
	m_MediaObject.play();
}
void MediaPlayer::pause()
{
	if (m_MediaObject.state() == Phonon::PausedState)
		return;
	m_MediaObject.pause();
}

QString MediaPlayer::getCurrentTitle()
{
	QString fileName=m_MediaObject.currentSource().fileName();
	return fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1) ;
}

#ifdef Q_OS_SYMBIAN
void MediaPlayer::selectIAP()
{
    TRAPD(err, selectIAPL());
    if (KErrNone != err)
        QMessageBox::warning(this, "Phonon Mediaplayer", "Error selecting IAP", QMessageBox::Close);
}

void MediaPlayer::selectIAPL()
{
    QVariant currentIAPValue = m_MediaObject.property("InternetAccessPointName");
    QString currentIAPString = currentIAPValue.toString();
    bool ok = false;
    CCommsDatabase *commsDb = CCommsDatabase::NewL(EDatabaseTypeIAP);
    CleanupStack::PushL(commsDb);
    commsDb->ShowHiddenRecords();
    CCommsDbTableView* view = commsDb->OpenTableLC(TPtrC(IAP));
    QStringList items;
    TInt currentIAP = 0;
    for (TInt l = view->GotoFirstRecord(), i = 0; l != KErrNotFound; l = view->GotoNextRecord(), i++) {
       TBuf<KCommsDbSvrMaxColumnNameLength> iapName;
       view->ReadTextL(TPtrC(COMMDB_NAME), iapName);
       QString iapString = QString::fromUtf16(iapName.Ptr(), iapName.Length());
       items << iapString;
       if (iapString == currentIAPString)
           currentIAP = i;
    }
    currentIAPString = QInputDialog::getItem(this, tr("Select Access Point"), tr("Select Access Point"), items, currentIAP, false, &ok);
    if (ok)
        m_MediaObject.setProperty("InternetAccessPointName", currentIAPString);
    CleanupStack::PopAndDestroy(2); //commsDB, view
}
#endif

