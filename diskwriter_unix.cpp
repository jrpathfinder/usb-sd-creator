
#include "diskwriter_unix.h"

#include <QDebug>
#include <unistd.h>
#include <QStorageInfo>
#include <QProcess>
#include <QThread>
#include <QMessageBox>
#ifdef Q_OS_MAC
#include <QProcess>
#endif


DiskWriter_unix::DiskWriter_unix(QObject *parent) :
    DiskWriter(parent)
{
    isCancelled = false;
}

DiskWriter_unix::~DiskWriter_unix()
{
    if (isOpen())
        close();
}

bool DiskWriter_unix::open(const QString& device)
{
    dev.setFileName(device);
#ifdef Q_OS_MAC
    // write to RAW device, this is MUCH faster
    QString rawdev(device);
    rawdev.replace(QString("/dev/"),QString("/dev/r"));

    // unmount the device
    QProcess unmount;
    unmount.start("diskutil unmountDisk "+rawdev, QIODevice::ReadOnly);
    unmount.waitForStarted();
    unmount.waitForFinished();
    dev.setFileName(rawdev);
#endif

    return dev.open(QFile::WriteOnly);
}

void DiskWriter_unix::close()
{
    dev.close();
}

void DiskWriter_unix::sync()
{
    if (this->isOpen())
        fsync(dev.handle());
}

bool DiskWriter_unix::isOpen()
{
    return dev.isOpen();
}

bool DiskWriter_unix::write(const QByteArray &data)
{
    return dev.write(data);
}

bool DiskWriter_unix::mount(const QString& what, const QString& where ) const
{
    QProcess cmd;
    cmd.start("mount " + what + " " +where, QIODevice::ReadWrite);
    cmd.waitForStarted();
    cmd.waitForFinished();


    //mount("/dev/hdb", "/cdrom", MS_MGC_VAL | MS_RDONLY | MS_NOSUID, "");

    qDebug() << "mount: checking" << what;
    if (checkIsMounted(what) == true) {
        qDebug() << "mount: correct!";
        return true;
    }else{
        QMessageBox::critical(NULL, QObject::tr("Mount Error!"),
        QObject::tr("Не удаётся смонтировать раздел для записи токена!"));
    }
    qDebug() << "mount: failed!";
    return false;
}

bool DiskWriter_unix::checkIsMounted(const QString &device) const
{
    qDebug() << "checkIsMounted " << device;
    char buf[2];
    QFile mountsFile("/proc/mounts");
    if (!mountsFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open" << mountsFile.fileName();
        return true;
    }

    // QFile::atEnd() is unreliable for proc
    while (mountsFile.read(buf, 1) > 0) {
        QString line = mountsFile.readLine();
        line.prepend(buf[0]);
        if (line.contains(device))
            return true;
    }

    return false;
}
/**
 * Copy security token to USB Flash drive after image file have been burnt
 * @brief DiskWriter_unix::copyToUsb
 * @param jtw
 */
void DiskWriter_unix::copyToUsb(const QString& device, const QString& jtw){
    /**
     * Try to mount new device
     * @brief cmd
     */
    mount(device+"1","/media/FDP");
    //qDebug() << "mount: success!";
    // Mounted volume root path
    QString storageRootPath;
    // Where to copy security token
    QString securityTokenLocation= "/fmos_token.txt";
    // write sec token to temp local security file
    QFile srcFile;
    srcFile.setFileName("tmp_token.txt");
    if(!srcFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() <<"Could not open for writing";
    }else{
        QTextStream stream(&srcFile);
       stream << jtw << endl;
    }
    srcFile.close();
    //Iterate over available mounted volumes:
    bool found = false;
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        qDebug()<<"Storage Root Path" << storage.rootPath();
        if (
                storage.isValid()
                &&
                storage.isReady()
                &&
                !storage.name().isEmpty()
                &&
                storage.name() == "FDP"
            ) {
            if (!storage.isReadOnly()) {
               found = true;
               storageRootPath = storage.rootPath();
               //Destination file to copy the token
               QString destPath  = storageRootPath+securityTokenLocation;
               qDebug() << "Copy token to USB Dest Path:" << destPath;
               if (QFile::exists(destPath)) QFile::remove(destPath);
               qDebug() << QFile::copy(srcFile.fileName(),destPath);
               qDebug("copied");
            }
        }
    }
    if(!found){
        QMessageBox::critical(NULL, QObject::tr("Dismount Error"),
                              QObject::tr("Не удается найти раздел для записи токена!"));
    }
}
