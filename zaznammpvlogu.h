#ifndef ZAZNAMMPVLOGU_H
#define ZAZNAMMPVLOGU_H
#include <QObject>


class ZaznamMpvLogu
{
public:
    ZaznamMpvLogu();
    QString imei="";
    QString rz="";
    QString pkt="";
    QString lat="";
    QString lng="";
    QString tm="";
    QString events="";
    QString rych="";
    QString smer="";

    QString vypis();
    QString vypisCsv();
    QString escape(QString vstup);
};

#endif // ZAZNAMMPVLOGU_H
