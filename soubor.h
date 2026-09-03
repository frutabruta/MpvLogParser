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
    void otevriSoubor(); //unused

    static int spocitejRadkySouboru(QString fileName);

    //nezarazeno

    static QString zmenPriponu(QString vstup, QString pripona);



    ZaznamMpvLogu attributesToZaznamMpvLogu(const QXmlStreamAttributes &vstup);
    QVector<ZaznamMpvLogu> logZpracujRadekStream(QString radek, int cisloRadku);
    QString prevedCas(const QString &s);

private:

    //instance knihoven
    //promenne
    //funkce
    void csvOtevri(); //unused
    void csvZapisJedenRadek(QVector<ZaznamMpvLogu> &vstup, QVector<QString> &hlavicka, QFile &file, SqLiteBase &sqLiteZaklad, QSqlQuery &query);
    void csvZapisKomplet(QString vstup); //unused
    void csvZapisKonec(QFile &file);
    void csvZapisSeznamZaznamu(QVector<ZaznamMpvLogu> &vstup); //unused
    bool csvZapisZacatek(QVector<QString> &hlavicka, QFile &file);


    QVector<ZaznamMpvLogu> logZpracujRadek(QString radek, int cisloRadku);
    QVector<QString> logZpracujRadekHledejHlavicky(QString radek, int cisloRadku, QVector<QString> seznamSloupecku);

    //nezarazeno

    ZaznamMpvLogu qDomElementToZaznamMpvLogu(QDomElement vstup);
    QString vektorStringuOddelovac(QVector<QString> vstup, QString oddelovac);

    void logHledejHlavickyVElementu(QDomElement koren, QString tagName, QVector<QString> &seznamSloupecku);


public slots:
    int slotSouborNaRadky2(QString fileName);
    QString slotLogVyrobSeznamSloupecku(QString fileName);
signals:
    void odesliChybovouHlasku(QString chybovaHlaska);
    void nastavProgressCteni(int vstup);
    void nastavProgressZapis(int vstup);
    void signalResultReady(bool result);

protected:
    QSqlQuery prepareInsertQuery(QVector<QString> &hlavicka, SqLiteBase &sqLiteZaklad);
};

#endif // SOUBOR_H
