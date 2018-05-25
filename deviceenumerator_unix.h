#ifndef DEVICEENUMERATOR_UNIX_H
#define DEVICEENUMERATOR_UNIX_H

#include "deviceenumerator.h"
#include <QStringList>

class DeviceEnumerator_unix : public DeviceEnumerator
{
public:
    QStringList getRemovableDeviceNames() const;
    QStringList getUserFriendlyNames(const QStringList& devices) const;
    bool unmountDevicePartitions(const QString &device) const;
    qint64 getSizeOfDevice(const QString &device) const;
    int loadEjectDrive(const QString &device, const loadEject action) const
    {
        Q_UNUSED(device);
        Q_UNUSED(action);
        return 0;
    }
    int removeDrive(const QString &device) const
    {
        Q_UNUSED(device);
        return 0;
    }

private:
    bool checkIsMounted(const QString& device) const;
    bool checkIfUSB(const QString& device) const;
    QStringList getDeviceNamesFromSysfs() const;
#if defined(Q_OS_LINUX)
    QStringList getPartitionsInfo(const QString &device) const;
    QString getFirstPartitionLabel(const QString& device) const;
    bool unmount(const QString& what) const;
    bool mount(const QString& what, const QString & where ) const;
#endif
};

#endif // DEVICEENUMERATOR_UNIX_H
