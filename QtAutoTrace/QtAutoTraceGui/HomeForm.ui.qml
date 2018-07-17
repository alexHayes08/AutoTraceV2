import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.0

Page {
    width: 600
    height: 400
    opacity: 0.9

    title: qsTr("Home")

    Column {
        id: column
        spacing: -4
        anchors.fill: parent

        Rectangle {
            id: rectangle
            height: 200
            color: "#00000000"
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            Label {
                id: label
                x: 0
                text: qsTr("Input File")
                anchors.top: parent.top
                anchors.topMargin: 0
            }

            TextField {
                id: textField_inputFile
                text: qsTr("Text Field")
                anchors.top: label.bottom
                anchors.topMargin: 0
            }

            Button {
                id: button_pickInputFile
                text: qsTr("Pick File")
                anchors.left: textField.right
                anchors.leftMargin: 0
                anchors.top: textField.verticalCenter
                anchors.topMargin: -20
                onClicked: inputFileDialog.visible = true
            }

            FileDialog {
                id: inputFileDialog
                title: "Input file..."
                onAccepted: {
                    textField_inputFile.text = inputFileDialog.fileUrl
                }
            }
        }

        Button {
            id: button
            text: qsTr("Generate SVG")
            padding: 6
            wheelEnabled: false
            focusPolicy: Qt.TabFocus
            display: AbstractButton.IconOnly
            Layout.alignment: Qt.AlignRight
        }
    }
}
