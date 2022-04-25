#ifndef ZAZNAMMPVLOGU_H
#define ZAZNAMMPVLOGU_H
#include <QObject>
#include <QMap>

class ZaznamMpvLogu
{
public:
    ZaznamMpvLogu();
    /*
    QString imei="";
  QString pkt="";
  QString lat="";
  QString lng="";
  QString tm="";

  QString events="";
  QString type="";
  QString line="";
  QString conn="";
  QString rych="";
  QString smer="";
  QString evc="";
  QString np="";
  QString turnus="";
  QString ridic="";
  QString akt="";
  QString konc="";
  QString delta="";
  QString ppevent="";
  QString ppstatus="";
  QString pperror="";
  QString rz="";
  */


  QMap<QString, QString>  Obsah;







    QString vypis();
    QString vypisCsvRadek(QVector<QString> hlavicka);
   static QString escape(QString vstup);
   static QString vypisCsvHlavicka(QVector<QString> hlavicka);



};

#endif // ZAZNAMMPVLOGU_H
