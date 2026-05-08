#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QMainWindow>
#include "ui_keygenerator.h"

class KeyGenerator : public QMainWindow
{
	Q_OBJECT

public:
	KeyGenerator(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~KeyGenerator();

private:
	Ui::KeyGeneratorClass ui;
private slots:
	void keyGen();
};

#endif // KEYGENERATOR_H
