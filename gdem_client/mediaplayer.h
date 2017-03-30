/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
***************************************************************************/

#ifndef MEDIALAYER_H
#define MEDIAPLAYER_H

#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtCore/QTimerEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QIcon>
#include <QtCore/QBasicTimer>
#include <QtGui/QAction>

#include <phonon/audiooutput.h>
#include <phonon/backendcapabilities.h>
#include <phonon/effect.h>
#include <phonon/effectparameter.h>
#include <phonon/effectwidget.h>
#include <phonon/mediaobject.h>
#include <phonon/seekslider.h>
#include <phonon/videowidget.h>
#include <phonon/volumeslider.h>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSlider;
class QTextEdit;
class QMenu;
class Ui_settings;
QT_END_NAMESPACE

class MediaPlayer;

class MediaVideoWidget : public Phonon::VideoWidget
{
    Q_OBJECT

public:
    MediaVideoWidget(MediaPlayer *player, QWidget *parent = 0);

public slots:
    // Over-riding non-virtual Phonon::VideoWidget slot
    void setFullScreen(bool);

signals:
    void fullScreenChanged(bool);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    bool event(QEvent *e);
    void timerEvent(QTimerEvent *e);
 private:
    MediaPlayer *m_player;
    QBasicTimer m_timer;
    QAction m_action;
};

class MediaPlayer :
            public QWidget
{
    Q_OBJECT
public:
    MediaPlayer(QWidget* parent=0);

    void setFile(const QString &text);
    void setLocation(const QString &location);
    void initVideoWindow();
    void initSettingsDialog();
    void setVolume(qreal volume);
    void setSmallScreen(bool smallScreen);
	void play();
	QString getCurrentTitle();
public slots:
    void rewind();
    void forward();
    void updateInfo();
    void updateTime();
    void finished();
    void playPause();
 	void pause();
signals:
	void videoFinished();
private slots:
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void bufferStatus(int percent);

#ifdef Q_OS_SYMBIAN
    void selectIAP();
#endif

    void hasVideoChanged(bool);
private:
    bool playPauseForDialog();
#ifdef Q_OS_SYMBIAN
    void selectIAPL();
#endif

    QIcon playIcon;
    QIcon pauseIcon;
    QMenu *fileMenu;
    QPushButton *playButton;
    QPushButton *rewindButton;
    QPushButton *forwardButton;
    Phonon::SeekSlider *slider;
    QLabel *timeLabel;
    QLabel *progressLabel;
    Phonon::VolumeSlider *volume;
    QSlider *m_hueSlider;
    QSlider *m_satSlider;
    QSlider *m_contSlider;
    QLabel *info;
    Phonon::Effect *nextEffect;
    QDialog *settingsDialog;
    Ui_settings *ui;

    QWidget m_videoWindow;
    Phonon::MediaObject m_MediaObject;
    Phonon::AudioOutput m_AudioOutput;
    MediaVideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;
    bool m_smallScreen;
};

#endif //MEDIAPLAYER_H
