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
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3

PeruseMain {
    id: root;
    width: Screen.desktopAvailableWidth * 0.6;
    height: Screen.desktopAvailableHeight * 0.7;

    function openOther() {
        openDlg.open();
    }

    FileDialog {
        id: openDlg;
        title: i18nc("@title:window standard file open dialog used to open a book not in the collection", "Please Choose a Comic to Open");
        folder: root.homeDir();
        property int splitPos: osIsWindows ? 8 : 7;
        onAccepted: {
            if(openDlg.fileUrl.toString().substring(0, 7) === "file://") {
                root.showBook(openDlg.fileUrl.toString().substring(splitPos), 0);
            }
        }
        onRejected: {
            // Just do nothing, we don't really care...
        }
    }
}
