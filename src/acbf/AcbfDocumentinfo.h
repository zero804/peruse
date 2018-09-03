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

#ifndef ACBFDOCUMENTINFO_H
#define ACBFDOCUMENTINFO_H

#include <memory>

#include "AcbfMetadata.h"

#include <QDate>
/**
 * \brief Class to handle the DocumentInfo section.
 * 
 * DocumentInfo, in ACBF is the data about the ACBF file itself.
 * It contains the ACBF author, creation date, original source, version
 * number and a version history.
 * 
 * Sources is a stringlist, which is useful when the ACBF encompasses several
 * pages of a webcomic, for example.
 */
namespace AdvancedComicBookFormat
{
class Author;
class ACBF_EXPORT DocumentInfo : public QObject
{
    Q_OBJECT
public:
    explicit DocumentInfo(Metadata* parent = nullptr);
    ~DocumentInfo() override;

    Metadata* metadata() const;

    /**
     * \brief write the documentinfo into the XML writer.
     */
    void toXml(QXmlStreamWriter *writer);
    /**
     * \brief load the DocumentInfo into this object.
     * @return True if the xmlReader encountered no errors.
     */
    bool fromXml(QXmlStreamReader *xmlReader);

    /**
     * \brief the list of authors that worked on this specific acbf.
     * @return a list of author objects.
     */
    QList<Author*> author() const;
    /**
     * \brief add an Author object to the list of authors.
     * @param author - an author object.
     */
    void addAuthor(Author* author);
    /**
     * \brief remove an Author object from the list of authors.
     * @param author - an author object.
     */
    void removeAuthor(Author* author);

    /**
     * @return a QDate with the creation date of this file.
     */
    QDate creationDate() const;
    /**
     * \brief set the creation date of this file.
     * @param creationDate - a QDate object holding the creation date.
     */
    void setCreationDate(const QDate& creationDate);

    /**
     * @return a list of sources that this ACBF is an adaptation of.
     */ 
    QStringList source() const;
    /**
     * \brief set the list of source that this ACBF is an adaptation of.
     * @param source - a stringlist of sources.
     */
    void setSource(const QStringList& source);

    /**
     * @returns the unique id of this ACBF document, used for cataloguing purposes.
     */
    QString id() const;
    /**
     * \brief set the unique ID for this file used for cataloguing purposes.
     * @param id - a unique ID. Can be any kind of string, a UUID is recommended as default.
     */
    void setId(const QString& id);

    /**
     * @return the version of this document.
     */
    QString version() const;
    /**
     * \brief set the version of this document
     * @param version - the version as a string.
     * TODO: The official XSD requires a floating point number here.
     */
    void setVersion(const QString& version);

    /**
     * @return a list of history entries as a stringlist.
     */
    QStringList history() const;
    /**
     * \brief set the whole history list as a single stringlist.
     * @param history - the whole history of the document as a stringlist.
     */
    void setHistory(const QStringList& history);
    /**
     * \brief add a single entry to the document history.
     * @param historyLine - a single entry in the document.
     */
    void addHistoryLine(const QString& historyLine);
private:
    class Private;
    Private* d;
};
}

#endif//ACBFDOCUMENTINFO_H
