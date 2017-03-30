#include "directorybox.h"
#include <QToolButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include "directorydialog.h"

DirectoryBox::DirectoryBox(QWidget *parent)
		: QWidget(parent, 0)
{
	if (!Init())
		Release();
}

DirectoryBox::~DirectoryBox()
{
	Release();
}

void DirectoryBox::SetValue(QString &value)
{
	m_edit->setText(value);
}

QString DirectoryBox::GetValue()
{
	return m_edit->text();
}

bool DirectoryBox::Init()
{
	setContentsMargins(0,0,0,0);
	setMouseTracking(true);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);
	layout->setAlignment(Qt::AlignTop);
	m_edit = new QLineEdit(this);
	if (!m_edit)
		return false;
	m_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_edit->setFrame(false);
	m_edit->setAcceptDrops(false);
	m_button = new QToolButton(this);
	if (!m_button)
		return false;
	m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_button->setText("...");
	layout->addWidget(m_edit);
	layout->addWidget(m_button);
	setLayout(layout);

	connect(m_button, SIGNAL(clicked()), this, SLOT(OpenBrowse()));
	connect(m_edit, SIGNAL(textEdited(QString)), this, SIGNAL(setDirectory()));

	return true;
}

void DirectoryBox::Release()
{
	if (m_edit)
		delete m_edit;
	if (m_button)
		delete m_button;
}

void DirectoryBox::OpenBrowse()
{
	QString dir = m_edit->text();
	m_edit->setText(DirectoryDialog::GetDirectory(this, dir));
	emit setDirectory();
}

void DirectoryBox::focusInEvent ( QFocusEvent * event)
{
	m_edit->setFocus();
}


