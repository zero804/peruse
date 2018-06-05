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

#include "AcbfTextarea.h"

#include <QDebug>
#include <QXmlStreamReader>

using namespace AdvancedComicBookFormat;

class Textarea::Private
{
public:
    Private()
        : textRotation(0)
    {}
    QString bgcolor;
    QList<QPoint> points;
    int textRotation;
    QString type;
    bool inverted;
    bool transparent;
    QStringList paragraphs;
};

Textarea::Textarea(Textlayer* parent)
    : QObject(parent)
    , d(new Private)
{
}

Textarea::~Textarea() = default;

void Textarea::toXml(QXmlStreamWriter* writer)
{
    writer->writeStartElement(QStringLiteral("text-area"));

    QStringList points;
    Q_FOREACH(const QPoint& point, d->points) {
        points << QStringLiteral("%1,%2").arg(QString::number(point.x())).arg(QString::number(point.y()));
    }
    writer->writeAttribute(QStringLiteral("points"), points.join(' '));

    if(!d->bgcolor.isEmpty()) {
        writer->writeAttribute(QStringLiteral("bgcolor"), d->bgcolor);
    }
    if(d->textRotation != 0) {
        writer->writeAttribute(QStringLiteral("text-rotation"), QString::number(d->textRotation));
    }
    if(!d->type.isEmpty()) {
        writer->writeAttribute(QStringLiteral("type"), d->type);
    }
    if(d->inverted) {
        // because the default is false, no need to write it otherwise...
        writer->writeAttribute(QStringLiteral("inverted"), QStringLiteral("true"));
    }
    if(d->transparent) {
        // because the default is false, no need to write it otherwise...
        writer->writeAttribute(QStringLiteral("transparent"), QStringLiteral("true"));
    }

    Q_FOREACH(const QString& paragraph, d->paragraphs) {
        writer->writeStartElement(QStringLiteral("p"));
        writer->writeCharacters(paragraph);
        writer->writeEndElement();
    }

    writer->writeEndElement();
}

bool Textarea::fromXml(QXmlStreamReader *xmlReader)
{
    setBgcolor(xmlReader->attributes().value(QStringLiteral("bgcolor")).toString());
    setTextRotation(xmlReader->attributes().value(QStringLiteral("text-rotation")).toInt());
    setType(xmlReader->attributes().value(QStringLiteral("type")).toString());
    setInverted(xmlReader->attributes().value(QStringLiteral("inverted")).toString().toLower() == QStringLiteral("true"));
    setTransparent(xmlReader->attributes().value(QStringLiteral("transparent")).toString().toLower() == QStringLiteral("true"));

    QStringList points = xmlReader->attributes().value(QStringLiteral("points")).toString().split(' ');
    Q_FOREACH(const QString& point, points) {
        QStringList elements = point.split(',');
        if(elements.length() == 2)
        {
            addPoint(QPoint(elements.at(0).toInt(), elements.at(1).toInt()));
        }
        else
        {
            qWarning() << "Failed to construct one of the points for a text-area. Attempted to handle the point" << point << "in the data" << points;
            return false;
        }
    }

    while(xmlReader->readNextStartElement())
    {
        if(xmlReader->name() == QStringLiteral("p"))
        {
            d->paragraphs.append(xmlReader->readElementText(QXmlStreamReader::IncludeChildElements));
        }
        else
        {
            qWarning() << Q_FUNC_INFO << "currently unsupported subsection in text-area:" << xmlReader->name();
            xmlReader->skipCurrentElement();
        }
    }
    if (xmlReader->hasError()) {
        qWarning() << Q_FUNC_INFO << "Failed to read ACBF XML document at token" << xmlReader->name() << "(" << xmlReader->lineNumber() << ":" << xmlReader->columnNumber() << ") The reported error was:" << xmlReader->errorString();
    }
    qDebug() << Q_FUNC_INFO << "Created a text area of type" << type();
    return !xmlReader->hasError();
}

QList<QPoint> Textarea::points() const
{
    return d->points;
}

QPoint Textarea::point(int index) const
{
    return d->points.at(index);
}

int Textarea::pointIndex(const QPoint& point) const
{
    return d->points.indexOf(point);
}

void Textarea::addPoint(const QPoint& point, int index)
{
    if(index > -1 && d->points.count() < index) {
        d->points.insert(index, point);
    }
    else {
        d->points.append(point);
    }
}

void Textarea::removePoint(const QPoint& point)
{
    d->points.removeAll(point);
}

bool Textarea::swapPoints(const QPoint& swapThis, const QPoint& withThis)
{
    int index1 = d->points.indexOf(swapThis);
    int index2 = d->points.indexOf(withThis);
    if(index1 > -1 && index2 > -1) {
        d->points.swap(index1, index2);
        return true;
    }
    return false;
}

QString Textarea::bgcolor() const
{
    return d->bgcolor;
}

void Textarea::setBgcolor(const QString& newColor)
{
    d->bgcolor = newColor;
}

void Textarea::setTextRotation(int rotation)
{
    d->textRotation = rotation;
}

int Textarea::textRotation() const
{
    return d->textRotation;
}

QString Textarea::type() const
{
    return d->type.isEmpty() ? "speech" : d->type;
}

void Textarea::setType(const QString& type)
{
    d->type = type;
}

QStringList Textarea::availableTypes()
{
    return {
        QStringLiteral("speech"), // (character is speaking, text is centered)
        QStringLiteral("commentary"), // (accompanying commentary, text is aligned to left)
        QStringLiteral("formal"), // (text alignment is - justify)
        QStringLiteral("letter"), // (rendered in handwriting font)
        QStringLiteral("code"), // (rendered in monospace font)
        QStringLiteral("heading"), // (e.g. chapter title)
        QStringLiteral("audio"), // (speech emanating from an audio device, e.g., television or radio speaker, telephone, walkie-talkie, etc.)
        QStringLiteral("thought"),
        QStringLiteral("sign"), // (any kind of sign/writing, text is centered)
    };
}

bool Textarea::inverted() const
{
    return d->inverted;
}

void Textarea::setInverted(bool inverted)
{
    d->inverted = inverted;
}

bool Textarea::transparent() const
{
    return d->transparent;
}

void Textarea::setTransparent(bool transparent)
{
    d->transparent = transparent;
}

QStringList Textarea::paragraphs() const
{
    return d->paragraphs;
}

void Textarea::setParagraphs(const QStringList& paragraphs)
{
    d->paragraphs = paragraphs;
}
