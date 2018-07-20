import QtQuick 2.11
import QtQuick.Controls 2.4

Page {
    width: 600
    height: 400

    title: qsTr("Results")

    property alias button_restart: button_restart

    Column {
        id: column
        anchors.fill: parent

        Row {
            id: row
            height: 40
            spacing: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            layoutDirection: Qt.LeftToRight

            Label {
                id: label_result
                y: 0
                text: qsTr("Generated Svg")
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
            }

            Button {
                id: button_restart
                text: qsTr("Restart")
            }
        }

        Image {
            id: image
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.Stretch
            source: "placeholder.svg"
        }
    }
}

/*##^## Designer {
    D{i:3;anchors_height:400;anchors_width:200;anchors_x:194;anchors_y:126}
}
 ##^##*/
