#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QtGui/QMainWindow>
#include "ui_keygenerator.h"

class KeyGenerator : public QMainWindow
{
	Q_OBJECT

public:
	KeyGenerator(QWidget *parent = 0, Qt::WFlags flags = 0);
	~KeyGenerator();

private:
	Ui::KeyGeneratorClass ui;
private slots:
	void keyGen();
};

#endif // KEYGENERATOR_H
