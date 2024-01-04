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



    ui->label_verze->setText(textVerze());
    ui->label_verze->setTextInteractionFlags(Qt::TextSelectableByMouse);


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

    QTime zacatek=QTime::currentTime();
    pridejChybuDoOkna("Zacatek importu:"+zacatek.toString() );
    soubor.cestaSouboruLog=ui->lineEdit_cestaLog->text();
    soubor.cestaSouboruCsv=ui->lineEdit_cestaCsv->text();
    soubor.cestaSouboruSqLite=ui->lineEdit_cestaSqLite->text();

    soubor.sloupecky=ui->lineEdit_formatHlavicky->text();
   // soubor.otevriSoubor();
    resetujProgressBar();
    ui->progressBar2->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruLog));
    ui->progressBar3->setMaximum(soubor.spocitejRadkySouboru(soubor.cestaSouboruLog));


    emit signalSpustitImport(soubor.cestaSouboruLog);

    QTime konec=QTime::currentTime();

    pridejChybuDoOkna("Konec importu:"+konec.toString()+" \n import trval vterin: "+QString::number(zacatek.secsTo(konec)) );

   // pridejChybuDoOkna("Konec importu:"+QTime::currentTime().toString() );


}


QString MainWindow::saveToFile(QString fileType,QString popis)
{
    QString str1 = popis+" (*."+fileType+");;All Files (*)";
    QByteArray ba = str1.toLocal8Bit();
    const char *c_str2 = ba.data();


    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Ulož soubor"), "",
                                                    tr(c_str2));
    return fileName;
}

QString MainWindow::loadFromFile(QString fileType, QString popis)
{   

    QString str1 = popis+" (*."+fileType+");;All Files (*)";
    QByteArray ba = str1.toLocal8Bit();
    const char *c_str2 = ba.data();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Otevři soubor"), "",
                                                    tr(c_str2));
    //tr("Soubor logu (*.log);;All Files (*)"));


    return fileName;
}




void MainWindow::on_pushButton_inputFile_clicked()
{
    soubor.cestaSouboruLog=this->loadFromFile("log","Soubor logu");

    ui->lineEdit_cestaCsv->setText(soubor.zmenPriponu(soubor.cestaSouboruLog,"csv"));
    ui->lineEdit_cestaSqLite->setText(soubor.zmenPriponu(soubor.cestaSouboruLog,"sqlite"));
    ui->lineEdit_cestaLog->setText( soubor.cestaSouboruLog);
}


void MainWindow::on_pushButton_outputFile_clicked()
{
    soubor.cestaSouboruCsv=this->saveToFile("csv","Comma separated value ");
    ui->lineEdit_cestaCsv->setText(soubor.cestaSouboruCsv);
}

void MainWindow::on_pushButton_fileSqLite_clicked()
{
    soubor.cestaSouboruSqLite=this->saveToFile("sqlite","SQLite databáze ");
    ui->lineEdit_cestaSqLite->setText(soubor.cestaSouboruSqLite);
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


QString MainWindow::textVerze()
{
    QDate datumKompilace=QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy");
    QTime casKompilace=QTime::fromString(__TIME__,"hh:mm:ss");
    qDebug()<<" date:"<<datumKompilace<<" time:"<<casKompilace;
    QString verze=datumKompilace.toString("yyyyMMdd")+"_"+casKompilace.toString("hhmm");
    return verze;
}
