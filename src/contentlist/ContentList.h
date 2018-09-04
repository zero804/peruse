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

#ifndef CONTENTLISTBASE_H
#define CONTENTLISTBASE_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QQmlParserStatus>

#include "ContentQuery.h"

/**
 * \brief Class to handle searching all the book entries.
 * 
 * This class handles the queries and the contentlisterbase
 * that in turn handles the actual searching.
 * 
 * When searching is done, the entries are filled into this model.
 */
class ContentList : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "queries")
    /**
     * \brief The list of query parameters that make up this search.
     */
    Q_PROPERTY(QQmlListProperty<ContentQuery> queries READ queries)
    /**
     * TODO: No idea.
     */
    Q_PROPERTY(bool autoSearch READ autoSearch WRITE setAutoSearch NOTIFY autoSearchChanged)
    /**
     * \brief Whether to cache the search results for later.
     */
    Q_PROPERTY(bool cacheResults READ cacheResults WRITE setCacheResults NOTIFY cacheResultsChanged)
public:
    explicit ContentList(QObject* parent = nullptr);
    ~ContentList() override;

    /**
     * Extra roles for the different kinds of data that can be searched.
     */
    enum Roles {
        FilenameRole = Qt::UserRole + 1,
        FilePathRole,
        MetadataRole
    };

    /**
     * @returns the list of search parameters as a list of content queries.
     */
    QQmlListProperty<ContentQuery> queries();

    /**
     * @return whether to do auto search.
     */
    bool autoSearch() const;
    
    /**
     * @return whether to cache the results.
     */
    bool cacheResults() const;

    /**
     * \brief QStrings with names for the extra roles.
     */
    QHash<int, QByteArray> roleNames() const override;
    /**
     * \brief Access the search results inside the model.
     * @param index The QModelIndex at which you wish to access the data.
     * @param role An enumerator of the type of data you want to access.
     * Is extended by the Roles enum.
     * 
     * @return a QVariant with the search entry data.
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    /**
     * @param parent The QModel index of the parent. This only counts for
     * tree like page structures, and thus defaults to a freshly constructed
     * QModelIndex. A wellformed QModelIndex will cause this function to return 0
     * @returns the number of total rows(search results) there are in this model.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * Inherited from QmlParserStatus, not implemented.
     */
    void classBegin() override;
    /**
     * \brief loads the cached results or autosearch
     * into the model if either is enabled.
     * 
     * Inherited from QmlParserStatus.
     */
    void componentComplete() override;

    /**
     * \brief Set whether to do autoSearch.
     * @param autoSearch whether to do autoSearch.
     */
    Q_SLOT void setAutoSearch(bool autoSearch);
    /**
     * \brief Set whether to cache the results.
     * @param cacheResults whether to cache the results.
     */
    Q_SLOT void setCacheResults(bool cacheResults);

    /**
     * \brief Fill the model with the results.
     * 
     * This clears the model of search entries and then
     * fills it up with the new entries.
     * 
     * @param results a stringlist with paths to the new
     * search results.
     */
    Q_SLOT void setKnownFiles(const QStringList& results);
    /**
     * \brief Start searching with the current queries list.
     */
    Q_SLOT void startSearch();

    Q_SIGNAL void autoSearchChanged();
    Q_SIGNAL void cacheResultsChanged();
    /**
     * \brief Fires when the search is completed.
     */
    Q_SIGNAL void searchCompleted();

    Q_INVOKABLE static QString getMimetype(QString filePath);

private:
    bool isComplete() const;
    Q_SLOT void fileFound(const QString& filePath, const QVariantMap& metaData);

    class Private;
    Private* d;
};

#endif//CONTENTLISTBASE_H
