#ifndef CONTOURDLG_H
#define CONTOURDLG_H

#include <QtGui/QDialog>

namespace Ui {
    class ContourDlg;
}


#define CONTOUR_INTERVAL_MIN	20
#define CONTOUR_INTERVAL_MAX	10000
#define CONTOUR_THICK_MIN		1
#define CONTOUR_THICK_MAX		50
class ContourDlg : public QDialog {
    Q_OBJECT
public:
    ContourDlg(QWidget *parent = 0);
    ~ContourDlg();

protected:
	double			m_contourInterval;
	QColor			m_ravineColor;
	QColor			m_mainCurveColor;
	unsigned int	m_ravineThick;
	unsigned int	m_mainCurveThick;

    void changeEvent(QEvent *e);
	void Init();
	void GetParameter();
	void SetParameter();
	void UpdateDialog();

protected slots:
	void SetContourInterval();
	void SetRavineColor();
	void SetRavineThick(int thick);
	void SetMainCurveColor();
	void SetMainCurveThick(int thick);
	void SetDefaultValue();

	virtual void accept();
	virtual void reject();

private:
    Ui::ContourDlg *m_ui;
};

#endif // CONTOURDLG_H
