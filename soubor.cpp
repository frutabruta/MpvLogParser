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
    //qDebug()<<"delka velikost je "<<vstup.size()<<" delka je "<<vstup.length();
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
           // qDebug()<<"konec prikazu";

        }

       // qDebug()<<i<<" "<<vstup[i];
    }
    qDebug()<<"konec";






    QString htmlSoubor="";
    QString htmlRadky="";
    for (int g=0;g<radkyHex.length();g++)
    {
        htmlRadky+=tabulkaRadek(tabulkaBunka(radky.at(g))+tabulkaBunka(radkyHex.at(g)));
    //    qDebug()<<radky.at(g)<<" "<<radkyHex.at(g);
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
    qDebug()<<"";

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
    qDebug()<<"";

    QString vystup=QString::number(vstup,16);
    if (vstup<0x10)
    {
        vystup="0"+vystup;
    }
    return vystup;

}

QString Soubor::tabulkaBunka(QString vstup)
{
    qDebug()<<"";

    QString vystup="<td>"+vstup+"</td>";
    return vystup;
}

QString Soubor::tabulkaRadek(QString vstup)
{
    qDebug()<<"";

    QString vystup="<tr>"+vstup+"</tr>";
    return vystup;
}

QString Soubor::wrapper(QString vstup)
{
    qDebug()<<"";

    QString vystup="<!DOCTYPE html><html>"+htmlHlavicka()+"<body><table>"+vstup+"<table><body></html>";
    return vystup;
}

void Soubor::zapisHtml(QString vstup)
{
    qDebug()<<"";


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
    qDebug()<<"";

    QString vystup="<head>"+this->htmlStyl()+"</head>";
   // QString vystup2="<head><link rel=\"stylesheet\" href=\"mystyle.css\"></head>";
    return vystup;
}

QString Soubor::htmlStyl()
{
    qDebug()<<"";

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
    qDebug()<<"";


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


QVector<ZaznamMpvLogu> Soubor::souborNaRadky(QString fileName)
{
    qDebug()<<"Soubor::souborNaRadky";

    // zdroj: https://stackoverflow.com/questions/5444959/read-a-text-file-line-by-line-in-qt
    QFile inputFile(fileName);
    QDomElement vystup;
    int counter=0;
    QVector<ZaznamMpvLogu> zaznamy;

    if (inputFile.open(QIODevice::ReadOnly))
    {



        qDebug()<<"soubor ma "<<counter<<" radku";
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            zaznamy.append(zpracujRadek(line));
            // qDebug()<<counter<<" "<<line;
            counter++;
            emit this->nastavProgressCteni(counter);
        }

        inputFile.close();
    }

    qDebug()<<"konec soubornaRadky";
    return zaznamy;
}


QVector<ZaznamMpvLogu> Soubor::zpracujRadek(QString radek)
{
   // qDebug()<<"Soubor::zpracujRadek";
    int zacatek =radek.indexOf("<");
    QString orezanyRadek;
    QVector<ZaznamMpvLogu> zaznamy2;
    if (zacatek<0)
    {
        qDebug()<<"zadna zprava na tomto radku";
        return zaznamy2;
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

    int pocetElementu=elementy.count();
    if (pocetElementu==0)
    {
        qDebug()<<"elementy nejsou";
        return zaznamy2;
    }
    else
    {
   //      qDebug()<<"elementu je "<<pocetElementu;
    }
    for( int i=0;i<pocetElementu;i++)
    {
    //    qDebug()<<"element cislo "<<i;
        ZaznamMpvLogu vysledek=qDomElementToZaznamMpvLogu(elementy.at(i).toElement());
       // zapisCsvKomplet( vysledek.vypisCsv());

    //    qDebug()<<"jdu appendovat element cislo "<<i<<" ktery ma ";//<<vysledek.Obsah.count()<<" elementu do struktury s poctem "<<zaznamy2.count();
        zaznamy2.push_back(vysledek);

    }


 //   qDebug()<<"pocet tagu: "<<pocetElementu;
    return zaznamy2;
}

ZaznamMpvLogu Soubor::qDomElementToZaznamMpvLogu(QDomElement vstup)
{
   // qDebug()<<"Soubor::qDomElementToZaznamMpvLogu";

    ZaznamMpvLogu zaznam;
    /*
    zaznam.imei= vstup.attributes().namedItem("imei").nodeValue();
    zaznam.rz= vstup.attributes().namedItem("rz").nodeValue();
    zaznam.pkt= vstup.attributes().namedItem("pkt").nodeValue();
    zaznam.lat= vstup.attributes().namedItem("lat").nodeValue();
    zaznam.lng= vstup.attributes().namedItem("lng").nodeValue();
    zaznam.tm= vstup.attributes().namedItem("tm").nodeValue();
    zaznam.events= vstup.attributes().namedItem("events").nodeValue();
    zaznam.rych= vstup.attributes().namedItem("rych").nodeValue();
*/

    //QMap<QString,QString> vystup;
    int pocetAtributu=0;
    pocetAtributu=vstup.attributes().count();


  //  qDebug()<<"pocet atributu je "<<pocetAtributu;
    if (pocetAtributu<=0)
    {

        return zaznam;
    }


    for(int i=0;i<pocetAtributu;i++)
    {
        QString atribut=vstup.attributes().item(i).nodeName();
        QString hodnota=vstup.attributes().item(i).nodeValue();
       // qDebug()<<" vypis atributu "<<atribut<<" "<<hodnota;
        zaznam.Obsah.insert(atribut,hodnota);
    }


     /*
    zaznam.= vstup.attributes().namedItem("").nodeValue();
    zaznam.= vstup.attributes().namedItem("").nodeValue();
    zaznam.= vstup.attributes().namedItem("").nodeValue();

    */
   // qDebug()<<"konec Soubor::qDomElementToZaznamMpvLogu";
    return zaznam;
}



void Soubor::zapisCsvKomplet(QString vstup)
{
    qDebug()<<"Soubor::zapisCsvKomplet";
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

void Soubor::zapisCsvSeznamZaznamu(QVector<ZaznamMpvLogu> &vstup)
{
    qDebug()<<"Soubor::zapisCsvSeznamZaznamu";
    QFile file(cestaSouboruHtml);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))

   // if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        // We're going to streaming text to the file
        QTextStream stream(&file);

        qDebug()<<"pocetZaznamu "<<vstup.count();


        QVector<QString> hlavicka;
        hlavicka.push_back("imei");
        hlavicka.push_back("pkt");
        hlavicka.push_back("lat");
        hlavicka.push_back("lng");
        hlavicka.push_back("tm");
        hlavicka.push_back("events");
        hlavicka.push_back("type");
        hlavicka.push_back("line");
        hlavicka.push_back("conn");
        hlavicka.push_back("rych");
        hlavicka.push_back("smer");
        hlavicka.push_back("evc");
        hlavicka.push_back("np");
        hlavicka.push_back("turnus");
        hlavicka.push_back("ridic");
        hlavicka.push_back("akt");
        hlavicka.push_back("konc");
        hlavicka.push_back("delta");
        hlavicka.push_back("ppevent");
        hlavicka.push_back("ppstatus");
        hlavicka.push_back("pperror");


        stream <<ZaznamMpvLogu::vypisCsvHlavicka(hlavicka);

        int pocetZaznamu=vstup.count();
        for(int i=0;i<pocetZaznamu;i++)
        {



               stream <<vstup[i].vypisCsvRadek(hlavicka);
               emit this->nastavProgressZapis(i);
        }

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

void Soubor::otevriCsv()
{
    file.setFileName(cestaSouboruHtml);
    //if(file.open(QIODevice::WriteOnly | QIODevice::Text))

    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        // We're going to streaming text to the file
        qDebug()<<"soubor "<<cestaSouboruHtml<<" je otevreny";

        QTextStream stream(&file);

       // stream << vstup;
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
/*
void Soubor::zapisRadekCsv(QString vstup)
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
*/

/*
void Soubor::zavriCsv(QString vstup)
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
*/


int Soubor::spocitejRadkySouboru(QString fileName)
{
    qDebug()<<"spocitejRadkySouboru";

    // zdroj: https://stackoverflow.com/questions/5444959/read-a-text-file-line-by-line-in-qt
    QFile inputFile(fileName);

    int counter=0;

    if (inputFile.open(QIODevice::ReadOnly))
    {

        QTextStream in_c(&inputFile);
        while (!in_c.atEnd())
        {
            //QString line = in_c.readLine();
            in_c.readLine();
            counter++;
        }
        inputFile.close();
    }

      qDebug()<<"soubor ma "<<counter<<" radku";
    qDebug()<<"konec soubornaRadky";
    return counter;
}



