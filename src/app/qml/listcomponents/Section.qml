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

import QtQuick 2.12

import org.kde.kirigami 2.7 as Kirigami
/**
 * @brief Draws a big header to indicate subsections within a page.
 */
Row {
    width: parent.width;
    height: headerText.paintedHeight;
    property alias text: headerText.text;
    property alias paintedHeight: headerText.paintedHeight;
    Item {
        width: Kirigami.Units.largeSpacing;
        height: 1;
    }
    Kirigami.Heading {
        id: headerText;
        width: paintedWidth;
        text: section;
    }
    Item {
        width: Kirigami.Units.smallSpacing;
        height: 1;
    }
    Rectangle {
        anchors.bottom: headerText.baseline;
        height: 2;
        width: parent.width - headerText.width - Kirigami.Units.smallSpacing - Kirigami.Units.largeSpacing * 2;
        radius: 2;
        color: headerText.color;
    }
}
