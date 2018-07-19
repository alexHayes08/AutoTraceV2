import QtQuick 2.11
import QtQuick.Controls 2.4
import aperture.qtautotracelib 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("QtAutoTraceV2 GUI")

    QtAutoTraceLib {
        id: autoTrace
    }

    InputOptions {
        id: inputOptions
    }

    FileDialog {
        id: inputFileDialog
        title: "Select Image"
        nameFilters: "Image files (*.png *.xpm *.jpg)"
        onAccepted: {
            var inputFileName = inputFileDialog.fileUrl.toString();
            homeForm.textField_inputFile.text = inputFileName;

            if (homeForm.textField_outputFile.text == null
                    || homeForm.textField_outputFile.text === "")
            {
                var outputName = inputFileName
                    .substring(0, inputFileName.lastIndexOf("."))
                    .concat(".svg");
                homeForm.textField_outputFile.text = outputName;
            }
        }
    }

    FileDialog {
        id: outputFileDialog
        title: "Select Image"
        nameFilters: "*.svg"
        onAccepted: {
            homeForm.textField_outputFile.text = outputFileDialog
                .fileUrl.toString();
        }
    }

    HomeForm {
        function onEditingInputOptionsFinished() {
            inputOptions.inputFileName = homeForm.textField_inputFile.text;
            inputOptions.outputFileName = homeForm.textField_outputFile.text;
            var colorCount = Number(homeForm.textField_colorCount.text);

            if (Number.isNaN(colorCount) || colorCount < 0) {
                inputOptions.colorCount = 0;
            } else {
                inputOptions.colorCount = colorCount;
            }

            var validInputFileName = inputOptions.inputFileName !== null
                || inputOptions !== "";

            var validOutputFileName = inputOptions.outputFileName !== null
                || inputOptions.outputFileName !== "";

            if (!validInputFileName) {
                homeForm.label_inputFileWarning.text = "Invalid file name."
            } else {
                homeForm.label_inputFileWarning.text = "";
            }

            if (!validOutputFileName) {
                homeForm.label_outputFileWarning.text = "Invalid file name."
            } else {
                homeForm.label_outputFileWarning.text = "";
            }

            homeForm.button_generateSvg.enabled = validInputFileName
                && validOutputFileName
        }
        id: homeForm
        button_inputFile.onClicked: {
            inputFileDialog.visible = true
        }
        button_outputFile.onClicked: {
            outputFileDialog.visible = true
        }
        textField_inputFile.onTextChanged: {
            onEditingInputOptionsFinished();
        }
        textField_outputFile.onTextChanged: {
            onEditingInputOptionsFinished();
        }
        button_generateSvg.onClicked: {
            autoTrace.generateSvg(inputOptions);
        }
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Page 1")
                width: parent.width
                onClicked: {
                    stackView.push("Page1Form.ui.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Page 2")
                width: parent.width
                onClicked: {
                    stackView.push("Page2Form.ui.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: homeForm
        anchors.fill: parent
    }
}
