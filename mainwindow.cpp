#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_cestaSouboru->setText(soubor.cestaSouboruHex);
    ui->lineEdit_cestaHTML->setText(soubor.cestaSouboruHtml);
    connect(&soubor,&Soubor::odesliChybovouHlasku,this,&MainWindow::pridejChybuDoOkna);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_process_clicked()
{
    soubor.cestaSouboruHex=ui->lineEdit_cestaSouboru->text();
    soubor.cestaSouboruHtml=ui->lineEdit_cestaHTML->text();
   // soubor.otevriSoubor();
   QVector<ZaznamMpvLogu> vysledek= soubor.souborNaRadky(soubor.cestaSouboruHex);
    soubor.zapisCsvSeznamZaznamu(vysledek);


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



