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

#ifndef ACBFPUBLISHINFO_H
#define ACBFPUBLISHINFO_H

#include <memory>

#include "AcbfMetadata.h"

#include <QDate>
/**
 * \brief Class to handle the publishing info.
 * 
 * The Publishing info in ACBF is about who published the book,
 * when, where, the ISBN number, and which license it was published
 * under.
 */
namespace AdvancedComicBookFormat
{
class ACBF_EXPORT PublishInfo : public QObject
{
    Q_OBJECT
public:
    explicit PublishInfo(Metadata* parent = nullptr);
    ~PublishInfo() override;

    /**
     * \brief Write the publishingInfo into the xml writer.
     */
    void toXml(QXmlStreamWriter *writer);
    /**
     * \brief load the publishing info into this object.
     * @return True if the xmlReader encountered no errors.
     */
    bool fromXml(QXmlStreamReader *xmlReader);

    /**
     * @return the name of the publisher.
     */
    QString publisher() const;
    /**
     * \brief set the name of the publisher.
     */
    void setPublisher(const QString& publisher);

    /**
     * @return the publishing date as a QDate object.
     */
    QDate publishDate() const;
    /**
     * \brief set the publishing date.
     * @param publishDate - the publishingdate as a QDate object.
     */
    void setPublishDate(const QDate& publishDate);

    /**
     * @return the name of the city the work was published in.
     */
    QString city() const;
    /**
     * \brief set the name of the city the work was published in.
     * @param city - the name of the city as a QString.
     */
    void setCity(const QString& city = QString());

    /**
     * @return the ISBN number as a QString.
     */
    QString isbn() const;
    /**
     * \brief set the ISBN number.
     * @param isbn - the ISBN number as a QString. ISBN numbers should be registered.
     */
    void setIsbn(const QString& isbn = QString());

    /**
     * @return the name of the license the comic was published under.
     */
    QString license() const;
    /**
     * \brief set the License that the work was published under.
     * @param license - the name of the license as a QString.
     */
    void setLicense(const QString& license = QString());
private:
    class Private;
    std::unique_ptr<Private> d;
};
}

#endif//ACBFPUBLISHINFO_H
