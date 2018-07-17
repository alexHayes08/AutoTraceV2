import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Item {
    id: container
    property alias labelText: rectangle.color

    ColumnLayout {
        id: labelInputContainer
        spacing: 2

        Label {
            id: inputLabel
            text: labelText
            wrapMode: Text.WordWrap
        }

        Rectangle {
            id: rectangle
            width: 200
            height: 200
            color: "#00000000"
            Layout.preferredHeight: 18
            Layout.fillHeight: false
            Layout.fillWidth: true

            TextInput {
                id: textInput
                anchors.fill: parent
                Layout.fillWidth: true
            }
        }

    }
}
