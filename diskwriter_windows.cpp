#include "diskwriter_windows.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStorageInfo>

DiskWriter_windows::DiskWriter_windows(QObject *parent) :
    DiskWriter(parent),
    hVolume(INVALID_HANDLE_VALUE),
    hRawDisk(INVALID_HANDLE_VALUE)
{
    isCancelled = false;
}

DiskWriter_windows::~DiskWriter_windows()
{
    if (isOpen())
        close();
}

bool DiskWriter_windows::open(const QString &device)
{
#ifdef WINDOWS_DUMMY_WRITE
    dev.setFileName(device);
    return dev.open(QFile::WriteOnly);
#endif

    hVolume = getHandleOnVolume(device, GENERIC_WRITE);
    if (hVolume == INVALID_HANDLE_VALUE)
        return false;

    if (!getLockOnVolume(hVolume)) {
        close();
        return false;
    }

    if (isVolumeMounted(hVolume) && !unmountVolume(hVolume)) {
        close();
        return false;
    }

    hRawDisk = getHandleOnDevice(device, GENERIC_WRITE);
    if (hRawDisk == INVALID_HANDLE_VALUE) {
        close();
        return false;
    }

    return true;
}

void DiskWriter_windows::close()
{
#ifdef WINDOWS_DUMMY_WRITE
    dev.close();
#endif

    if (hRawDisk != INVALID_HANDLE_VALUE) {
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
    }

    if (hVolume != INVALID_HANDLE_VALUE) {
        removeLockOnVolume(hVolume);
        CloseHandle(hVolume);
        hVolume = INVALID_HANDLE_VALUE;
    }
}

void DiskWriter_windows::sync()
{
#ifdef WINDOWS_DUMMY_WRITE
    return;
#endif

    FlushFileBuffers(hVolume);
}

bool DiskWriter_windows::isOpen()
{
#ifdef WINDOWS_DUMMY_WRITE
    return dev.isOpen();
#endif

    return (hRawDisk != INVALID_HANDLE_VALUE && hVolume != INVALID_HANDLE_VALUE);
}

bool DiskWriter_windows::write(const QByteArray &data)
{
#ifdef WINDOWS_DUMMY_WRITE
    return dev.write(data);
#endif

    DWORD byteswritten;
    bool ok;
    ok = WriteFile(hRawDisk, data.constData(), data.size(), &byteswritten, NULL);
    if (!ok) {
        const QString errText = errorAsString(GetLastError());
        QMessageBox::critical(NULL, QObject::tr("Write Error"),
                              QObject::tr("An error occurred when attempting to write data to handle.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
    }

    return ok;
}

// Adapted from win32 DiskImager
HANDLE DiskWriter_windows::getHandleOnDevice(const QString& device, DWORD access)
{
    HANDLE hDevice;
    const QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(deviceNumberFromName(device));

    //qDebug() << "getHandleOnDevice" << devicename;

    hDevice = CreateFile(devicename.toStdWString().c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    //if (hDevice == INVALID_HANDLE_VALUE) {
    //    const QString errText = errorAsString(GetLastError());
    //    QMessageBox::critical(NULL, QObject::tr("Device Error"),
    //                          QObject::tr("An error occurred when attempting to get a handle on the device.\n"
    //                                      "Error %1: %2").arg(GetLastError()).arg(errText));
    //}
    return hDevice;
}

// Adapted from win32 DiskImager
HANDLE DiskWriter_windows::getHandleOnVolume(const QString &volume, DWORD access)
{
    HANDLE hVolume;
    QString volumename = "\\\\.\\" + volume;
    if (volumename.endsWith("\\"))
        volumename.chop(1);

    //qDebug() << "getHandleOnVolume" << volumename;

    hVolume = CreateFile(volumename.toStdWString().c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hVolume == INVALID_HANDLE_VALUE) {
        const QString errText = errorAsString(GetLastError());
        QMessageBox::critical(NULL, QObject::tr("Volume Error"),
                              QObject::tr("An error occurred when attempting to get a handle on the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
    }

    return hVolume;
}

// Adapted from win32 DiskImager
bool DiskWriter_windows::getLockOnVolume(HANDLE handle) const
{
    DWORD junk;
    bool ok;
    ok = DeviceIoControl(handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!ok) {
        const QString errText = errorAsString(GetLastError());
        QMessageBox::critical(NULL, QObject::tr("Lock Error"),
                              QObject::tr("An error occurred when attempting to lock the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
    }

    return ok;
}

// Adapted from win32 DiskImager
bool DiskWriter_windows::removeLockOnVolume(HANDLE handle) const
{
    DWORD junk;
    bool ok;
    ok = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!ok) {
        const QString errText = errorAsString(GetLastError());
        QMessageBox::critical(NULL, QObject::tr("Unlock Error"),
                              QObject::tr("An error occurred when attempting to unlock the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
    }

    return ok;
}

// Adapted from win32 DiskImager
bool DiskWriter_windows::unmountVolume(HANDLE handle) const
{
    DWORD junk;
    bool ok;
    ok = DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!ok) {
        const QString errText = errorAsString(GetLastError());
        QMessageBox::critical(NULL, QObject::tr("Dismount Error"),
                              QObject::tr("An error occurred when attempting to dismount the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
    }

    return ok;
}

// Adapted from win32 DiskImager
bool DiskWriter_windows::isVolumeMounted(HANDLE handle) const
{
    DWORD junk;
    return DeviceIoControl(handle, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &junk, NULL);
}

ULONG DiskWriter_windows::deviceNumberFromName(const QString &device)
{
    QString volumename = "\\\\.\\" + device;
    if (volumename.endsWith("\\"))
        volumename.chop(1);

    HANDLE h = ::CreateFile(volumename.toStdWString().c_str(), 0, 0, NULL, OPEN_EXISTING, 0, NULL);

    STORAGE_DEVICE_NUMBER info;
    DWORD bytesReturned = 0;

    ::DeviceIoControl(h, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &info, sizeof(info), &bytesReturned, NULL);
    CloseHandle(h);

    return info.DeviceNumber;
}

QString DiskWriter_windows::errorAsString(DWORD error)
{
    if(error == 0)
        return QString();

    LPSTR messageBuffer = NULL;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    const QString message = QString::fromUtf16((const ushort*)messageBuffer, size);
    LocalFree(messageBuffer);

    return message;
}
void DiskWriter_windows::copyToUsb(const QString& device, const QString& jtw){
    // Mounted volume root path
    QString storageRootPath;
    // Where to copy security token
    QString securityTokenLocation= "/fmos_token.txt";
    QFile srcFile;
    srcFile.setFileName("tmp_token.txt");
    if(!srcFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() <<"Could not open for writing";
    }else{
        QTextStream stream(&srcFile);
                    stream << jtw << endl;
    }
    srcFile.close();
    bool found =false;
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (
                storage.isValid() &&
                storage.isReady() &&
                (!(storage.name()).isEmpty()) &&
                storage.name() == "FDP"
            ) {
            if (!storage.isReadOnly()) {
               found = true;
               qDebug() << "path:" << storage.rootPath();
               storageRootPath = storage.rootPath();
               QString destPath = storageRootPath + securityTokenLocation;
               qDebug() << "Copy to USB destPath:" << destPath;
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
