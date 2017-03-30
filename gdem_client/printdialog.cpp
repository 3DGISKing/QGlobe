#include "printdialog.h"
#include "msgdata.h"
#include "gdemclient.h"
#include "printoutputdlg.h"

PrintDialog::PrintDialog(QPrinter* a_pPrinter, QWidget* a_pParent)
        : QDialog(a_pParent)
{
    ui.setupUi(this);

    LoadPrinterInfoOnSystem();
    m_pPrinter = a_pPrinter;
}

PrintDialog::~PrintDialog()
{
}

void PrintDialog::LoadPrinterInfoOnSystem()
{
    m_PrintInfoList = QPrinterInfo::availablePrinters();
    QPrinterInfo printInfo;
    int nDefaultIndex = 0;

    for (int i = 0; i < m_PrintInfoList.size(); i++)
    {
        printInfo = m_PrintInfoList.at(i);
        ui.printerCombo->addItem(printInfo.printerName());
        
        if (printInfo.isDefault())
            nDefaultIndex = i;
    }

    ui.printerCombo->setCurrentIndex(nDefaultIndex);
}

void PrintDialog::on_printerCombo_currentIndexChanged(int a_nIndex)
{
    m_CurPrintInfo = m_PrintInfoList.at(a_nIndex);

    LoadPageSizeList();

    QPrinter printer(m_CurPrintInfo);
    switch ( printer.printerState() )
    {
    case QPrinter::Idle:
        ui.printerStateLabel->setText(tr("Printer Idle"));
        break;
    case QPrinter::Active:
        ui.printerStateLabel->setText(tr("Printer Active"));
        break;
    case QPrinter::Aborted:
        ui.printerStateLabel->setText(tr("Printer Abort"));
        break;
    case QPrinter::Error:
        ui.printerStateLabel->setText(tr("Printer Error"));
        break;
    default:
        break;
    }
}

void PrintDialog::LoadPageSizeList()
{
    ui.sizeCombo->clear();

    m_PageSizeList = m_CurPrintInfo.supportedPaperSizes();
    
    if (m_PageSizeList.size() > 0)
    {
        OptimizePageSizeList(m_PageSizeList);
    }
    else
    {
        m_PageSizeList << QPrinter::Letter 
                << QPrinter::A4 
                << QPrinter::Custom;
    }

    QPrinter printer(m_CurPrintInfo);
    QPrinter::PaperSize ePageSize;
    QSizeF  oPageSize;

    for (int i = 0; i < m_PageSizeList.size(); i++)
    {
        ePageSize = m_PageSizeList.at(i);
        ui.sizeCombo->addItem( GetPageSizeStrFormUint(ePageSize, oPageSize) );
        ui.sizeCombo->setItemData(i, oPageSize);
    }

    ui.sizeCombo->setCurrentIndex(0);
    on_sizeCombo_currentIndexChanged(0);
}

void PrintDialog::OptimizePageSizeList(QList<QPrinter::PaperSize> &a_PageSizeList)
{
    int nListSize = a_PageSizeList.size();
    bool bCustom = false;

    for (int i = 0; i < nListSize; i++)
    {
        if (a_PageSizeList.at(i) == QPrinter::Custom)
        {
            a_PageSizeList.removeAt(i);
            i--;
            nListSize--;
            bCustom = true;            
        }
    }

    if (bCustom)
        a_PageSizeList.append(QPrinter::Custom);
}

QString PrintDialog::GetPageSizeStrFormUint(uint a_nPageSize, QSizeF &a_oPageSize)
{
    QString strPageSize;

    switch(a_nPageSize)
    {
    case QPrinter::A0:          //	5	841 x 1189 mm
        strPageSize = "A0";
        a_oPageSize.setWidth(841);
        a_oPageSize.setHeight(1189);
        break;
    case QPrinter::A1:          //      6	594 x 841 mm
        strPageSize = "A1";
        a_oPageSize.setWidth(594);
        a_oPageSize.setHeight(841);
        break;
    case QPrinter::A2:          //      7	420 x 594 mm
        strPageSize = "A2";
        a_oPageSize.setWidth(420);
        a_oPageSize.setHeight(594);
        break;
    case QPrinter::A3:          //      8	297 x 420 mm
        strPageSize = "A3";
        a_oPageSize.setWidth(297);
        a_oPageSize.setHeight(420);
        break;
    case QPrinter::A4:          //      0	210 x 297 mm, 8.26 x 11.69 inches
        strPageSize = "A4";
        a_oPageSize.setWidth(210);
        a_oPageSize.setHeight(297);
        break;
    case QPrinter::A5:          //      9	148 x 210 mm
        strPageSize = "A5";
        a_oPageSize.setWidth(148);
        a_oPageSize.setHeight(210);
        break;
    case QPrinter::A6:          //      10	105 x 148 mm
        strPageSize = "A6";
        a_oPageSize.setWidth(105);
        a_oPageSize.setHeight(148);
        break;
    case QPrinter::A7:          //      11	74 x 105 mm
        strPageSize = "A7";
        a_oPageSize.setWidth(74);
        a_oPageSize.setHeight(105);
        break;
    case QPrinter::A8:          //      12	52 x 74 mm
        strPageSize = "A8";
        a_oPageSize.setWidth(52);
        a_oPageSize.setHeight(74);
        break;
    case QPrinter::A9:          //      13	37 x 52 mm
        strPageSize = "A9";
        a_oPageSize.setWidth(37);
        a_oPageSize.setHeight(52);
        break;
    case QPrinter::B0:          //      14	1030 x 1456 mm
        strPageSize = "B0";
        a_oPageSize.setWidth(1030);
        a_oPageSize.setHeight(1456);
        break;
    case QPrinter::B1:          //      15	728 x 1030 mm
        strPageSize = "B1";
        a_oPageSize.setWidth(728);
        a_oPageSize.setHeight(1030);
        break;
    case QPrinter::B10:         //      16	32 x 45 mm
        strPageSize = "B10";
        a_oPageSize.setWidth(32);
        a_oPageSize.setHeight(45);
        break;
    case QPrinter::B2:          //      17	515 x 728 mm
        strPageSize = "B2";
        a_oPageSize.setWidth(515);
        a_oPageSize.setHeight(728);
        break;
    case QPrinter::B3:          //      18	364 x 515 mm
        strPageSize = "B3";
        a_oPageSize.setWidth(364);
        a_oPageSize.setHeight(515);
        break;
    case QPrinter::B4:          //      19	257 x 364 mm
        strPageSize = "B4";
        a_oPageSize.setWidth(257);
        a_oPageSize.setHeight(364);
        break;
    case QPrinter::B5:          //      1	182 x 257 mm, 7.17 x 10.13 inches
        strPageSize = "B5";
        a_oPageSize.setWidth(182);
        a_oPageSize.setHeight(257);
        break;
    case QPrinter::B6:          //      20	128 x 182 mm
        strPageSize = "B6";
        a_oPageSize.setWidth(128);
        a_oPageSize.setHeight(182);
        break;
    case QPrinter::B7:          //      21	91 x 128 mm
        strPageSize = "B7";
        a_oPageSize.setWidth(91);
        a_oPageSize.setHeight(128);
        break;
    case QPrinter::B8:          //      22	64 x 91 mm
        strPageSize = "B8";
        a_oPageSize.setWidth(64);
        a_oPageSize.setHeight(91);
        break;
    case QPrinter::B9:          //      23	45 x 64 mm
        strPageSize = "B9";
        a_oPageSize.setWidth(45);
        a_oPageSize.setHeight(64);
        break;
    case QPrinter::C5E:         //      24	163 x 229 mm
        strPageSize = "C5E";
        a_oPageSize.setWidth(163);
        a_oPageSize.setHeight(229);
        break;
    case QPrinter::Comm10E:	//      25	105 x 241 mm, U.S. Common 10 Envelope
        strPageSize = "Comm10E";
        a_oPageSize.setWidth(105);
        a_oPageSize.setHeight(241);
        break;
    case QPrinter::DLE:         //      26	110 x 220 mm
        strPageSize = "DLE";
        a_oPageSize.setWidth(110);
        a_oPageSize.setHeight(220);
        break;
    case QPrinter::Executive:	//      4	7.5 x 10 inches, 191 x 254 mm
        strPageSize = "Executive";
        a_oPageSize.setWidth(191);
        a_oPageSize.setHeight(254);
        break;
    case QPrinter::Folio:	//      27	210 x 330 mm
        strPageSize = "Folio";
        a_oPageSize.setWidth(210);
        a_oPageSize.setHeight(330);
        break;
    case QPrinter::Ledger:	//      28	432 x 279 mm
        strPageSize = "Ledger";
        a_oPageSize.setWidth(432);
        a_oPageSize.setHeight(279);
        break;
    case QPrinter::Legal:	//      3	8.5 x 14 inches, 216 x 356 mm
        strPageSize = "Legal";
        a_oPageSize.setWidth(216);
        a_oPageSize.setHeight(356);
        break;
    case QPrinter::Letter:      //	2	8.5 x 11 inches, 216 x 279 mm
        strPageSize = "Letter";
        a_oPageSize.setWidth(216);
        a_oPageSize.setHeight(279);
        break;
    case QPrinter::Tabloid:	//      29	279 x 432 mm
        strPageSize = "Tabloid";
        a_oPageSize.setWidth(279);
        a_oPageSize.setHeight(432);
        break;
    case QPrinter::Custom:
        strPageSize = "Custom";
        a_oPageSize.setWidth(0);
        a_oPageSize.setHeight(0);
        break;
    default:
        break;
    }

    return strPageSize;
}

void PrintDialog::on_sizeCombo_currentIndexChanged(int a_nIndex)
{
    if ( !ui.sizeCombo->currentText().compare("Custom") )
    {
        ui.widthEdit->setReadOnly(false);
        ui.heightEdit->setReadOnly(false);
    }
    else
    {
        ui.widthEdit->setReadOnly(true);
        ui.heightEdit->setReadOnly(true);
    }

    QSizeF oPageSize = ui.sizeCombo->itemData(a_nIndex).toSizeF();
    ui.widthEdit->setText(QString::number(oPageSize.width()));
    ui.heightEdit->setText(QString::number(oPageSize.height()));
}

void PrintDialog::on_OKButton_clicked()
{
    if(false == SetPrinterInfo())
        return;

    QDialog::accept();
}

bool PrintDialog::SetPrinterInfo()
{
    bool bPrintToFile = ui.printToFileCheck->isChecked();
    bool bDirection = ui.directionCombo->currentIndex();
    QSizeF oPageSize = QSizeF(ui.widthEdit->text().toDouble(), ui.heightEdit->text().toDouble());
    uint nNumOfCopies = ui.copyNumSpin->value();
    bool bColorMode = ui.colorCheck->isChecked();
    bool bBothSide = ui.bothSideCheck->isChecked();

    if(bPrintToFile)
    {
        QString strOutput;
        PrintOutputDlg printoutputdlg(&strOutput);
        if (QDialog::Accepted == printoutputdlg.exec())
        {
            m_pPrinter->setOutputFormat(QPrinter::PostScriptFormat);
            m_pPrinter->setOutputFileName(QDir::homePath() + "/" + strOutput);
        }
        else
        {
            return false;
        }
    }
    else
    {
        m_pPrinter->setOutputFormat(QPrinter::NativeFormat);
    }

    m_pPrinter->setOrientation((QPrinter::Orientation)bDirection);
    m_pPrinter->setPaperSize(oPageSize, QPrinter::Millimeter);
    m_pPrinter->setNumCopies(nNumOfCopies);
    m_pPrinter->setColorMode((QPrinter::ColorMode)bColorMode);
    m_pPrinter->setDoubleSidedPrinting(bBothSide);

    QPrinter printer(m_CurPrintInfo);
    m_pPrinter->setFullPage(true);
    m_pPrinter->setPrinterName(m_CurPrintInfo.printerName());
    m_pPrinter->setResolution(printer.resolution());

    return true;
}

void PrintDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}
