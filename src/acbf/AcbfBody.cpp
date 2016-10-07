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

#include "AcbfBody.h"
#include "AcbfPage.h"

#include <QDebug>
#include <QXmlStreamReader>

using namespace AdvancedComicBookFormat;

class Body::Private
{
public:
    Private() {}
    QString bgcolor;
    QList<Page*> pages;
};

Body::Body(Document* parent)
    : QObject(parent)
    , d(new Private)
{
}

Body::~Body()
{
    delete d;
}

Document * Body::document()
{
    return qobject_cast<Document*>(parent());
}

void Body::toXml(QXmlStreamWriter *writer)
{
    writer->writeStartElement("body");

    Q_FOREACH(Page* page, d->pages) {
        page->toXml(writer);
    }

    writer->writeEndElement();
}

bool Body::fromXml(QXmlStreamReader *xmlReader)
{
    setBgcolor(xmlReader->attributes().value("bgcolor").toString());
    while(xmlReader->readNextStartElement())
    {
        if(xmlReader->name() == "page")
        {
            Page* newPage = new Page(document());
            if(!newPage->fromXml(xmlReader)) {
                return false;
            }
            d->pages.append(newPage);
        }
        else
        {
            qWarning() << Q_FUNC_INFO << "currently unsupported subsection:" << xmlReader->name();
            xmlReader->skipCurrentElement();
        }
        if(xmlReader->readNext() == QXmlStreamReader::EndElement && xmlReader->name() == "body") {
            break;
        }
    }
    if (xmlReader->hasError()) {
        qWarning() << Q_FUNC_INFO << "Failed to read ACBF XML document at token" << xmlReader->name() << "(" << xmlReader->lineNumber() << ":" << xmlReader->columnNumber() << ") The reported error was:" << xmlReader->errorString();
    }
    return !xmlReader->hasError();
}

QString Body::bgcolor()
{
    return d->bgcolor;
}

void Body::setBgcolor(QString newColor)
{
    d->bgcolor = newColor;
}

QList<Page *> Body::pages()
{
    return d->pages;
}

Page * Body::page(int index)
{
    return d->pages.at(index);
}

int Body::pageIndex(Page* page)
{
    return d->pages.indexOf(page);
}

void Body::addPage(Page* page, int index)
{
    if(index > -1 && d->pages.count() < index) {
        d->pages.insert(index, page);
    }
    else {
        d->pages.append(page);
    }
}

void Body::removePage(Page* page)
{
    d->pages.removeAll(page);
}

bool Body::swapPages(Page* swapThis, Page* withThis)
{
    int index1 = d->pages.indexOf(swapThis);
    int index2 = d->pages.indexOf(withThis);
    if(index1 > -1 && index2 > -1) {
        d->pages.swap(index1, index2);
        return true;
    }
    return false;
}