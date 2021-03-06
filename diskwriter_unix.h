#ifndef DISKWRITER_UNIX_H
#define DISKWRITER_UNIX_H

#include "diskwriter.h"
#include <QFile>

class DiskWriter_unix : public DiskWriter
{
    Q_OBJECT
public:
    explicit DiskWriter_unix(QObject *parent = 0);
    ~DiskWriter_unix();

private:
    QFile dev;

    bool open(const QString &device);
    void close();
    void sync();
    bool isOpen();
    bool write(const QByteArray &data);
    void copyToUsb(const QString & device, const QString& jtw);
    bool mount(const QString& what, const QString& where ) const;
    bool checkIsMounted(const QString& device) const;
};

#endif // DISKWRITER_UNIX_H
