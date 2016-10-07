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

#include "AcbfDatabaseref.h"

#include <QXmlStreamWriter>

using namespace AdvancedComicBookFormat;

class DatabaseRef::Private
{
public:
    Private() {}
    QString dbname;
    QString type;
    QString reference;
};

DatabaseRef::DatabaseRef(BookInfo* parent)
    : QObject(parent)
    , d(new Private)
{
}

DatabaseRef::~DatabaseRef()
{
    delete d;
}

void DatabaseRef::toXml(QXmlStreamWriter* writer)
{
    writer->writeStartElement("databaseref");
    writer->writeAttribute("dbname", d->dbname);
    if(!d->type.isEmpty()) {
        writer->writeAttribute("type", d->type);
    }
    writer->writeCharacters(d->reference);
    writer->writeEndElement();
}

bool DatabaseRef::fromXml(QXmlStreamReader *xmlReader)
{
    setDbname(xmlReader->attributes().value("volume").toString());
    setType(xmlReader->attributes().value("type").toString());
    setReference(xmlReader->readElementText(QXmlStreamReader::IncludeChildElements));
    return true;
}

QString DatabaseRef::dbname()
{
    return d->dbname;
}

void DatabaseRef::setDbname(QString dbname)
{
    d->dbname = dbname;
}

QString DatabaseRef::type()
{
    return d->type;
}

void DatabaseRef::setType(QString type)
{
    d->type = type;
}

QString DatabaseRef::reference()
{
    return d->reference;
}

void DatabaseRef::setReference(QString reference)
{
    d->reference = reference;
}