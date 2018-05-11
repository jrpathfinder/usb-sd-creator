////////////////////////////////////////////////////////////////////////////////
//      This file is part of LibreELEC - http://www.libreelec.tv
//      Copyright (C) 2013-2015 RasPlex project
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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class QNetworkReply;
class QUrl;

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);
    ~DownloadManager();

    QNetworkReply* get(const QUrl& url);
    QNetworkReply* post(const QUrl& url);

signals:
    void downloadComplete(const QByteArray&);
    void partialData(const QByteArray, qlonglong total);
    void downloadProgress(qint64, qint64);
    void downloadError(const QString);

public slots:
    void cancelDownload();

private slots:
    void handleProgress(qint64,qint64);
    void slotError(QNetworkReply::NetworkError errorCode);
    void handleGetFinished(QNetworkReply*);
    void handleReadyRead();

private:
    QNetworkAccessManager* manager;
    QNetworkReply* latestReply;

    enum {
        RESPONSE_OK = 200,
        RESPONSE_201 = 201,
        RESPONSE_PARTIAL = 206,
        RESPONSE_FOUND = 302,
        RESPONSE_REDIRECT = 307,
        RESPONSE_BAD_REQUEST = 400,
        RESPONSE_NOT_FOUND = 404
    };
};

#endif // DOWNLOADMANAGER_H
