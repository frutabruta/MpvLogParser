#ifndef SOUBOR_H
#define SOUBOR_H

#include "zaznammpvlogu.h"
#include <QObject>
#include <QtXml>
#include <QtDebug>
#include <QFile>
#include <QApplication>


#include "XmlRopidImportStream/sqlitezaklad.h"

class Soubor : public QObject
{
    Q_OBJECT

public:

    //konstruktor a destruktor
    explicit Soubor(QObject *parent = nullptr);


    //instance knihoven

    //promenne

    QString cestaSouboruLog="";
    QString cestaSouboruCsv="";
    QString cestaSouboruSqLite="";
    QString sloupecky="";
    QFile file;

    //funkce
    void otevriSoubor();

    int spocitejRadkySouboru(QString fileName);

    //nezarazeno

    QString zmenPriponu(QString vstup, QString pripona);
    bool zalozSqlTabulku(QString nazevTabulky, QVector<QString> sloupecky);

private:

    //instance knihoven
    //promenne
    //funkce
    void csvOtevri();
    void csvZapisJedenRadek(QVector<ZaznamMpvLogu> &vstup, QVector<QString> hlavicka, QFile &file, SqLiteZaklad &sqLiteZaklad);
    void csvZapisKomplet(QString vstup);
    void csvZapisKonec(QFile &file);
    void csvZapisSeznamZaznamu(QVector<ZaznamMpvLogu> &vstup);
    bool csvZapisZacatek(QVector<QString> &hlavicka, QFile &file);

    QString formatHex(QString vstup);
    QString hexDoplnNulu(char vstup);

    QString htmlHlavicka();
    QString htmlStyl();
    QString htmlTabulkaBunka(QString vstup); //nepouzito
    QString htmlTabulkaRadek(QString vstup); //nepouzit
    void htmlVypis(QByteArray vstup);
    QString htmlWrapper(QString vstup);
    void htmlZapis(QString vstup);
    QString htmlZavorky(char vstup);

    QVector<ZaznamMpvLogu> logSouborNaRadky(QString fileName);
    QVector<ZaznamMpvLogu> logZpracujRadek(QString radek, int cisloRadku);
    QVector<QString> logZpracujRadekHledejHlavicky(QString radek, int cisloRadku, QVector<QString> seznamSloupecku);

    //nezarazeno

    ZaznamMpvLogu qDomElementToZaznamMpvLogu(QDomElement vstup);
    QString vektorStringuOddelovac(QVector<QString> vstup, QString oddelovac);

public slots:
    int slotSouborNaRadky2(QString fileName);
    QString slotLogVyrobSeznamSloupecku(QString fileName);
signals:
    void odesliChybovouHlasku(QString chybovaHlaska);
    void nastavProgressCteni(int vstup);
    void nastavProgressZapis(int vstup);

};

#endif // SOUBOR_H
