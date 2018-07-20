import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 600
    height: 400

    title: qsTr("Loading")

    property alias progressBar_readingImage: progressBar_readingImage
    property alias busyIndicator_readingImage: busyIndicator_readingImage
    property alias progressBar_generatingSplines: progressBar_generatingSplines
    property alias busyIndicator_generatingSplines: busyIndicator_generatingSplines

    GridLayout {
        id: gridLayout
        columnSpacing: 0
        rowSpacing: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        columns: 2

        Label {
            id: label_readingImage
            text: "Reading Image"
            padding: 6
            Layout.columnSpan: 2
            font.pointSize: 11
        }

        ProgressBar {
            id: progressBar_readingImage
            Layout.fillWidth: true
            value: 0
        }

        BusyIndicator {
            id: busyIndicator_readingImage
            wheelEnabled: false
        }

        Label {
            id: label_generatingSplines
            text: "Generating Splines"
            padding: 6
            Layout.columnSpan: 2
            font.pointSize: 11
        }

        ProgressBar {
            id: progressBar_generatingSplines
            Layout.fillWidth: true
            value: 0
        }

        BusyIndicator {
            id: busyIndicator_generatingSplines
            running: false
            wheelEnabled: true
        }
    }
}
