/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFONTDIALOG_P_H
#define QFONTDIALOG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

#include "private/qdialog_p.h"
#include "qfontdatabase.h"
#include "qfontdialog.h"

#ifndef QT_NO_FONTDIALOG

QT_BEGIN_NAMESPACE

class QBoxLayout;
class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QFontListView;
class QGroupBox;
class QLabel;
class QLineEdit;

class QFontDialogPrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QFontDialog)

public:
    inline QFontDialogPrivate()
        : writingSystem(QFontDatabase::Any)
    { }

    void updateFamilies();
    void updateStyles();
    void updateSizes();

    static QFont getFont(bool *ok, const QFont &initial, QWidget *parent,
                         const QString &title, QFontDialog::FontDialogOptions options);

    void init();
    void _q_sizeChanged(const QString &);
    void _q_familyHighlighted(int);
    void _q_writingSystemHighlighted(int);
    void _q_styleHighlighted(int);
    void _q_sizeHighlighted(int);
    void _q_updateSample();
    void updateSampleFont(const QFont &newFont);
    void retranslateStrings();

    QLabel *familyAccel;
    QLineEdit *familyEdit;
    QFontListView *familyList;

    QLabel *styleAccel;
    QLineEdit *styleEdit;
    QFontListView *styleList;

    QLabel *sizeAccel;
    QLineEdit *sizeEdit;
    QFontListView *sizeList;

    QGroupBox *effects;
    QCheckBox *strikeout;
    QCheckBox *underline;
    QComboBox *color;

    QGroupBox *sample;
    QLineEdit *sampleEdit;

    QLabel *writingSystemAccel;
    QComboBox *writingSystemCombo;

    QBoxLayout *buttonLayout;
    QBoxLayout *effectsLayout;
    QBoxLayout *sampleLayout;
    QBoxLayout *sampleEditLayout;

    QDialogButtonBox *buttonBox;

    QFontDatabase fdb;
    QString family;
    QFontDatabase::WritingSystem writingSystem;
    QString style;
    int size;
    bool smoothScalable;
    QFont selectedFont;
    QFontDialog::FontDialogOptions opts;
    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;

#ifdef Q_WS_MAC
    static void *openCocoaFontPanel(const QFont &initial,
            QWidget *parent, const QString &title,
            QFontDialog::FontDialogOptions options,
            QFontDialogPrivate *priv = 0);
    static void closeCocoaFontPanel(void *delegate);
    static QFont execCocoaFontPanel(bool *ok, const QFont &initial, QWidget *parent,
            const QString &title, QFontDialog::FontDialogOptions options);
    static void setFont(void *delegate, const QFont &font);

    inline void done(int result) { q_func()->done(result); }
    inline QFontDialog *fontDialog() { return q_func(); }

    void *delegate;

    static bool sharedFontPanelAvailable;
#endif
};

#endif // QT_NO_FONTDIALOG

QT_END_NAMESPACE

#endif // QFONTDIALOG_P_H
