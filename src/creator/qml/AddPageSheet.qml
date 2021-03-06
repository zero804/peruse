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
import QtQuick.Controls 2.12 as QtControls
import QtQuick.Dialogs 1.3

import org.kde.kirigami 2.7 as Kirigami
/**
 * @brief overlay with options for adding a page.
 * 
 * It is accessed from Book
 */
Kirigami.OverlaySheet {
    id: root;
    property int addPageAfter: 0;
    property QtObject model;
    Column {
        height: childrenRect.height;
        spacing: Kirigami.Units.smallSpacing;
        Kirigami.Heading {
            width: parent.width;
            height: paintedHeight;
            text: i18nc("title text for the add page sheet", "Add a Page?");
        }
        QtControls.Label {
            width: parent.width;
            height: paintedHeight;
            text: i18nc("help text for the add page sheet", "Please select the method you want to add the new page. No changes will be made outside of the project by performing these actions.");
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
        }
        Item {
            width: parent.width;
            height: Kirigami.Units.largeSpacing;
        }
        QtControls.Button {
            anchors.horizontalCenter: parent.horizontalCenter;
//             iconName: "document-open";
            text: i18nc("@action:button add a page by finding an image on the filesystem and copying it into the book", "Copy Image from Device");
            onClicked: openDlg.open();
            FileDialog {
                id: openDlg;
                title: i18nc("@title:window standard file open dialog used to find a page to add to the book", "Please Choose an Image to Add");
                folder: mainWindow.homeDir();
                property int splitPos: osIsWindows ? 8 : 7;
                onAccepted: {
                    if(openDlg.fileUrl.toString().substring(0, 7) === "file://") {
                        root.model.addPageFromFile(openDlg.fileUrl.toString().substring(splitPos));
                        root.close();
                    }
                }
                onRejected: {
                    // Just do nothing, we don't really care...
                }
            }
        }
        QtControls.Button {
            anchors.horizontalCenter: parent.horizontalCenter;
//             iconName: "document-new";
            text: i18nc("@action:button add a page by creating a new image using an image editor", "Create a New Image Using an Image Editor");
        }
        QtControls.Button {
            anchors.horizontalCenter: parent.horizontalCenter;
//             iconName: "camera";
            text: i18nc("@action:button add a page by taking a photo with a camera", "Take a Photo and Add That");
        }
    }
}
