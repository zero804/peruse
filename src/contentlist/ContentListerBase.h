/*
 * Copyright (C) 2015 Dan Leinir Turthra Jensen <admin@leinir.dk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CONTENTLISTERBASE_H
#define CONTENTLISTERBASE_H

#include <QObject>

class ContentListerBase : public QObject
{
    Q_OBJECT
public:
    explicit ContentListerBase(QObject* parent = nullptr);
    ~ContentListerBase() override;

    virtual void addLocation(QString path);
    virtual void addMimetype(QString mimetype);
    virtual void setSearchString(const QString& searchString);
    virtual void setKnownFiles(QStringList knownFiles);
    Q_SLOT virtual void startSearch();

    Q_SIGNAL void fileFound(const QString& filePath, const QVariantHash& metadata);
    Q_SIGNAL void searchCompleted();
};

#endif//CONTENTLISTERBASE_H
