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

#include "AcbfPage.h"
#include "AcbfTextlayer.h"

#include <QDebug>
#include <QHash>
#include <QXmlStreamReader>

using namespace AdvancedComicBookFormat;

class Page::Private
{
public:
    Private()
        : isCoverPage(false)
    {}
    QString bgcolor;
    QString transition;
    QHash<QString, QString> title;
    QString imageHref;
    QHash<QString, Textlayer*> textLayers;
    QList<Frame*> frames;
    QList<Jump*> jumps;
    bool isCoverPage;
};

Page::Page(Document* parent)
    : QObject(parent)
    , d(new Private)
{
}

Page::~Page()
{
    delete d;
}

void Page::toXml(QXmlStreamWriter* writer)
{
    if(d->isCoverPage) {
        writer->writeStartElement("coverpage");
    }
    else {
        writer->writeStartElement("page");
    }

    if(!d->bgcolor.isEmpty()) {
        writer->writeAttribute("bgcolor", d->bgcolor);
    }
    if(!d->transition.isEmpty()) {
        writer->writeAttribute("transition", d->transition);
    }

    QHashIterator<QString, QString> titles(d->title);
    while(titles.hasNext()) {
        titles.next();
        writer->writeStartElement("title");
        writer->writeAttribute("lang", titles.key());
        writer->writeCharacters(titles.value());
        writer->writeEndElement();
    }

    writer->writeStartElement("image");
    writer->writeAttribute("href", d->imageHref);
    writer->writeEndElement();

    Q_FOREACH(Textlayer* layer, d->textLayers.values()) {
        layer->toXml(writer);
    }

//     Q_FOREACH(Frame* frame, d->frames) {
//         frame->toXml(writer);
//     }

//     Q_FOREACH(Jump* jump, d->jumps) {
//         jump->toXml(writer);
//     }

    writer->writeEndElement();
}

bool Page::fromXml(QXmlStreamReader *xmlReader)
{
    setBgcolor(xmlReader->attributes().value("bgcolor").toString());
    setTransition(xmlReader->attributes().value("transition").toString());
    while(xmlReader->readNextStartElement())
    {
        if(xmlReader->name() == "title")
        {
            d->title[xmlReader->attributes().value("lang").toString()] = xmlReader->readElementText();
        }
        else if(xmlReader->name() == "image")
        {
            setImageHref(xmlReader->attributes().value("href").toString());
        }
        else if(xmlReader->name() == "text-layer")
        {
            Textlayer* newLayer = new Textlayer(this);
            if(!newLayer->fromXml(xmlReader)) {
                return false;
            }
            d->textLayers[newLayer->language()] = newLayer;
        }
//             else if(xmlReader->name() == "frame")
//             {
//                 Frame* newFrame = new Frame(this);
//                 if(!newFrame->fromXml()) {
//                     return false;
//                 }
//                 d->frames.append(newFrame);
//             }
//             else if(xmlReader->name() == "jump")
//             {
//                 Jump* newJump = new Jump(this);
//                 if(!newJump->fromXml()) {
//                     return false;
//                 }
//                 d->jumps.append(newJump);
//             }
        else
        {
            qWarning() << Q_FUNC_INFO << "currently unsupported subsection:" << xmlReader->name();
            xmlReader->skipCurrentElement();
        }
        if(xmlReader->readNext() == QXmlStreamReader::EndElement && xmlReader->name() == "page") {
            break;
        }
    }
    if (xmlReader->hasError()) {
        qWarning() << Q_FUNC_INFO << "Failed to read ACBF XML document at token" << xmlReader->name() << "(" << xmlReader->lineNumber() << ":" << xmlReader->columnNumber() << ") The reported error was:" << xmlReader->errorString();
    }
    return !xmlReader->hasError();
}

QString Page::bgcolor()
{
    return d->bgcolor;
}

void Page::setBgcolor(QString newColor)
{
    d->bgcolor = newColor;
}

QString Page::transition()
{
    return d->transition;
}

void Page::setTransition(QString transition)
{
    d->transition = transition;
}

QStringList Page::availableTransitions()
{
    QStringList transitions;
    transitions << "fade"; // (old page fades out into background, then new page fades in)
    transitions << "blend"; // (new page blends in the image while old page blends out)
    transitions << "scroll_right"; // (screen scrolls to the right to a new page; reversed behavior applies when moving to previous page)
    transitions << "scroll_down"; // (screen scrolls down to a new page; reversed behavior applies when moving to previous page)
    transitions << "none"; // (no transition animation happens)
    return transitions;
}

QStringList Page::titleForAllLanguages()
{
    return d->title.values();
}

QString Page::title(QString language)
{
    return d->title.value(language);
}

void Page::setTitle(QString title, QString language)
{
    if(title.isEmpty())
    {
        d->title.remove(language);
    }
    else
    {
        d->title[language] = title;
    }
}

QString Page::imageHref()
{
    return d->imageHref;
}

void Page::setImageHref(QString imageHref)
{
    d->imageHref = imageHref;
}

QList<Textlayer *> Page::textLayersForAllLanguages()
{
    return d->textLayers.values();
}

Textlayer * Page::textLayer(QString language)
{
    return d->textLayers.value(language);
}

void Page::setTextLayer(Textlayer* textlayer, QString language)
{
    if(textlayer)
    {
        d->textLayers[language] = textlayer;
    }
    else
    {
        d->textLayers.remove(language);
    }
}

QList<Frame *> Page::frames()
{
    return d->frames;
}

Frame * Page::frame(int index)
{
    return d->frames.at(index);
}

int Page::frameIndex(Frame* frame)
{
    return d->frames.indexOf(frame);
}

void Page::addFrame(Frame* frame, int index)
{
    if(index > -1 && d->frames.count() < index) {
        d->frames.insert(index, frame);
    }
    else {
        d->frames.append(frame);
    }
}

void Page::removeFrame(Frame* frame)
{
    d->frames.removeAll(frame);
}

bool Page::swapFrames(Frame* swapThis, Frame* withThis)
{
    int index1 = d->frames.indexOf(swapThis);
    int index2 = d->frames.indexOf(withThis);
    if(index1 > -1 && index2 > -1) {
        d->frames.swap(index1, index2);
        return true;
    }
    return false;
}

QList<Jump *> Page::jumps()
{
    return d->jumps;
}

Jump * Page::jump(int index)
{
    return d->jumps.at(index);
}

int Page::jumpIndex(Jump* jump)
{
    return d->jumps.indexOf(jump);
}

void Page::addJump(Jump* jump, int index)
{
    if(index > -1 && d->jumps.count() < index) {
        d->jumps.insert(index, jump);
    }
    else {
        d->jumps.append(jump);
    }
}

void Page::removeJump(Jump* jump)
{
    d->jumps.removeAll(jump);
}

bool Page::swapJumps(Jump* swapThis, Jump* withThis)
{
    int index1 = d->jumps.indexOf(swapThis);
    int index2 = d->jumps.indexOf(withThis);
    if(index1 > -1 && index2 > -1) {
        d->jumps.swap(index1, index2);
        return true;
    }
    return false;
}

bool Page::isCoverPage()
{
    return d->isCoverPage;
}

void Page::setIsCoverPage(bool isCoverPage)
{
    d->isCoverPage = isCoverPage;
}