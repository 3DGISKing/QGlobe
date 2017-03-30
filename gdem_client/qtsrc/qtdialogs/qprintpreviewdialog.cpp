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

#include "qprintpreviewdialog.h"
#include "qprintpreviewwidget.h"
#include <private/qprinter_p.h>

#include <QtGui/qaction.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpagesetupdialog.h>
#include <QtGui/qprinter.h>
#include <QtGui/qstyle.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qvalidator.h>
#include <QtGui/qfiledialog.h>
#include <QtCore/QCoreApplication>

#include <math.h>

#ifndef QT_NO_PRINTPREVIEWDIALOG

QT_BEGIN_NAMESPACE

namespace {
class ZoomFactorValidator : public QDoubleValidator
{
public:
    ZoomFactorValidator(QObject* parent)
        : QDoubleValidator(parent) {}
    ZoomFactorValidator(qreal bottom, qreal top, int decimals, QObject *parent)
        : QDoubleValidator(bottom, top, decimals, parent) {}

    State validate(QString &input, int &pos) const
    {
        bool replacePercent = false;
        if (input.endsWith(QLatin1Char('%'))) {
            input = input.left(input.length() - 1);
            replacePercent = true;
        }
        State state = QDoubleValidator::validate(input, pos);
        if (replacePercent)
            input += QLatin1Char('%');
        const int num_size = 4;
        if (state == Intermediate) {
            int i = input.indexOf(QLocale::system().decimalPoint());
            if ((i == -1 && input.size() > num_size)
                || (i != -1 && i > num_size))
                return Invalid;
        }
        return state;
    }
};

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget* parent = 0)
        : QLineEdit(parent)
    {
        setContextMenuPolicy(Qt::NoContextMenu);
        connect(this, SIGNAL(returnPressed()), SLOT(handleReturnPressed()));
    }

protected:
    void focusInEvent(QFocusEvent *e)
    {
        origText = text();
        QLineEdit::focusInEvent(e);
    }

    void focusOutEvent(QFocusEvent *e)
    {
        if (isModified() && !hasAcceptableInput())
            setText(origText);
        QLineEdit::focusOutEvent(e);
    }

private slots:
    void handleReturnPressed()
    {
        origText = text();
    }

private:
    QString origText;
};
} // anonymous namespace

class QPrintPreviewDialogPrivate
{
    Q_DECLARE_PUBLIC(QPrintPreviewDialog)
public:
    QPrintPreviewDialogPrivate(QPrintPreviewDialog *q)
        : q_ptr(q), printDialog(0), ownPrinter(false),
          initialized(false) {}

    // private slots
    void _q_fit(QAction *action);
    void _q_zoomIn();
    void _q_zoomOut();
    void _q_navigate(QAction *action);
    void _q_setMode(QAction *action);
    void _q_pageNumEdited();
    void _q_print();
    void _q_pageSetup();
    void _q_previewChanged();
    void _q_zoomFactorChanged();

    void init(QPrinter *printer = 0);
    void populateScene();
    void layoutPages();
    void setupActions();
    void updateNavActions();
    void setFitting(bool on);
    bool isFitting();
    void updatePageNumLabel();
    void updateZoomFactor();

    QPrintPreviewDialog *q_ptr;
    QPrintDialog *printDialog;
    QPrintPreviewWidget *preview;
    QPrinter *printer;
    bool ownPrinter;
    bool initialized;

    // widgets:
    QLineEdit *pageNumEdit;
    QLabel *pageNumLabel;
    QComboBox *zoomFactor;

    // actions:
    QActionGroup* navGroup;
    QAction *nextPageAction;
    QAction *prevPageAction;
    QAction *firstPageAction;
    QAction *lastPageAction;

    QActionGroup* fitGroup;
    QAction *fitWidthAction;
    QAction *fitPageAction;

    QActionGroup* zoomGroup;
    QAction *zoomInAction;
    QAction *zoomOutAction;

    QActionGroup* orientationGroup;
    QAction *portraitAction;
    QAction *landscapeAction;

    QActionGroup* modeGroup;
    QAction *singleModeAction;
    QAction *facingModeAction;
    QAction *overviewModeAction;

    QActionGroup *printerGroup;
    QAction *printAction;
    QAction *pageSetupAction;
    QAction *closeAction;

    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;
};

void QPrintPreviewDialogPrivate::init(QPrinter *_printer)
{
    Q_Q(QPrintPreviewDialog);

    if (_printer) {
        preview = new QPrintPreviewWidget(_printer, q);
        printer = _printer;
    } else {
        ownPrinter = true;
        printer = new QPrinter;
        preview = new QPrintPreviewWidget(printer, q);
    }
    QObject::connect(preview, SIGNAL(paintRequested(QPrinter *)), q, SIGNAL(paintRequested(QPrinter *)));
    QObject::connect(preview, SIGNAL(previewChanged()), q, SLOT(_q_previewChanged()));
    setupActions();

    // Navigation
    QToolButton* nextPageButton = new QToolButton;
    nextPageButton->setDefaultAction(nextPageAction);
    QToolButton* prevPageButton = new QToolButton;
    prevPageButton->setDefaultAction(prevPageAction);
    QToolButton* firstPageButton = new QToolButton;
    firstPageButton->setDefaultAction(firstPageAction);
    QToolButton* lastPageButton = new QToolButton;
    lastPageButton->setDefaultAction(lastPageAction);

    pageNumEdit = new LineEdit;
    pageNumEdit->setAlignment(Qt::AlignRight);
    pageNumEdit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    pageNumLabel = new QLabel;
    QObject::connect(pageNumEdit, SIGNAL(editingFinished()), q, SLOT(_q_pageNumEdited()));

    QToolButton* fitWidthButton = new QToolButton;
    fitWidthButton->setDefaultAction(fitWidthAction);
    QToolButton* fitPageButton = new QToolButton;
    fitPageButton->setDefaultAction(fitPageAction);

    zoomFactor = new QComboBox;
    zoomFactor->setEditable(true);
    zoomFactor->setMinimumContentsLength(7);
    zoomFactor->setInsertPolicy(QComboBox::NoInsert);
    LineEdit *zoomEditor = new LineEdit;
    zoomEditor->setValidator(new ZoomFactorValidator(1, 1000, 1, zoomEditor));
    zoomFactor->setLineEdit(zoomEditor);
    static const short factorsX2[] = { 25, 50, 100, 200, 250, 300, 400, 800, 1600 };
    for (int i = 0; i < int(sizeof(factorsX2) / sizeof(factorsX2[0])); ++i)
        zoomFactor->addItem(QPrintPreviewDialog::tr("%1%").arg(factorsX2[i] / 2.0));
    QObject::connect(zoomFactor->lineEdit(), SIGNAL(editingFinished()),
                     q, SLOT(_q_zoomFactorChanged()));
    QObject::connect(zoomFactor, SIGNAL(currentIndexChanged(int)),
                     q, SLOT(_q_zoomFactorChanged()));

    QToolButton* zoomInButton = new QToolButton;
    zoomInButton->setDefaultAction(zoomInAction);
    zoomInButton->setAutoRepeat(true);
    zoomInButton->setAutoRepeatInterval(200);
    zoomInButton->setAutoRepeatDelay(200);

    QToolButton* zoomOutButton = new QToolButton;
    zoomOutButton->setDefaultAction(zoomOutAction);
    zoomOutButton->setAutoRepeat(true);
    zoomOutButton->setAutoRepeatInterval(200);
    zoomOutButton->setAutoRepeatDelay(200);

    //Cannot use the actions' triggered signal here, since it doesnt autorepeat
    QObject::connect(zoomInButton, SIGNAL(clicked()), q, SLOT(_q_zoomIn()));
    QObject::connect(zoomOutButton, SIGNAL(clicked()), q, SLOT(_q_zoomOut()));

    QToolButton* portraitButton = new QToolButton;
    portraitButton->setDefaultAction(portraitAction);
    QToolButton* landscapeButton = new QToolButton;
    landscapeButton->setDefaultAction(landscapeAction);

    QToolButton* singleModeButton = new QToolButton;
    singleModeButton->setDefaultAction(singleModeAction);
    QToolButton* facingModeButton = new QToolButton;
    facingModeButton->setDefaultAction(facingModeAction);
    QToolButton* overviewModeButton = new QToolButton;
    overviewModeButton->setDefaultAction(overviewModeAction);

    QToolButton *printButton = new QToolButton;
    printButton->setDefaultAction(printAction);
    QToolButton *pageSetupButton = new QToolButton;
    pageSetupButton->setDefaultAction(pageSetupAction);
    QToolButton *closeButton = new QToolButton;
    closeButton->setDefaultAction(closeAction);

    QHBoxLayout* modeLayout = new QHBoxLayout;
    modeLayout->setSpacing(0);
    modeLayout->addWidget(singleModeButton);
    modeLayout->addWidget(facingModeButton);
    modeLayout->addWidget(overviewModeButton);

    QHBoxLayout *barLayout = new QHBoxLayout;
    barLayout->addWidget(fitWidthButton);
    barLayout->addWidget(fitPageButton);
    barLayout->addWidget(zoomFactor);
    barLayout->addWidget(zoomOutButton);
    barLayout->addWidget(zoomInButton);
    barLayout->addWidget(portraitButton);
    barLayout->addWidget(landscapeButton);
    barLayout->addStretch();
    barLayout->addWidget(firstPageButton);
    barLayout->addWidget(prevPageButton);
    barLayout->addWidget(pageNumEdit);
    barLayout->addWidget(pageNumLabel);
    barLayout->addWidget(nextPageButton);
    barLayout->addWidget(lastPageButton);
    barLayout->addStretch();
    barLayout->addLayout(modeLayout);
    barLayout->addStretch();
    barLayout->addWidget(pageSetupButton);
    barLayout->addWidget(printButton);
    barLayout->addWidget(closeButton);

    QWidget* buttonBar = new QWidget;
    buttonBar->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
    barLayout->setMargin(0);
    buttonBar->setLayout(barLayout);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(buttonBar);
    topLayout->addWidget(preview);
    q->setLayout(topLayout);

    QString caption = QCoreApplication::translate("QPrintPreviewDialog", "Print Preview");
    if (!printer->docName().isEmpty())
        caption += QString::fromLatin1(": ") + printer->docName();
    q->setWindowTitle(caption);

    if (!printer->isValid()
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
        || printer->outputFormat() != QPrinter::NativeFormat
#endif
        )
        pageSetupButton->setEnabled(false);
}

static inline void qt_setupActionIcon(QAction *action, const QLatin1String &name)
{
    QLatin1String imagePrefix(":/trolltech/dialogs/qprintpreviewdialog/images/");
    QIcon icon;
    icon.addFile(imagePrefix + name + QLatin1String("-24.png"), QSize(24, 24));
    icon.addFile(imagePrefix + name + QLatin1String("-32.png"), QSize(32, 32));
    action->setIcon(icon);
}

void QPrintPreviewDialogPrivate::setupActions()
{
    Q_Q(QPrintPreviewDialog);

    // Navigation
    navGroup = new QActionGroup(q);
    navGroup->setExclusive(false);
    nextPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Next page"));
    prevPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Previous page"));
    firstPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "First page"));
    lastPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Last page"));
    qt_setupActionIcon(nextPageAction, QLatin1String("go-next"));
    qt_setupActionIcon(prevPageAction, QLatin1String("go-previous"));
    qt_setupActionIcon(firstPageAction, QLatin1String("go-first"));
    qt_setupActionIcon(lastPageAction, QLatin1String("go-last"));
    QObject::connect(navGroup, SIGNAL(triggered(QAction*)), q, SLOT(_q_navigate(QAction*)));


    fitGroup = new QActionGroup(q);
    fitWidthAction = fitGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Fit width"));
    fitPageAction = fitGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Fit page"));
    fitWidthAction->setObjectName(QLatin1String("fitWidthAction"));
    fitPageAction->setObjectName(QLatin1String("fitPageAction"));
    fitWidthAction->setCheckable(true);
    fitPageAction->setCheckable(true);
    qt_setupActionIcon(fitWidthAction, QLatin1String("fit-width"));
    qt_setupActionIcon(fitPageAction, QLatin1String("fit-page"));
    QObject::connect(fitGroup, SIGNAL(triggered(QAction*)), q, SLOT(_q_fit(QAction*)));

    // Zoom
    zoomGroup = new QActionGroup(q);
    zoomInAction = zoomGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Zoom in"));
    zoomOutAction = zoomGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Zoom out"));
    qt_setupActionIcon(zoomInAction, QLatin1String("zoom-in"));
    qt_setupActionIcon(zoomOutAction, QLatin1String("zoom-out"));

    // Portrait/Landscape
    orientationGroup = new QActionGroup(q);
    portraitAction = orientationGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Portrait"));
    landscapeAction = orientationGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Landscape"));
    portraitAction->setCheckable(true);
    landscapeAction->setCheckable(true);
    qt_setupActionIcon(portraitAction, QLatin1String("layout-portrait"));
    qt_setupActionIcon(landscapeAction, QLatin1String("layout-landscape"));
    QObject::connect(portraitAction, SIGNAL(triggered(bool)), preview, SLOT(setPortraitOrientation()));
    QObject::connect(landscapeAction, SIGNAL(triggered(bool)), preview, SLOT(setLandscapeOrientation()));

    // Display mode
    modeGroup = new QActionGroup(q);
    singleModeAction = modeGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Show single page"));
    facingModeAction = modeGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Show facing pages"));
    overviewModeAction = modeGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Show overview of all pages"));
    qt_setupActionIcon(singleModeAction, QLatin1String("view-page-one"));
    qt_setupActionIcon(facingModeAction, QLatin1String("view-page-sided"));
    qt_setupActionIcon(overviewModeAction, QLatin1String("view-page-multi"));
    singleModeAction->setObjectName(QLatin1String("singleModeAction"));
    facingModeAction->setObjectName(QLatin1String("facingModeAction"));
    overviewModeAction->setObjectName(QLatin1String("overviewModeAction"));

    singleModeAction->setCheckable(true);
    facingModeAction->setCheckable(true);
    overviewModeAction->setCheckable(true);
    QObject::connect(modeGroup, SIGNAL(triggered(QAction*)), q, SLOT(_q_setMode(QAction*)));

    // Print
    printerGroup = new QActionGroup(q);
    printAction = printerGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Print"));
    pageSetupAction = printerGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Page setup"));
    closeAction = printerGroup->addAction(QCoreApplication::translate("QPrintPreviewDialog", "Close"));
    qt_setupActionIcon(printAction, QLatin1String("print"));
    qt_setupActionIcon(pageSetupAction, QLatin1String("page-setup"));
    QObject::connect(printAction, SIGNAL(triggered(bool)), q, SLOT(_q_print()));
    QObject::connect(pageSetupAction, SIGNAL(triggered(bool)), q, SLOT(_q_pageSetup()));
    QObject::connect(closeAction, SIGNAL(triggered(bool)), q, SLOT(reject()));

    // Initial state:
    fitPageAction->setChecked(true);
    singleModeAction->setChecked(true);
    if (preview->orientation() == QPrinter::Portrait)
        portraitAction->setChecked(true);
    else
        landscapeAction->setChecked(true);
}


bool QPrintPreviewDialogPrivate::isFitting()
{
    return (fitGroup->isExclusive()
            && (fitWidthAction->isChecked() || fitPageAction->isChecked()));
}


void QPrintPreviewDialogPrivate::setFitting(bool on)
{
    if (isFitting() == on)
        return;
    fitGroup->setExclusive(on);
    if (on) {
        QAction* action = fitWidthAction->isChecked() ? fitWidthAction : fitPageAction;
        action->setChecked(true);
        if (fitGroup->checkedAction() != action) {
            // work around exclusitivity problem
            fitGroup->removeAction(action);
            fitGroup->addAction(action);
        }
    } else {
        fitWidthAction->setChecked(false);
        fitPageAction->setChecked(false);
    }
}

void QPrintPreviewDialogPrivate::updateNavActions()
{
    int curPage = preview->currentPage();
    int numPages = preview->numPages();
    nextPageAction->setEnabled(curPage < numPages);
    prevPageAction->setEnabled(curPage > 1);
    firstPageAction->setEnabled(curPage > 1);
    lastPageAction->setEnabled(curPage < numPages);
    pageNumEdit->setText(QString::number(curPage));
}

void QPrintPreviewDialogPrivate::updatePageNumLabel()
{
    Q_Q(QPrintPreviewDialog);

    int numPages = preview->numPages();
    int maxChars = QString::number(numPages).length();
    pageNumLabel->setText(QString(QLatin1String("/ %1")).arg(numPages));
    int cyphersWidth = q->fontMetrics().width(QString().fill(QLatin1Char('8'), maxChars));
    int maxWidth = pageNumEdit->minimumSizeHint().width() + cyphersWidth;
    pageNumEdit->setMinimumWidth(maxWidth);
    pageNumEdit->setMaximumWidth(maxWidth);
    pageNumEdit->setValidator(new QIntValidator(1, numPages, pageNumEdit));
    // any old one will be deleted later along with its parent pageNumEdit
}

void QPrintPreviewDialogPrivate::updateZoomFactor()
{
    zoomFactor->lineEdit()->setText(QString().sprintf("%.1f%%", preview->zoomFactor()*100));
}

void QPrintPreviewDialogPrivate::_q_fit(QAction* action)
{
    setFitting(true);
    if (action == fitPageAction)
        preview->fitInView();
    else
        preview->fitToWidth();
}

void QPrintPreviewDialogPrivate::_q_zoomIn()
{
    setFitting(false);
    preview->zoomIn();
    updateZoomFactor();
}

void QPrintPreviewDialogPrivate::_q_zoomOut()
{
    setFitting(false);
    preview->zoomOut();
    updateZoomFactor();
}

void QPrintPreviewDialogPrivate::_q_pageNumEdited()
{
    bool ok = false;
    int res = pageNumEdit->text().toInt(&ok);
    if (ok)
        preview->setCurrentPage(res);
}

void QPrintPreviewDialogPrivate::_q_navigate(QAction* action)
{
    int curPage = preview->currentPage();
    if (action == prevPageAction)
        preview->setCurrentPage(curPage - 1);
    else if (action == nextPageAction)
        preview->setCurrentPage(curPage + 1);
    else if (action == firstPageAction)
        preview->setCurrentPage(1);
    else if (action == lastPageAction)
        preview->setCurrentPage(preview->numPages());
    updateNavActions();
}

void QPrintPreviewDialogPrivate::_q_setMode(QAction* action)
{
    if (action == overviewModeAction) {
        preview->setViewMode(QPrintPreviewWidget::AllPagesView);
        setFitting(false);
        fitGroup->setEnabled(false);
        navGroup->setEnabled(false);
        pageNumEdit->setEnabled(false);
        pageNumLabel->setEnabled(false);
    } else if (action == facingModeAction) {
        preview->setViewMode(QPrintPreviewWidget::FacingPagesView);
    } else {
        preview->setViewMode(QPrintPreviewWidget::SinglePageView);
    }
    if (action == facingModeAction || action == singleModeAction) {
        fitGroup->setEnabled(true);
        navGroup->setEnabled(true);
        pageNumEdit->setEnabled(true);
        pageNumLabel->setEnabled(true);
        setFitting(true);
    }
}

void QPrintPreviewDialogPrivate::_q_print()
{
    Q_Q(QPrintPreviewDialog);

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
    if (printer->outputFormat() != QPrinter::NativeFormat) {
        QString title;
        QString suffix;
        if (printer->outputFormat() == QPrinter::PdfFormat) {
            title = QCoreApplication::translate("QPrintPreviewDialog", "Export to PDF");
            suffix = QLatin1String(".pdf");
        } else {
            title = QCoreApplication::translate("QPrintPreviewDialog", "Export to PostScript");
            suffix = QLatin1String(".ps");
        }
        QString fileName = QFileDialog::getSaveFileName(q, title, printer->outputFileName(),
                                                        QLatin1String("*") + suffix);
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(suffix);
            printer->setOutputFileName(fileName);
        }
        if (!printer->outputFileName().isEmpty())
            preview->print();
        q->accept();
        return;
    }
#endif

    if (!printDialog)
        printDialog = new QPrintDialog(printer, q);
    if (printDialog->exec() == QDialog::Accepted) {
        preview->print();
        q->accept();
    }
}

void QPrintPreviewDialogPrivate::_q_pageSetup()
{
    Q_Q(QPrintPreviewDialog);

    QPageSetupDialog pageSetup(printer, q);
    if (pageSetup.exec() == QDialog::Accepted) {
        // update possible orientation changes
        if (preview->orientation() == QPrinter::Portrait) {
            portraitAction->setChecked(true);
            preview->setPortraitOrientation();
        }else {
            landscapeAction->setChecked(true);
            preview->setLandscapeOrientation();
        }
    }
}

void QPrintPreviewDialogPrivate::_q_previewChanged()
{
    updateNavActions();
    updatePageNumLabel();
    updateZoomFactor();
}

void QPrintPreviewDialogPrivate::_q_zoomFactorChanged()
{
    QString text = zoomFactor->lineEdit()->text();
    bool ok;
    qreal factor = text.remove(QLatin1Char('%')).toFloat(&ok);
    factor = qMax(qreal(1.0), qMin(qreal(1000.0), factor));
    if (ok) {
        preview->setZoomFactor(factor/100.0);
        zoomFactor->setEditText(QString(QLatin1String("%1%")).arg(factor));
        setFitting(false);
    }
}

///////////////////////////////////////////////////////////////////////////

/*!
    \class QPrintPreviewDialog
    \since 4.4

    \brief The QPrintPreviewDialog class provides a dialog for
    previewing and configuring page layouts for printer output.

    Using QPrintPreviewDialog in your existing application is
    straightforward:

    \list 1
    \o Create the QPrintPreviewDialog.

    You can construct a QPrintPreviewDialog with an existing QPrinter
    object, or you can have QPrintPreviewDialog create one for you,
    which will be the system default printer.

    \o Connect the paintRequested() signal to a slot.

    When the dialog needs to generate a set of preview pages, the
    paintRequested() signal will be emitted. You can use the exact
    same code for the actual printing as for having the preview
    generated, including calling QPrinter::newPage() to start a new
    page in the preview. Connect a slot to the paintRequested()
    signal, where you draw onto the QPrinter object that is passed
    into the slot.

    \o Call exec().

    Call QPrintPreviewDialog::exec() to show the preview dialog.
    \endlist


    \sa QPrinter, QPrintDialog, QPageSetupDialog, QPrintPreviewWidget
*/

/*!
    Constructs a QPrintPreviewDialog based on \a printer and with \a
    parent as the parent widget. The widget flags \a flags are passed on
    to the QWidget constructor.

    \sa QWidget::setWindowFlags()
*/
QPrintPreviewDialog::QPrintPreviewDialog(QPrinter* printer, QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags), d_ptr(new QPrintPreviewDialogPrivate(this))
{
    Q_D(QPrintPreviewDialog);
    d->init(printer);
}

/*!
    \overload
    \fn QPrintPreviewDialog::QPrintPreviewDialog(QWidget *parent, Qt::WindowFlags flags)

    This will create an internal QPrinter object, which will use the
    system default printer.
*/
QPrintPreviewDialog::QPrintPreviewDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), d_ptr(new QPrintPreviewDialogPrivate(this))
{
    Q_D(QPrintPreviewDialog);
    d->init();
}

/*!
    Destroys the QPrintPreviewDialog.
*/
QPrintPreviewDialog::~QPrintPreviewDialog()
{
    Q_D(QPrintPreviewDialog);
    if (d->ownPrinter)
        delete d->printer;
    delete d->printDialog;
    delete d_ptr;
}

/*!
    \reimp
*/
void QPrintPreviewDialog::setVisible(bool visible)
{
    Q_D(QPrintPreviewDialog);
    // this will make the dialog get a decent default size
    if (visible && !d->initialized) {
        d->preview->updatePreview();
        d->initialized = true;
    }
    QDialog::setVisible(visible);
}

/*!
    \reimp
*/
void QPrintPreviewDialog::done(int result)
{
    Q_D(QPrintPreviewDialog);
    QDialog::done(result);
    if (d->receiverToDisconnectOnClose) {
        disconnect(this, SIGNAL(finished(int)),
                   d->receiverToDisconnectOnClose, d->memberToDisconnectOnClose);
        d->receiverToDisconnectOnClose = 0;
    }
    d->memberToDisconnectOnClose.clear();
}

/*!
    \overload
    \since 4.5

    Opens the dialog and connects its finished(int) signal to the slot specified
    by \a receiver and \a member.

    The signal will be disconnected from the slot when the dialog is closed.
*/
void QPrintPreviewDialog::open(QObject *receiver, const char *member)
{
    Q_D(QPrintPreviewDialog);
    // the int parameter isn't very useful here; we could just as well connect
    // to reject(), but this feels less robust somehow
    connect(this, SIGNAL(finished(int)), receiver, member);
    d->receiverToDisconnectOnClose = receiver;
    d->memberToDisconnectOnClose = member;
    QDialog::open();
}

/*!
    Returns a pointer to the QPrinter object this dialog is currently
    operating on.
*/
QPrinter *QPrintPreviewDialog::printer()
{
    Q_D(QPrintPreviewDialog);
    return d->printer;
}

/*!
    \fn void QPrintPreviewDialog::paintRequested(QPrinter *printer)

    This signal is emitted when the QPrintPreviewDialog needs to generate
    a set of preview pages.

    The \a printer instance supplied is the paint device onto which you should
    paint the contents of each page, using the QPrinter instance in the same way
    as you would when printing directly.
*/


QT_END_NAMESPACE

#include "moc_qprintpreviewdialog.cpp"
#include "qprintpreviewdialog.moc"

#endif // QT_NO_PRINTPREVIEWDIALOG


