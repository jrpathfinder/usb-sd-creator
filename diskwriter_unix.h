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
};

#endif // DISKWRITER_UNIX_H
