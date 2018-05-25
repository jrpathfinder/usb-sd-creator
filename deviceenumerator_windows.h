#ifndef DEVICEENUMERATOR_WINDOWS_H
#define DEVICEENUMERATOR_WINDOWS_H

#include "deviceenumerator.h"

#include <QStringList>
#include <QDebug>

class DeviceEnumerator_windows : public DeviceEnumerator
{
public:
    QStringList getRemovableDeviceNames() const;
    QStringList getUserFriendlyNames(const QStringList& devices) const;
    bool unmountDevicePartitions(const QString &device) const {
        Q_UNUSED(device);
        return true;
    }
    qint64 getSizeOfDevice(const QString &device) const;
    int loadEjectDrive(const QString &device, const loadEject action) const;
    int removeDrive(const QString &device) const;

private:
    static QString getLabelOfDevice(const QString &device);

    // returned errorlevels
    enum {
      ERRL_SUCCESS = 0,
      ERRL_INVALID_PARAM,
      ERRL_NO_VOLREAD,
      ERRL_NO_LOCK,
      ERRL_NO_EJECT
    };
};

#endif // DEVICEENUMERATOR_WINDOWS_H
