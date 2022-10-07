#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>

#include "soubor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString saveToFile();

    QString loadFromFile();
    void resetujProgressBar();

public slots:
    void pridejChybuDoOkna(QString vstup);
    void slotNastavProgressCteni(int hodnota);
    void slotNastavProgressZapis(int hodnota);
private slots:
    void on_pushButton_process_clicked();
    void on_pushButton_inputFile_clicked();
    void on_pushButton_outputFile_clicked();
    void on_pushButton_najdiHlavicky_clicked();

private:
    Ui::MainWindow *ui;
    Soubor soubor;

    QByteArray obsahSouboru;

signals:
    void signalSpustitImport(QString parametr);
};
#endif // MAINWINDOW_H
