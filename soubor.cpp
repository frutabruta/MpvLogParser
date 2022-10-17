#include "soubor.h"


Soubor::Soubor(QObject *parent) : QObject(parent)
{
    qDebug()<<Q_FUNC_INFO;
    QString nazevSouboru="soubor.log";
    cestaSouboruLog=QCoreApplication::applicationDirPath()+"/"+nazevSouboru;
    sloupecky="turnus;line;evc;np;lat;lng;akt;takt;konc;tjr;pkt;tm;events;imei;type;conn;rych;smer;ridi;delta;ppevent;ppstatus;pperror";
    cestaSouboruCsv=zmenPriponu(cestaSouboruLog,"csv");
}

void Soubor::csvZapisKomplet(QString vstup)
{
    qDebug()<<Q_FUNC_INFO;
    QFile file(cestaSouboruCsv);
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

void Soubor::csvZapisSeznamZaznamu(QVector<ZaznamMpvLogu> &vstup)
{
    qDebug()<<Q_FUNC_INFO;
    QFile file(cestaSouboruCsv);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))

        // if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        // We're going to streaming text to the file
        QTextStream stream(&file);

        qDebug()<<"pocetZaznamu "<<vstup.count();


        QVector<QString> hlavicka;
        QStringList seznam=sloupecky.split(";");

        foreach(QString retezec,seznam)
        {
            hlavicka.push_back(retezec);
        }


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


bool Soubor::csvZapisZacatek(QVector<QString> &hlavicka, QFile &file)
{
    qDebug()<<Q_FUNC_INFO;
    // QFile file(cestaSouboruHtml);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        QStringList seznam=sloupecky.split(";");

        foreach(QString retezec,seznam)
        {
            hlavicka.push_back(retezec);
        }

        stream <<ZaznamMpvLogu::vypisCsvHlavicka(hlavicka);

        return 1;

    }
    else
    {
        QString chybovaHlaska="soubor nelze zapsat";
        qDebug()<<chybovaHlaska;
        emit odesliChybovouHlasku(chybovaHlaska);

    }
    return 0;
}

void Soubor::csvZapisJedenRadek(QVector<ZaznamMpvLogu> &vstup, QVector<QString> hlavicka, QFile &file, SqLiteZaklad &sqLiteZaklad)
{
    QTextStream stream(&file);

    int pocetZaznamu=vstup.count();
    for(int i=0;i<pocetZaznamu;i++)
    {
        stream <<vstup[i].vypisCsvRadek(hlavicka);
        sqLiteZaklad.vlozRadekDat("vozidlo",hlavicka,vstup[i].toQVectorQString(hlavicka));
        emit this->nastavProgressZapis(i);
    }
}

void Soubor::csvZapisKonec(QFile &file)
{
    qDebug()<<Q_FUNC_INFO;
    file.close();
    QString zapsano="Writing finished";
    qDebug() << zapsano;
    emit odesliChybovouHlasku(zapsano);
}

void Soubor::csvOtevri()
{
    qDebug()<<Q_FUNC_INFO;
    file.setFileName(cestaSouboruCsv);
    //if(file.open(QIODevice::WriteOnly | QIODevice::Text))

    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug()<<"soubor "<<cestaSouboruCsv<<" je otevreny";
        QTextStream stream(&file);
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


void Soubor::otevriSoubor()
{
    qDebug()<<Q_FUNC_INFO;
    QDomDocument doc("mydocument");

    qDebug()<<"cesta k souboru je "<<cestaSouboruLog;
    QFile file(cestaSouboruLog);


    if (!file.open(QIODevice::ReadOnly))
    {
        QString neotevruSoubor="soubor se nepovedlo otevrit";
        qDebug()<<neotevruSoubor;
        emit odesliChybovouHlasku(neotevruSoubor);
        return;
    }

    QByteArray vysledek;
    vysledek=file.readAll();
    file.close();

    emit    odesliChybovouHlasku("soubor otevren");
    qDebug()<<"uspech";
    QDomElement koren=doc.firstChildElement();

    qDebug()<<vysledek;

}


void Soubor::htmlVypis(QByteArray vstup)
{
    qDebug()<<Q_FUNC_INFO;
    int delka=vstup.length();
    QVector<QString> radky;
    QVector<QString> radkyHex;
    QString radek="";
    QString radekHex="";
    for (int i=0; i<(delka);i++)
    {

        radek+=htmlZavorky(vstup.at(i));
        radekHex+=" ";
        radekHex+=this->formatHex(this->hexDoplnNulu(vstup.at(i)));
        if (vstup.at(i) ==0x0D)
        {
            i++;
            radek+=htmlZavorky(vstup.at(i));
            radekHex+=" ";
            radekHex+=this->formatHex(this->hexDoplnNulu(vstup.at(i)));

            radky.append(radek);
            radkyHex.append(radekHex);
            radek="";
            radekHex="";

        }
    }
    qDebug()<<"konec";

    QString htmlSoubor="";
    QString htmlRadky="";
    for (int g=0;g<radkyHex.length();g++)
    {
        htmlRadky+=htmlTabulkaRadek(htmlTabulkaBunka(radky.at(g))+htmlTabulkaBunka(radkyHex.at(g)));
    }

    htmlSoubor=this->htmlWrapper(htmlRadky);
    htmlZapis(htmlSoubor);
    qDebug()<<htmlSoubor;

}


QString Soubor::htmlZavorky(char vstup)
{
    // qDebug()<<Q_FUNC_INFO;

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

QString Soubor::htmlTabulkaBunka(QString vstup)
{
    // qDebug()<<Q_FUNC_INFO;

    QString vystup="<td>"+vstup+"</td>";
    return vystup;
}

QString Soubor::htmlTabulkaRadek(QString vstup)
{
    //  qDebug()<<Q_FUNC_INFO;

    QString vystup="<tr>"+vstup+"</tr>";
    return vystup;
}

QString Soubor::htmlWrapper(QString vstup)
{
    //  qDebug()<<Q_FUNC_INFO;

    QString vystup="<!DOCTYPE html><html>"+htmlHlavicka()+"<body><table>"+vstup+"<table><body></html>";
    return vystup;
}

void Soubor::htmlZapis(QString vstup)
{
    //  qDebug()<<Q_FUNC_INFO;


    QFile file(cestaSouboruCsv);
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
    qDebug()<<Q_FUNC_INFO;

    QString vystup="<head>"+this->htmlStyl()+"</head>";
    // QString vystup2="<head><link rel=\"stylesheet\" href=\"mystyle.css\"></head>";
    return vystup;
}

QString Soubor::htmlStyl()
{
    qDebug()<<Q_FUNC_INFO;

    QString styl="*{font-family: \"Courier New\", Courier, \"Lucida Sans Typewriter\", \"Lucida Typewriter\", monospace;color: black;}";
    styl+="td{border: 1px solid blue;}";
    styl+="span.escape{color:red;}";
    styl+="span.format{color:orange;}";
    styl+="span.lf{color:green;}";

    QString vystup="<style>"+styl+"</style>";
    return vystup;
}




QVector<ZaznamMpvLogu> Soubor::logSouborNaRadky(QString fileName)
{
    qDebug()<<Q_FUNC_INFO;

    // zdroj: https://stackoverflow.com/questions/5444959/read-a-text-file-line-by-line-in-qt
    QFile inputFile(fileName);
    // QDomElement vystup;
    int counter=0;
    QVector<ZaznamMpvLogu> zaznamy;

    if (inputFile.open(QIODevice::ReadOnly))
    {

        //   qDebug()<<"soubor ma "<<counter<<" radku";
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            zaznamy.append(logZpracujRadek(line,counter));

            // qDebug()<<counter<<" "<<line;
            counter++;
            emit this->nastavProgressCteni(counter);
        }

        inputFile.close();
    }


    qDebug()<<"konec soubornaRadky";
    return zaznamy;
}


QVector<ZaznamMpvLogu> Soubor::logZpracujRadek(QString radek, int cisloRadku)
{
    //  qDebug()<<Q_FUNC_INFO;
    int zacatek =radek.indexOf("<");
    QString orezanyRadek;
    QVector<ZaznamMpvLogu> zaznamy2;
    if (zacatek<0)
    {
        qDebug()<<"zadna zprava na radku "<<QString::number(cisloRadku);
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
        qDebug()<<"elementy nejsou na radku:"<<QString::number(cisloRadku);
        return zaznamy2;
    }
    else
    {
        //      qDebug()<<"elementu je "<<pocetElementu;
    }
    for( int i=0;i<pocetElementu;i++)
    {
        ZaznamMpvLogu vysledek=qDomElementToZaznamMpvLogu(elementy.at(i).toElement());
        zaznamy2.push_back(vysledek);
    }
    return zaznamy2;
}


QVector<QString> Soubor::logZpracujRadekHledejHlavicky(QString radek, int cisloRadku, QVector<QString> seznamSloupecku )
{
    // qDebug()<<Q_FUNC_INFO;

    int zacatek =radek.indexOf("<");
    QString orezanyRadek;
    if (zacatek<0)
    {
        qDebug()<<"zadna zprava na radku "<<QString::number(cisloRadku);
        return seznamSloupecku;
    }
    else
    {
        orezanyRadek=radek.mid(zacatek);
    }
    QDomDocument soubor;
    soubor.setContent(orezanyRadek);
    QDomElement koren;
    koren=soubor.firstChildElement();
    QDomNodeList elementy=koren.elementsByTagName("V");

    int pocetElementu=elementy.count();
    if (pocetElementu==0)
    {

        qDebug()<<"elementy nejsou na radku:"<<QString::number(cisloRadku);
        return seznamSloupecku;
    }
    else
    {
        //  qDebug()<<"elementu na radku"<<QString::number(cisloRadku)<<" je "<<pocetElementu;
    }
    for( int i=0;i<pocetElementu;i++)
    {
        QDomNamedNodeMap atributy= elementy.at(i).toElement().attributes();

        for(int j=0;j<atributy.length();j++)
        {
            QString nazevAtributu= atributy.item(j).nodeName();
            if(!seznamSloupecku.contains(nazevAtributu))
            {
                if(nazevAtributu!="")
                {
                    seznamSloupecku.push_back(nazevAtributu);
                    qDebug()<<"novy atribut: "<<nazevAtributu;

                }
            }

        }
    }

    return seznamSloupecku;
}

int Soubor::slotSouborNaRadky2(QString fileName)
{
    qDebug()<<Q_FUNC_INFO;

    // zdroj: https://stackoverflow.com/questions/5444959/read-a-text-file-line-by-line-in-qt
    QFile inputFile(fileName);
    // QDomElement vystup;
    int counter=0;


    QFile csvcko(cestaSouboruCsv);

    QVector<QString> hlavicka;
    csvZapisZacatek(hlavicka,csvcko);

    SqLiteZaklad sqLiteZaklad;
    sqLiteZaklad.cestaKomplet=cestaSouboruSqLite;
    sqLiteZaklad.pripoj();
    if(!sqLiteZaklad.zahajTransakci())
    {
        qDebug()<<"transakci se nepovedlo zahajit";
    }


    if (inputFile.open(QIODevice::ReadOnly))
    {

        //   qDebug()<<"soubor ma "<<counter<<" radku";
        sqLiteZaklad.zrusSqlTabulku("vozidlo",hlavicka);
        sqLiteZaklad.zalozSqlTabulku("vozidlo",hlavicka);

        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QVector<ZaznamMpvLogu> zaznamy=logZpracujRadek(line,counter);

            counter++;

            csvZapisJedenRadek(zaznamy,hlavicka,csvcko, sqLiteZaklad);

            emit this->nastavProgressCteni(counter);
            qApp->processEvents();

        }

        inputFile.close();
    }

    sqLiteZaklad.ukonciTransakci();
    sqLiteZaklad.zavriDB();

    csvZapisKonec(csvcko);

    qDebug()<<"konec soubornaRadky";
    return counter;
}

QString Soubor::slotLogVyrobSeznamSloupecku(QString fileName)
{
    qDebug()<<Q_FUNC_INFO;
    QVector<QString> seznamSloupecku;
    QString vyslednaRadaSloupecku="";

    // zdroj: https://stackoverflow.com/questions/5444959/read-a-text-file-line-by-line-in-qt
    QFile inputFile(fileName);
    int counter=0;


    if (inputFile.open(QIODevice::ReadOnly))
    {

        //   qDebug()<<"soubor ma "<<counter<<" radku";
        QTextStream in(&inputFile);
        seznamSloupecku.clear();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            seznamSloupecku=logZpracujRadekHledejHlavicky(line,counter, seznamSloupecku);

            // qDebug()<<counter<<" "<<line;
            counter++;

            emit this->nastavProgressCteni(counter);
            qApp->processEvents();

        }

        vyslednaRadaSloupecku=vektorStringuOddelovac(seznamSloupecku,";");
        qDebug()<<"hlavicky: "<<vyslednaRadaSloupecku;
        inputFile.close();
    }

    qDebug()<<"konec soubornaRadky";
    return vyslednaRadaSloupecku;
}


ZaznamMpvLogu Soubor::qDomElementToZaznamMpvLogu(QDomElement vstup)
{
    ZaznamMpvLogu zaznam;

    int pocetAtributu=0;
    pocetAtributu=vstup.attributes().count();

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
    // qDebug()<<"konec Soubor::qDomElementToZaznamMpvLogu";
    return zaznam;
}

int Soubor::spocitejRadkySouboru(QString fileName)
{
    qDebug()<<Q_FUNC_INFO;
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

QString Soubor::vektorStringuOddelovac(QVector<QString> vstup, QString oddelovac)
{
    QString vystup="";
    if (vstup.isEmpty())
    {
        return "";
    }
    if (vstup.count()==1)
    {
        return vstup.first();
    }

    for(int i=0;i<vstup.count()-1;i++)
    {
        vystup+=vstup.at(i)+oddelovac;
    }
    vystup+=vstup.last();
    return vystup;
}

QString Soubor::zmenPriponu(QString vstup,QString pripona)
{
    qDebug()<<Q_FUNC_INFO;

    if (vstup.isEmpty())
    {
        return "";
    }

    QString zbytek=vstup.split(".").first();
    QString staraPripona=vstup.split(".").last();

    qDebug()<<"zbytek: "<<zbytek<<" pripona: "<<staraPripona;

    QString vystup=zbytek+"."+pripona;

    return vystup;
}
