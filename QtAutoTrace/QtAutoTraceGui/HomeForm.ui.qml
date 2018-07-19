import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Page {
    anchors.fill: parent

    property alias button_generateSvg: button_generateSvg

    property alias textField_inputFile: textField_inputFile
    property alias button_inputFile: button_inputFile
    property alias label_inputFileWarning: label_inputFileWarning

    property alias textField_outputFile: textField_outputFile
    property alias button_outputFile: button_outputFile
    property alias label_outputFileWarning: label_outputFileWarning

    property alias textField_colorCount: textField_colorCount
    property alias label_colorCountWarning_text: label_colorCountWarning

    opacity: 1

    title: qsTr("Home")

    Column {
        id: column
        spacing: 0
        anchors.fill: parent

        GridLayout {
            id: gridLayout_inputFile
            height: 100
            columnSpacing: 0
            rowSpacing: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            rows: 3
            columns: 2

            Label {
                id: label_inputFile
                text: qsTr("Input File")
                font.wordSpacing: 0
                Layout.columnSpan: 2
                font.pointSize: 15
                rightPadding: 6
                leftPadding: 6
                bottomPadding: 6
                topPadding: 6
            }

            TextField {
                id: textField_inputFile
                text: qsTr("")
                placeholderText: "/.../some/file/inputfile.jpeg"
                Layout.fillWidth: true
                font.pointSize: 11
            }

            Button {
                id: button_inputFile
                text: qsTr("Pick File")
                font.pointSize: 12
            }

            Label {
                id: label_inputFileWarning
                text: qsTr("")
                font.pointSize: 11
                wrapMode: Text.WordWrap
                Layout.columnSpan: 2
                Layout.fillWidth: true
            }
        }

        GridLayout {
            id: gridLayout_outputFile
            height: 100
            columns: 2
            rows: 3
            columnSpacing: 0
            rowSpacing: 0
            anchors.left: parent.left
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.right: parent.right

            Label {
                id: label_outputFile
                text: qsTr("Output File")
                topPadding: 6
                bottomPadding: 6
                Layout.columnSpan: 2
                font.wordSpacing: 0
                rightPadding: 6
                font.pointSize: 15
                leftPadding: 6
            }

            TextField {
                id: textField_outputFile
                text: qsTr("")
                font.pointSize: 11
                placeholderText: "/.../some/file/path/outputfile.svg"
                Layout.fillWidth: true
            }

            Button {
                id: button_outputFile
                text: qsTr("Pick File")
                font.pointSize: 12
            }

            Label {
                id: label_outputFileWarning
                text: qsTr("")
                Layout.fillWidth: true
                Layout.columnSpan: 2
                wrapMode: Text.WordWrap
                font.pointSize: 11
            }
        }

        GridLayout {
            id: gridLayout_colorCount
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left

            Label {
                id: label_colorCount
                text: "Color Count"
                topPadding: 6
                bottomPadding: 6
                Layout.columnSpan: 2
                font.wordSpacing: 0
                rightPadding: 6
                font.pointSize: 15
                leftPadding: 6
            }

            TextField {
                id: textField_colorCount
                text: qsTr("")
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                font.pointSize: 11
                placeholderText: "[1-999] or 0 for all colors"
                Layout.fillWidth: true
                validator: IntValidator {
                    bottom: 1
                    top: 999
                }
            }

            Label {
                id: label_colorCountWarning
                text: qsTr("")
                Layout.fillWidth: true
                Layout.columnSpan: 2
                wrapMode: Text.WordWrap
                font.pointSize: 11
            }

            columnSpacing: 0
            columns: 2
            rows: 3
            rowSpacing: 0
            anchors.leftMargin: 0
        }

        GridLayout {
            id: gridLayout_generateSvg
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            columns: 1
            columnSpacing: 0
            rowSpacing: 0
            rows: 1

            Button {
                id: button_generateSvg
                text: qsTr("Generate SVG")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                enabled: false
                rightPadding: 0
                leftPadding: 0
                bottomPadding: 0
                topPadding: 0
                highlighted: true
                flat: false
                font.pointSize: 12
                padding: 0
                wheelEnabled: false
                focusPolicy: Qt.ClickFocus
                display: AbstractButton.TextBesideIcon
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
