import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Item {
    id: container

    property alias label_input: label_input
    property alias textInput_input: textInput

    ColumnLayout {
        id: labelInputContainer
        spacing: 2

        Label {
            id: label_input
            text: labelText
            wrapMode: Text.WordWrap
        }

        TextInput {
            id: textInput
            anchors.fill: parent
            Layout.fillWidth: true
        }

        Label {
            id: label_warning
            text: ""
        }
    }
}
