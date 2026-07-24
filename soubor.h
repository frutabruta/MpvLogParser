#ifndef SOUBOR_H
#define SOUBOR_H

#include "zaznammpvlogu.h"
#include <QObject>
#include <QtXml>
#include <QtDebug>
#include <QFile>
#include <QApplication>


#include "XmlRopidImportStream/sqlitebase.h"

class Soubor : public QThread
{
    Q_OBJECT

    void run() override {
        /* ... here is the expensive or blocking operation ... */
        slotSouborNaRadky2(cestaSouboruLog);
        emit signalResultReady(true);
    }

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

    static int spocitejRadkySouboru(QString fileName);

    //nezarazeno

    static QString zmenPriponu(QString vstup, QString pripona);
    bool zalozSqlTabulku(QString nazevTabulky, QVector<QString> sloupecky);

    void natahniLog(QString radek);
    ZaznamMpvLogu attributesToZaznamMpvLogu(QXmlStreamAttributes vstup);
    QVector<ZaznamMpvLogu> logZpracujRadekStream(QString radek, int cisloRadku);
private:

    //instance knihoven
    //promenne
    //funkce
    void csvOtevri();
    void csvZapisJedenRadek(QVector<ZaznamMpvLogu> &vstup, QVector<QString> &hlavicka, QFile &file, SqLiteBase &sqLiteZaklad);
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
    void signalResultReady(bool result);

};

#endif // SOUBOR_H
