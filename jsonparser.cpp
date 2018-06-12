#include "jsonparser.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStandardPaths>
#include <QCollator>
#include <algorithm>

JsonParser::JsonParser(const QByteArray &data, int prs)
{
    qDebug() << prs;
    parse(data);
}

JsonParser::JsonParser(const QByteArray &data)
{
    parseAndSet(data, "");
}
void JsonParser::addExtra(const QByteArray &data, const QString label)
{
    parseAndSet(data, label);
}
/**
 * Authorization POST request's json parser:
 * @brief JsonParser::parse
 * @param data
 */
void JsonParser::parse(const QByteArray &data)
{
    qDebug() << "parse data:" << data;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
    QJsonObject   jsonObject   = jsonDocument.object();
    for (QJsonObject::Iterator item  = jsonObject.begin();
                               item != jsonObject.end();
                               item++)
    {
        QJsonObject userObject = item.value().toObject();
        for (QJsonObject::Iterator its  = userObject.begin();
                                   its != userObject.end();
                                   its++)
        {
            if(its.key() == "jwt"){
                qDebug() << "JTW" << its.value().toString();
                jtw = its.value().toString();
            }
        }
    }
}
/**
 * Parsing JSON Data from reply of QByteArray
 * @brief JsonParser::parseAndSet
 * @param data
 * @param label
 */
void JsonParser::parseAndSet(const QByteArray &data, const QString label)
{
    qDebug() << "parseAndSet data:" << data << label;
    qDebug() << "parse data:" << data;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
    QJsonObject   jsonObject   = jsonDocument.object();
    for (QJsonObject::Iterator item  = jsonObject.begin();
                               item != jsonObject.end();
                               item++)
    {
        QJsonObject userObject = item.value().toObject();
        for (QJsonObject::Iterator its  = userObject.begin();
                                   its != userObject.end();
                                   its++)
        {
            qDebug() << its.key();
            if(its.key() == "url"){
                qDebug() << "URL" << its.value().toString();
                url    = its.value().toString();
            }else if(its.key() == "sha256"){
                qDebug() << "sha256" << its.value().toString();
                sha256 = its.value().toString();
            }else if(its.key() == "name"){
                qDebug() << "file name" << its.value().toString();
                filename = its.value().toString();
            }
        }
    }

}

QList<JsonData> JsonParser::getJsonData() const
{
    return dataList;
}

QString JsonParser::getJTW() const
{
    return jtw;
}
QString JsonParser::getUrl() const
{
    return url;
}
QString JsonParser::getSha256() const
{
    return sha256;
}
QString JsonParser::getFilename() const
{
    return filename;
}
