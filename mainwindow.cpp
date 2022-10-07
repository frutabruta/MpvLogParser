#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_cestaLog->setText(soubor.cestaSouboruLog);
    ui->lineEdit_cestaCsv->setText(soubor.cestaSouboruCsv);
    connect(&soubor,&Soubor::odesliChybovouHlasku,this,&MainWindow::pridejChybuDoOkna);
    connect(&soubor,&Soubor::nastavProgressCteni,this,&MainWindow::slotNastavProgressCteni);
    connect(&soubor,&Soubor::nastavProgressZapis,this,&MainWindow::slotNastavProgressZapis);

    connect(this,&MainWindow::signalSpustitImport,&soubor,&Soubor::slotSouborNaRadky2);


    QString compilationTime = QString("%1T%2").arg(__DATE__,__TIME__);
    ui->label_verze->setText(compilationTime);



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

    soubor.cestaSouboruLog=ui->lineEdit_cestaLog->text();
    soubor.cestaSouboruCsv=ui->lineEdit_cestaCsv->text();

    soubor.sloupecky=ui->lineEdit_formatHlavicky->text();
   // soubor.otevriSoubor();
    resetujProgressBar();
    ui->progressBar2->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruLog));
    ui->progressBar3->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruLog));


    emit signalSpustitImport(soubor.cestaSouboruLog);
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




void MainWindow::on_pushButton_inputFile_clicked()
{
    soubor.cestaSouboruLog=this->loadFromFile();
    ui->lineEdit_cestaCsv->setText(soubor.zmenPriponu(soubor.cestaSouboruLog,"csv"));
    ui->lineEdit_cestaLog->setText( soubor.cestaSouboruLog);
}


void MainWindow::on_pushButton_outputFile_clicked()
{
    soubor.cestaSouboruCsv=this->saveToFile();
    ui->lineEdit_cestaCsv->setText(soubor.cestaSouboruCsv);
}

void MainWindow::pridejChybuDoOkna(QString vstup)
{
    ui->errorlog->appendPlainText(vstup);
}


void MainWindow::on_pushButton_najdiHlavicky_clicked()
{
    pridejChybuDoOkna("Zacatek hledani hlavicek:"+QTime::currentTime().toString() );

    soubor.cestaSouboruLog=ui->lineEdit_cestaLog->text();
    soubor.cestaSouboruCsv=ui->lineEdit_cestaCsv->text();
   // soubor.otevriSoubor();
    resetujProgressBar();
    ui->progressBar2->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruLog));
    ui->progressBar3->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruLog));


   // emit signalSpustitImport(soubor.cestaSouboruHex);
    pridejChybuDoOkna("Konec hledani hlavicek:"+QTime::currentTime().toString() );
    ui->lineEdit_formatHlavicky->setText(soubor.slotLogVyrobSeznamSloupecku(soubor.cestaSouboruLog));


}

