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
