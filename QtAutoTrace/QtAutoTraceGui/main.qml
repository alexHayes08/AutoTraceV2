import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.0
import aperture.qtautotracelib 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("QtAutoTraceV2 GUI")

    QtAutoTraceLib {
        id: autoTrace
        onError: {
            stackView.push(resultsPage)
        }

        onReadingImage: {
            if (percentDone == 100) {
                loadingPage.progressBar_readingImage.value = 1;
                loadingPage.busyIndicator_readingImage.running = false
            } else {
                if (!loadingPage.busyIndicator_readingImage.running) {
                    loadingPage.busyIndicator_readingImage.running = true;
                }

                loadingPage.progressBar_readingImage.value = percentDone / 100;
            }
        }

        onGeneratingSplines: {
            if (percentDone == 100) {
                loadingPage.progressBar_generatingSplines.value = 1;
                loadingPage.busyIndicator_generatingSplines.running = false;
            } else {
                if (!loadingPage.busyIndicator_generatingSplines.running) {
                    loadingPage.busyIndicator_generatingSplines.running = true
                }

                loadingPage.progressBar_generatingSplines.value = percentDone / 100;
            }
        }

        onFinished: {
            stackView.push(resultsPage);
        }
    }

    FileDialog {
        id: inputFileDialog
        title: "Select Image"
        nameFilters: "Image files (*.bmp *.png *.xpm *.jpg)"
        onAccepted: {
            var inputFileName = inputFileDialog.fileUrl
                .toString()
                .replace("file:///", "");
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

    LoadingPage {
        id: loadingPage
    }

    ResultsPage {
        id: resultsPage
        button_restart.onClicked: {
            stackView.pop(homeForm);
        }
    }

    HomeForm {
        function getInputOptions() {
            var inputOptions = {
                inputFileName: homeForm.textField_inputFile.text,
                outputFileName: homeForm.textField_outputFile.text,
                colorCount: Number(homeForm.textField_colorCount.text)
            };

            if (Number.isNaN(inputOptions.colorCount)
                    || inputOptions.colorCount < 0) {
                inputOptions.colorCount = 0;
            }

            return inputOptions;
        }

        function onEditingInputOptionsFinished() {
            var inputOptions = getInputOptions();

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
            autoTrace.generateSvg(getInputOptions());
            stackView.push(loadingPage)
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
                text: qsTr("Loading")
                width: parent.width
                onClicked: {
                    stackView.push(loadingPage)
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Results")
                width: parent.width
                onClicked: {
                    stackView.push(resultsPage)
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
