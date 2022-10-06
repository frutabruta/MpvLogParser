#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_cestaSouboru->setText(soubor.cestaSouboruHex);
    ui->lineEdit_cestaHTML->setText(soubor.cestaSouboruHtml);
    connect(&soubor,&Soubor::odesliChybovouHlasku,this,&MainWindow::pridejChybuDoOkna);
    connect(&soubor,&Soubor::nastavProgressCteni,this,&MainWindow::slotNastavProgressCteni);
    connect(&soubor,&Soubor::nastavProgressZapis,this,&MainWindow::slotNastavProgressZapis);

    connect(this,&MainWindow::signalSpustitImport,&soubor,&Soubor::slotSouborNaRadky2);



    resetujProgressBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resetujProgressBar()
{
    ui->progressBar2->setMinimum(0);
    ui->progressBar2->setMaximum(100);
    ui->progressBar2->setValue(0);

    ui->progressBar3->setMinimum(0);
    ui->progressBar3->setMaximum(100);
    ui->progressBar3->setValue(0);
}


void MainWindow::slotNastavProgressCteni(int hodnota)
{
    ui->progressBar2->setValue(hodnota);
}

void MainWindow::slotNastavProgressZapis(int hodnota)
{
    ui->progressBar3->setValue(hodnota);
}

void MainWindow::on_pushButton_process_clicked()
{
    pridejChybuDoOkna("Zacatek importu:"+QTime::currentTime().toString() );

    soubor.cestaSouboruHex=ui->lineEdit_cestaSouboru->text();
    soubor.cestaSouboruHtml=ui->lineEdit_cestaHTML->text();
   // soubor.otevriSoubor();
    resetujProgressBar();
    ui->progressBar2->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruHex));
    ui->progressBar3->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruHex));


    emit signalSpustitImport(soubor.cestaSouboruHex);
    pridejChybuDoOkna("Konec importu:"+QTime::currentTime().toString() );


}


QString MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Ulož soubor"), "",
                                                    tr("Comma separated value (*.csv);;All Files (*)"));
    return fileName;
}

QString MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Otevři soubor"), "",
                                                    tr("Soubor logu (*.log);;All Files (*)"));
    return fileName;
}




void MainWindow::on_pushButtonInputFile_clicked()
{
    soubor.cestaSouboruHex=this->loadFromFile();
    ui->lineEdit_cestaSouboru->setText(soubor.cestaSouboruHex);
}


void MainWindow::on_pushButton_outputFile_clicked()
{
    soubor.cestaSouboruHtml=this->saveToFile();
    ui->lineEdit_cestaHTML->setText(soubor.cestaSouboruHtml);
}

void MainWindow::pridejChybuDoOkna(QString vstup)
{
    ui->errorlog->appendPlainText(vstup);
}



