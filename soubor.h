#ifndef SOUBOR_H
#define SOUBOR_H


#include "zaznammpvlogu.h"
#include <QObject>
#include <QtXml>




#include <QtDebug>
#include <QFile>
#include <QtXml>
#include <QApplication>

class Soubor : public QObject
{
    Q_OBJECT
public:
    explicit Soubor(QObject *parent = nullptr);

    void otevriSoubor();

    void vypis(QByteArray vstup);
    QString zavorky(char vstup);
    QString hexDoplnNulu(char vstup);
    QString tabulkaBunka(QString vstup);
    QString tabulkaRadek(QString vstup);
    QString wrapper(QString vstup);
    void zapisHtml(QString vstup);
    QString htmlHlavicka();
    QString htmlStyl();
    QString formatHex(QString vstup);

    QString cestaSouboruHex="vstup.hex";
    QString cestaSouboruHtml="vystup.html";
    QVector<ZaznamMpvLogu> souborNaRadky(QString fileName);
    QVector<ZaznamMpvLogu> zpracujRadek(QString radek, int cisloRadku);
    ZaznamMpvLogu qDomElementToZaznamMpvLogu(QDomElement vstup);
    void zapisCsvKomplet(QString vstup);

    QFile file;
    void otevriCsv();
    void zapisCsvSeznamZaznamu(QVector<ZaznamMpvLogu> &vstup);
    int spocitejRadkySouboru(QString fileName);

    void zapisCsvKonec(QFile &file);
    bool zapisCsvZacatek(QVector<QString> &hlavicka, QFile &file);
    void zapisCsvJedenRadek(QVector<ZaznamMpvLogu> &vstup, QVector<QString> hlavicka, QFile &file);

public slots:
    int slotSouborNaRadky2(QString fileName);
signals:
    void odesliChybovouHlasku(QString chybovaHlaska);
    void nastavProgressCteni(int vstup);
    void nastavProgressZapis(int vstup);

};

#endif // SOUBOR_H
