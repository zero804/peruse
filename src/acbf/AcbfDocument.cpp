/*
 * Copyright (C) 2016 Dan Leinir Turthra Jensen <admin@leinir.dk>
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

#include "AcbfDocument.h"
#include "AcbfBody.h"
#include "AcbfMetadata.h"
#include "AcbfBookinfo.h"
#include "AcbfData.h"
#include "AcbfReferences.h"
#include "AcbfStyleSheet.h"

#include <QDebug>
#include <QXmlStreamReader>

using namespace AdvancedComicBookFormat;

class Document::Private {
public:
    Private()
        : metaData(nullptr)
        , body(nullptr)
        , data(nullptr)
    {}
    Metadata* metaData;
    Body* body;
    Data* data;
    References* references;
    StyleSheet* cssStyleSheet;
    
};

Document::Document(QObject* parent)
    : QObject(parent)
    , d(new Private)
{
    d->metaData = new Metadata(this);
    d->body = new Body(this);
    d->data = new Data(this);
    d->references = new References(this);
    d->cssStyleSheet = new StyleSheet(this);
}

Document::~Document() = default;

QString Document::toXml()
{
    QString output;
    QXmlStreamWriter writer(&output);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(QStringLiteral("ACBF"));
    writer.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://www.fictionbook-lib.org/xml/acbf/1.0"));
    d->metaData->toXml(&writer);
    d->body->toXml(&writer);
    writer.writeEndElement();
    writer.writeEndDocument();

    return output;
}

bool Document::fromXml(QString xmlDocument)
{
    QXmlStreamReader xmlReader(xmlDocument);
    if(xmlReader.readNextStartElement())
    {
        if(xmlReader.name() == QStringLiteral("ACBF")
            && (xmlReader.namespaceUri().startsWith(QStringLiteral("http://www.fictionbook-lib.org/xml/acbf/"))
                || xmlReader.namespaceUri().startsWith(QStringLiteral("http://www.acbf.info/xml/acbf/"))
            ))
        {
            qDebug()<< xmlReader.documentEncoding().toString();
            while(xmlReader.readNextStartElement())
            {
                if(xmlReader.name() == QStringLiteral("meta-data"))
                {
                    if(!d->metaData->fromXml(&xmlReader)) {
                        break;
                    }
                }
                else if(xmlReader.name() == QStringLiteral("body"))
                {
                    if(!d->body->fromXml(&xmlReader)) {
                        break;
                    }
                }
                else if(xmlReader.name() == QStringLiteral("data"))
                {
                    if(!d->data->fromXml(&xmlReader)) {
                        break;
                    }
                }
                else if(xmlReader.name() == QStringLiteral("references"))
                {
                    if(!d->references->fromXml(&xmlReader)) {
                        break;
                    }
                }
                else if(xmlReader.name() == QStringLiteral("style"))
                {
                    if(!d->cssStyleSheet->fromXml(&xmlReader)) {
                        break;
                    }
                }
                else
                {
                    qWarning() << Q_FUNC_INFO << "currently unsupported subsection:" << xmlReader.name();
                    xmlReader.skipCurrentElement();
                }
            }
        }
        else {
            qWarning() << Q_FUNC_INFO << "not an ACBF XML document";
            return false;
        }
    }
    if (xmlReader.hasError()) {
        qWarning() << Q_FUNC_INFO << "Failed to read ACBF XML document at token" << xmlReader.name() << "(" << xmlReader.lineNumber() << ":" << xmlReader.columnNumber() << ") The reported error was:" << xmlReader.errorString();
    }
    qDebug() << Q_FUNC_INFO << "Completed ACBF document creation for" << d->metaData->bookInfo()->title();
    return !xmlReader.hasError();
}

Metadata * Document::metaData() const
{
    return d->metaData;
}

Body * Document::body() const
{
    return d->body;
}

Data * Document::data() const
{
    return d->data;
}

References * Document::references() const
{
    return d->references;
}

StyleSheet * Document::styleSheet() const
{
    return d->cssStyleSheet;
}
