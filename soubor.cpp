#include "soubor.h"
#include <QtDebug>
#include <QFile>
#include <QtXml>
#include "zaznammpvlogu.h"


Soubor::Soubor(QObject *parent) : QObject(parent)
{
    QString nazevSouboru="testhexdump.hex";
    cestaSouboruHex=QCoreApplication::applicationDirPath()+"/"+nazevSouboru;
}



void Soubor::otevriSoubor()
{
    qDebug()<<"Soubor::otevriSoubor()";
    QDomDocument doc("mydocument");
    // QFile file("xml_zdroje/XML_Zlicin_20200702_20200705.xml");

    // QFile file(cesta+"/konfigurace/konfigurace.xml");
    //QString cesta;

    qDebug()<<"cesta k souboru je "<<cestaSouboruHex;
    QFile file(cestaSouboruHex);




    if (!file.open(QIODevice::ReadOnly))
    {
        //  emit odesliChybovouHlasku("soubor se nepovedlo otevrit");
        QString neotevruSoubor="soubor se nepovedlo otevrit";
        qDebug()<<neotevruSoubor;
        emit odesliChybovouHlasku(neotevruSoubor);
        return;
    }
    /*
    if (!doc.setContent(&file))
    {
      //   emit odesliChybovouHlasku("soubor se nepovedlo otevrit2");
        qDebug()<<"nepovedlo se nastavit obsah dom dokumentu";
        file.close();
        return;
    }
    */
    QByteArray vysledek;
    vysledek=file.readAll();
    file.close();

    emit    odesliChybovouHlasku("soubor otevren");
    qDebug()<<"uspech";
    QDomElement koren=doc.firstChildElement();

    qDebug()<<vysledek;


    this->vypis(vysledek);

    // najdiCestaHlaseni(doc);




}


void Soubor::vypis(QByteArray vstup)
{
    qDebug()<<"Soubor::vypis";
    qDebug()<<"delka velikost je "<<vstup.size()<<" delka je "<<vstup.length();
    int delka=vstup.length();
    QVector<QString> radky;
    QVector<QString> radkyHex;
    QString radek="";
    QString radekHex="";
    for (int i=0; i<(delka);i++)
    {

        radek+=zavorky(vstup.at(i));
        radekHex+=" ";
        radekHex+=this->formatHex(this->hexDoplnNulu(vstup.at(i)));
        if (vstup.at(i) ==0x0D)
        {
            i++;
            radek+=zavorky(vstup.at(i));
            radekHex+=" ";
            radekHex+=this->formatHex(this->hexDoplnNulu(vstup.at(i)));

            radky.append(radek);
            radkyHex.append(radekHex);
            radek="";
            radekHex="";
            qDebug()<<"konec prikazu";

        }

        qDebug()<<i<<" "<<vstup[i];
    }
    qDebug()<<"konec";






    QString htmlSoubor="";
    QString htmlRadky="";
    for (int g=0;g<radkyHex.length();g++)
    {
        htmlRadky+=tabulkaRadek(tabulkaBunka(radky.at(g))+tabulkaBunka(radkyHex.at(g)));
        qDebug()<<radky.at(g)<<" "<<radkyHex.at(g);
    }
    /*
    foreach(QString hodnota,radky)
    {
        qDebug()<<hodnota;
    }
*/
    htmlSoubor=this->wrapper(htmlRadky);
    zapisHtml(htmlSoubor);
    qDebug()<<htmlSoubor;

}


QString Soubor::zavorky(char vstup)
{
    QString vystup="";
    if (vstup>=0x20)
    {
        vystup+=vstup;
    }
    else
    {
        vystup+="<";
        vystup+=formatHex( hexDoplnNulu(vstup));
        vystup+=">";
    }
    return vystup;
}


QString Soubor::hexDoplnNulu(char vstup)
{
    QString vystup=QString::number(vstup,16);
    if (vstup<0x10)
    {
        vystup="0"+vystup;
    }
    return vystup;

}

QString Soubor::tabulkaBunka(QString vstup)
{
    QString vystup="<td>"+vstup+"</td>";
    return vystup;
}

QString Soubor::tabulkaRadek(QString vstup)
{
    QString vystup="<tr>"+vstup+"</tr>";
    return vystup;
}

QString Soubor::wrapper(QString vstup)
{
    QString vystup="<!DOCTYPE html><html>"+htmlHlavicka()+"<body><table>"+vstup+"<table><body></html>";
    return vystup;
}

void Soubor::zapisHtml(QString vstup)
{
    QFile file(cestaSouboruHtml);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // We're going to streaming text to the file
        QTextStream stream(&file);

        stream << vstup;
        file.close();
        QString zapsano="Writing finished";
        qDebug() << zapsano;
        emit odesliChybovouHlasku(zapsano);
    }
    else
    {
        QString chybovaHlaska="soubor nelze zapsat";
        qDebug()<<chybovaHlaska;
        emit odesliChybovouHlasku(chybovaHlaska);
    }


}

QString Soubor::htmlHlavicka()
{
    QString vystup="<head>"+this->htmlStyl()+"</head>";
    QString vystup2="<head><link rel=\"stylesheet\" href=\"mystyle.css\"></head>";
    return vystup;
}

QString Soubor::htmlStyl()
{
    QString styl="*{font-family: \"Courier New\", Courier, \"Lucida Sans Typewriter\", \"Lucida Typewriter\", monospace;color: black;}";
    styl+="td{border: 1px solid blue;}";
    styl+="span.escape{color:red;}";
    styl+="span.format{color:orange;}";
    styl+="span.lf{color:green;}";

    QString vystup="<style>"+styl+"</style>";
    return vystup;
}

QString Soubor::formatHex(QString vstup)
{
    QString vystup=vstup;
    if (vstup=="1b")
    {
        vystup="<span class=\"format\">"+vstup+"</span>";
    }
    if (vstup=="0a")
    {
        vystup="<span class=\"lf\">"+vstup+"</span>";
    }
    if (vstup=="0e")
    {
        vystup="<span class=\"escape\">"+vstup+"</span>";
    }

    return vystup;
}


QDomElement Soubor::souborNaRadky(QString fileName)
{
    // zdroj: https://stackoverflow.com/questions/5444959/read-a-text-file-line-by-line-in-qt
    QFile inputFile(fileName);
    QDomElement vystup;
    int counter=0;
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            zpracujRadek(line);
            // qDebug()<<counter<<" "<<line;
            counter++;
        }
        inputFile.close();
    }
    return vystup;
}


void Soubor::zpracujRadek(QString radek)
{
    qDebug()<<"Soubor::zpracujRadek";
    int zacatek =radek.indexOf("<");
    QString orezanyRadek;
    if (zacatek<0)
    {
        qDebug()<<"zadna zprava na tomto radku";
    }
    else
    {
        orezanyRadek=radek.mid(zacatek);
      //  qDebug()<<" orezany radek "<<orezanyRadek;
    }
    QDomDocument soubor;
    soubor.setContent(orezanyRadek);
    QDomElement koren;
    koren=soubor.firstChildElement();
    QDomNodeList elementy=koren.elementsByTagName("V");

    for( int i=0;i<elementy.count();i++)
    {
        ZaznamMpvLogu vysledek=qDomElementToZaznamMpvLogu(elementy.at(i).toElement());
        zapisCsv( vysledek.vypisCsv());

    }


    qDebug()<<"pocet tagu: "<<elementy.count();
}

ZaznamMpvLogu Soubor::qDomElementToZaznamMpvLogu(QDomElement vstup)
{


    ZaznamMpvLogu zaznam;
    zaznam.imei= vstup.attributes().namedItem("imei").nodeValue();
    zaznam.rz= vstup.attributes().namedItem("rz").nodeValue();
    zaznam.pkt= vstup.attributes().namedItem("pkt").nodeValue();
    zaznam.lat= vstup.attributes().namedItem("lat").nodeValue();
    zaznam.lng= vstup.attributes().namedItem("lng").nodeValue();
    zaznam.tm= vstup.attributes().namedItem("tm").nodeValue();
    zaznam.events= vstup.attributes().namedItem("events").nodeValue();
    zaznam.rych= vstup.attributes().namedItem("rych").nodeValue();


     /*
    zaznam.= vstup.attributes().namedItem("").nodeValue();
    zaznam.= vstup.attributes().namedItem("").nodeValue();
    zaznam.= vstup.attributes().namedItem("").nodeValue();

    */

    return zaznam;
}



void Soubor::zapisCsv(QString vstup)
{
    QFile file(cestaSouboruHtml);
    //if(file.open(QIODevice::WriteOnly | QIODevice::Text))

    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        // We're going to streaming text to the file
        QTextStream stream(&file);

        stream << vstup;
        //stream << "ahoj";
        file.close();
        QString zapsano="Writing finished";
        qDebug() << zapsano;
        emit odesliChybovouHlasku(zapsano);
    }
    else
    {
        QString chybovaHlaska="soubor nelze zapsat";
        qDebug()<<chybovaHlaska;
        emit odesliChybovouHlasku(chybovaHlaska);
    }


}



