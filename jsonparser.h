////////////////////////////////////////////////////////////////////////////////
//      This file is part of LibreELEC - http://www.libreelec.tv
//      Copyright (C) 2016 Team LibreELEC
//
//  LibreELEC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  LibreELEC is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with LibreELEC.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

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

private:
    QList<JsonData> dataList;
    QString jtw;
};

#endif // JSONPARSER_H
