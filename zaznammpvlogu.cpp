#include "zaznammpvlogu.h"
#include "QObject"
#include <QDebug>
ZaznamMpvLogu::ZaznamMpvLogu()
{

    /*
    QString ="";
    QString ="";
   */

}


QString ZaznamMpvLogu::vypis()
{
    qDebug()<<"ZaznamMpvLogu::vypis";
    QString vystup;
    //this->Obsah["imei"];
    //vystup+=this->Obsah[""];

    vystup+="zaznam ";


    vystup+=this->Obsah.value("imei");
    vystup+" ";
    vystup+=this->Obsah.value("rz");
    vystup+" ";
    vystup+=this->Obsah.value("pkt");
    vystup+" ";
    vystup+=this->Obsah.value("lat");
    vystup+" ";
    vystup+=this->Obsah.value("lng");
    vystup+" ";
    vystup+=this->Obsah.value("tm");
    vystup+" ";
    vystup+=this->Obsah.value("events");
    vystup+" ";
    vystup+=this->Obsah.value("rych");


    return vystup;
}

QString ZaznamMpvLogu::vypisCsvRadek(QVector<QString> hlavicka)
{
  //  qDebug()<<"ZaznamMpvLogu::vypisCsv()";
    QString vystup;
    QString separator=";";
    //vystup+=escape(this->Obsah.value(""]);


    foreach( QString polozka,hlavicka)
    {
        vystup+=escape(this->Obsah.value(polozka));
        //qDebug()<<"polozka "<<polozka;
        vystup+=separator;
    }


    vystup+="\n";

    return vystup;
}

QString ZaznamMpvLogu::vypisCsvHlavicka(QVector<QString> hlavicka)
{
    qDebug()<<"ZaznamMpvLogu::vypisCsv()";
    QString vystup;
    QString separator=";";
    //vystup+=escape(this->Obsah.value(""]);


    foreach( QString polozka,hlavicka)
    {
        vystup+=escape(polozka);
       // qDebug()<<"polozka "<<polozka;
        vystup+=separator;
    }


    vystup+="\n";

    return vystup;
}

QString ZaznamMpvLogu::escape(QString vstup)
{
    QString vystup;
    vystup= "\""+vstup+"\"";


    return vystup;
}


