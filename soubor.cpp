#include "soubor.h"


Soubor::Soubor(QObject *parent)
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
        seznam.push_front("kanal");
        seznam.push_front("timestamp");
        seznam.push_front("message_tag");


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

void Soubor::csvZapisJedenRadek(QVector<ZaznamMpvLogu> &vstup, QVector<QString> &hlavicka, QFile &file,SqLiteBase &sqLiteZaklad, QSqlQuery &query)
{
    QTextStream stream(&file);

    int pocetZaznamu=vstup.count();

    ////////


    for(int i=0;i<pocetZaznamu;i++)
    {
        stream <<vstup[i].vypisCsvRadek(hlavicka);
        sqLiteZaklad.insertDataRow(query,vstup[i].toQVectorQString(hlavicka));

    }
    //emit this->nastavProgressZapis(i);
}

QSqlQuery Soubor::prepareInsertQuery(QVector<QString> &hlavicka, SqLiteBase &sqLiteZaklad)
{
    QString nazevTabulky="vozidlo";

    /////////////////


    //qDebug() << Q_FUNC_INFO;
    /*
    if (hlavicka.size() != data.size())
    {
        qDebug() << "insertDataRow failed: header/data size mismatch";
        qDebug() << "hlavicka size:" << hlavicka.size();
        qDebug() << "data size:" << data.size();
        return false;
    }*/

    /*
    if (!sqLiteZaklad.dbFile.isOpen())
    {
        qDebug() << "insertDataRow failed: database is not open";
        qDebug() << sqLiteZaklad.dbFile.lastError().text();
        return;
    }
*/

    QString safeTableName = sqLiteZaklad.sanitizeSqlIdentifier(nazevTabulky);

    QVector<QString> safeColumns;
    QVector<QString> placeholders;

    for (int i = 0; i < hlavicka.size(); i++)
    {
        safeColumns.append(sqLiteZaklad.sanitizeSqlIdentifier(hlavicka.at(i)));
        placeholders.append("?");
    }

    QString queryText = QString("INSERT INTO %1 (%2) VALUES (%3);")
                            .arg(
                                safeTableName,
                                safeColumns.join(","),
                                placeholders.join(",")
                                );

    //qDebug() << queryText;

    QSqlQuery query(sqLiteZaklad.dbFile);

    if (!query.prepare(queryText))
    {
        qDebug() << "Prepare failed:";
        qDebug() << query.lastError().text();
        qDebug() << queryText;
    }

    return query;
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


    /////outer



    QDomNamedNodeMap atributyParent= koren.toElement().attributes();

    for(int i=0;i<atributyParent.length();i++)
    {
        QString nazevAtributu= atributyParent.item(i).nodeName();
        if(!seznamSloupecku.contains(nazevAtributu))
        {
            if(nazevAtributu!="")
            {
                seznamSloupecku.push_back(nazevAtributu);
                qDebug()<<"novy atribut: "<<nazevAtributu;

            }
        }
    }



    ///// inner
    QDomNodeList elementy=koren.elementsByTagName("V");
    if(elementy.isEmpty())
    {
        elementy=koren.elementsByTagName("vlak");
    }
    if(elementy.isEmpty())
    {
        if(koren.nodeName()=="position")
        {
            elementy=soubor.elementsByTagName("position");
        }
    }

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
    ////////

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

    SqLiteBase sqLiteZaklad;
    sqLiteZaklad.dbFilePath=cestaSouboruSqLite;

    if(!sqLiteZaklad.initialize())
    {
        qDebug()<<"initialization failed";
        return 0;
    }
    else
    {
        if(!sqLiteZaklad.transactionStart())
        {
            qDebug()<<"transakci se nepovedlo zahajit";
            return 0;
        }
        else
        {
            qDebug() << "transaction started:" << sqLiteZaklad.dbFile.driver()->hasFeature(QSqlDriver::Transactions);
            if (inputFile.open(QIODevice::ReadOnly))
            {
                //   qDebug()<<"soubor ma "<<counter<<" radku";
                sqLiteZaklad.tableDelete("vozidlo",hlavicka);

                if(!sqLiteZaklad.tableCreate("vozidlo",hlavicka))
                {
                    qDebug()<<"Failed to create table";
                    inputFile.close();
                    return false;
                }


                QElapsedTimer timer;

                qint64 parseTimeMs = 0;
                qint64 writeTimeMs = 0;


                QTextStream in(&inputFile);

                QSqlQuery insertQuery = prepareInsertQuery(hlavicka,sqLiteZaklad);

                while (!in.atEnd())
                {
                    QString line = in.readLine();
                    timer.start();
                    //     QVector<ZaznamMpvLogu> zaznamy=logZpracujRadek(line,counter);
                    QVector<ZaznamMpvLogu> zaznamy=logZpracujRadekStream(line,counter);
                    parseTimeMs += timer.elapsed();
                    timer.restart();
                    counter++;

                    csvZapisJedenRadek(zaznamy,hlavicka,csvcko, sqLiteZaklad, insertQuery);
                    writeTimeMs += timer.elapsed();

                    if ((counter % 1000) == 0)
                    {
                        this->nastavProgressCteni(counter);
                        //qDebug() << "line" << counter << "parse total:" << parseTimeMs << "ms" << "write total:" << writeTimeMs << "ms";
                    }
                }                
                inputFile.close();
            }

            sqLiteZaklad.transactionStop();
            sqLiteZaklad.dbClose();

            csvZapisKonec(csvcko);
        }

        qDebug()<<"konec soubornaRadky";
        return counter;
    }


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
        zaznam.obsah.insert(atribut,hodnota);
    }
    // qDebug()<<"konec Soubor::qDomElementToZaznamMpvLogu";
    return zaznam;
}

ZaznamMpvLogu Soubor::attributesToZaznamMpvLogu(const QXmlStreamAttributes &vstup)
{
    ZaznamMpvLogu zaznam;

    foreach(QXmlStreamAttribute hhh, vstup)
    {
        zaznam.obsah.insert(hhh.name().toString(),hhh.value().toString());
    }

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




QVector<ZaznamMpvLogu> Soubor::logZpracujRadekStream(QString radek, int cisloRadku)
{
    //  qDebug()<<Q_FUNC_INFO;
    int zacatek =radek.indexOf("<");
    QStringView orezanyRadek;
    QVector<ZaznamMpvLogu> zaznamy2;
    if (zacatek<0)
    {
        qDebug()<<"zadna zprava na radku "<<QString::number(cisloRadku);
        return zaznamy2;
    }
    else
    {
        orezanyRadek=QStringView(radek).mid(zacatek);
        //  qDebug()<<" orezany radek "<<orezanyRadek;
    }

    QString hlavicka=radek.left(zacatek);


    QStringList elementyHlavicky=hlavicka.split("\t");

  //  QDateTime timestamp=QDateTime::fromString(elementyHlavicky.value(0),"dd.MM.yyyy hh:mm:ss");


  //  QString timeStampString=timestamp.toString(Qt::ISODate);
    QString kanal=elementyHlavicky.value(1);
    // qDebug()<<"timestamp "<<timestamp<<" kanal "<<elementyHlavicky.value(1)<<" ip "<<elementyHlavicky.value(2);

    /*
    if(!timestamp.isValid())
    {
        qDebug()<<"puvodniTimestamp "<<elementyHlavicky.value(0);
    }*/

    ////////////////////////

    QXmlStreamReader reader(QStringView(radek).mid(zacatek));
    QTextStream errorStream(stderr);


    //reader.readElementText(QXmlStreamReader::IncludeChildElements);

    QXmlStreamAttributes atributyZprava;
    QString elementName="";

    int pocetElementu=0;



    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();

        //QString currentToken = reader.tokenString();


        if(token == QXmlStreamReader::StartElement)
        {
            QXmlStreamAttributes atributy=reader.attributes();
            elementName=reader.name().toString();
            if(elementName=="V")
            {            
                ZaznamMpvLogu vysledek=attributesToZaznamMpvLogu(atributy);

                vysledek.obsah.insert("timestamp",prevedCas(elementyHlavicky.value(0)));
                vysledek.obsah.insert("kanal",kanal);
                vysledek.obsah.insert("message_tag",elementName);

                zaznamy2.push_back(vysledek);
            }
            else if(elementName=="APC")
            {
                ZaznamMpvLogu vysledek=attributesToZaznamMpvLogu(atributy);
                vysledek.obsah.insert("timestamp",prevedCas(elementyHlavicky.value(0)));
                vysledek.obsah.insert("kanal",kanal);
                vysledek.obsah.insert("message_tag",elementName);
                zaznamy2.push_back(vysledek);
            }

            // metro
            else if(elementName=="vlak")
            {              
                atributy.append(atributyZprava);
                ZaznamMpvLogu vysledek=attributesToZaznamMpvLogu(atributy);
                vysledek.obsah.insert("message_tag",elementName);
                zaznamy2.push_back(vysledek);
            }
            else if(elementName=="M")
            {

                atributyZprava=atributy;
                //zaznamy2.push_back(vysledek);
            }

            //vlaky
            else if(elementName=="position")
            {

                //  atributy.append(atributyZprava);
                ZaznamMpvLogu vysledek=attributesToZaznamMpvLogu(atributy);
                vysledek.obsah.insert("message_tag",elementName);
                zaznamy2.push_back(vysledek);
                //qDebug()<<"position";

            }

            pocetElementu++;

        }

        //  emit signalNastavProgress(reader.lineNumber());

    }



    /////////////////////////





    if (pocetElementu==0)
    {
        qDebug()<<"elementy nejsou na radku:"<<QString::number(cisloRadku);

    }

    return zaznamy2;
}



QString Soubor::prevedCas(const QString &s)
{
    if (s.length() < 19)
        return QString();

    QString iso(19, QChar());
    const QChar *src = s.constData();
    QChar *dst = iso.data();

    // yyyy
    dst[0]=src[6]; dst[1]=src[7]; dst[2]=src[8]; dst[3]=src[9];
    dst[4]='-';
    // MM
    dst[5]=src[3]; dst[6]=src[4];
    dst[7]='-';
    // dd
    dst[8]=src[0]; dst[9]=src[1];
    dst[10]='T';
    // hh:mm:ss (already in the right order, just copy the tail)
    for (int i = 0; i < 8; ++i)
        dst[11+i] = src[11+i];

    return iso;
}
