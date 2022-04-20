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
    QString vystup;
    vystup+="zaznam "+this->imei+" "+this->rz+" "+this->pkt+" "+this->lat+" "+ this->lng+" "+ this->tm+" "+ this->events+" "+this->rych;

    return vystup;
}

QString ZaznamMpvLogu::vypisCsv()
{
    QString vystup;
    QString separator=";";
    vystup+=escape(this->imei);
    vystup+=separator;
    vystup+=escape(this->rz);
    vystup+=separator;
    vystup+=escape(this->pkt);
    vystup+=separator;
    vystup+=escape(this->lat);
    vystup+=separator;
    vystup+=escape(this->lng);
    vystup+=separator;
    vystup+=escape(this->tm);
    vystup+=separator;
    vystup+=escape(this->events);
    vystup+=separator;
    vystup+=escape(this->rych);

    vystup+="\n";

    return vystup;
}

QString ZaznamMpvLogu::escape(QString vstup)
{
    QString vystup;
    vystup= "\""+vstup+"\"";


    return vystup;
}
