#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QDebug>

class JsonData
{
  public:
    JsonData() {}

    JsonData(QString name, QString id, QString url, QList<QMap<QString, QVariant>> &images)
    {
         addData(name, id, url, images);
    }

    void addData(QString name, QString id, QString url, QList<QMap<QString, QVariant>> &images)
    {
        JsonData::name = name;
        JsonData::id = id;
        JsonData::url = url;
        JsonData::images = images;
    }

    bool operator== (const JsonData &data) const
    {
        if (data.name == this->name)
            return true;

        return false;
    }

    QString name;
    QString id;
    QString url;
    QList<QVariantMap> images;
};

class JsonParser
{
public:
    JsonParser() {}
    JsonParser(const QByteArray &data);
    JsonParser(const QByteArray &data, int prs);
    void addExtra(const QByteArray &data, const QString label);
    void parseAndSet(const QByteArray &data, const QString label);
    void parse(const QByteArray &data);
    QList<JsonData> getJsonData() const;
    QString getJTW() const;
    QString getUrl() const;
    QString getSha256() const;
    QString getFilename() const;

private:
    QList<JsonData> dataList;
    QString jtw;
    QString url;
    QString sha256;
    QString filename;
};

#endif // JSONPARSER_H
