#ifndef SOUBOR_H
#define SOUBOR_H

#include <QObject>
#include <QtXml>
#include "zaznammpvlogu.h"

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
    QDomElement souborNaRadky(QString fileName);
    void zpracujRadek(QString radek);
    ZaznamMpvLogu qDomElementToZaznamMpvLogu(QDomElement vstup);
    void zapisCsv(QString vstup);
signals:
     void odesliChybovouHlasku(QString chybovaHlaska);


};

#endif // SOUBOR_H
