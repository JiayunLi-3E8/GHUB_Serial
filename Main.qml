import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import GHUBSerial

ApplicationWindow {
    id: window
    width: 320
    height: 320
    visible: true
    title: qsTr("串口配置")

    SerialPortManager {
        id: serialPortManager
        portName: settings.port
        baudRate: settings.baudRate

        byteSize: {
            switch (settings.byteSize) {
            case 5:
                return SerialPortManager.Data5
            case 6:
                return SerialPortManager.Data6
            case 7:
                return SerialPortManager.Data7
            case 8:
                return SerialPortManager.Data8
            default:
                return SerialPortManager.Data8
            }
        }

        parity: {
            switch (settings.parity) {
            case "None":
                return SerialPortManager.NoParity
            case "Even":
                return SerialPortManager.EvenParity
            case "Odd":
                return SerialPortManager.OddParity
            default:
                return SerialPortManager.NoParity
            }
        }

        stopBits: {
            switch (settings.stopBits) {
            case 1:
                return SerialPortManager.OneStop
            case 2:
                return SerialPortManager.TwoStop
            default:
                return SerialPortManager.OneStop
            }
        }

        Component.onCompleted: {
            serialPortManager.open()
        }
    }

    GlobalKeyboardHook {
        onKeyPressed: pressedKey => {
                          console.log("Global Key Pressed:", pressedKey)
                          switch (pressedKey) {
                              case Qt.Key_F13:
                              serialPortManager.sendCommand("Enable")
                              break
                              case Qt.Key_F14:
                              serialPortManager.sendCommand("Disable")
                              break
                              case Qt.Key_F15:
                              serialPortManager.sendCommand("Berrey")
                              break
                              case Qt.Key_F16:
                              serialPortManager.sendCommand("Aug")
                              break
                              default:
                              break
                          }
                      }
    }

    SystemTrayIcon {
        id: tray
        visible: true
        icon.source: "qrc:/qt/qml/GHUBSerial/res/TrayIcon.ico"
        tooltip: "GHUB串口"

        menu: Menu {
            MenuItem {
                text: qsTr("串口配置")
                onTriggered: {
                    window.show()
                    window.raise()
                    window.requestActivate()
                }
            }
            MenuItem {
                text: qsTr("退出")
                onTriggered: Qt.quit()
            }
        }

        onActivated: reason => {
                         if (reason === SystemTrayIcon.DoubleClick
                             || reason === SystemTrayIcon.Trigger) {
                             window.show()
                             window.raise()
                             window.requestActivate()
                         }
                     }
    }

    onClosing: close => {
                   tray.visible = true
                   window.visible = false
                   close.accepted = false
               }

    Settings {
        id: settings
        property string port: ""
        property int baudRate: 115200
        property int byteSize: 8
        property string parity: "None"
        property int stopBits: 1
        category: "Serial"
    }

    SerialPortInfoModel {
        id: serialPortInfoModel
    }

    AutoStartManager {
        id: autoStart
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 20
        columns: 2
        rowSpacing: 12
        columnSpacing: 16

        Label {
            text: qsTr("开机自启：")
        }
        Switch {
            checked: autoStart.isAutoStartEnabled()
            onCheckedChanged: autoStart.setAutoStartEnabled(checked)
        }

        Label {
            text: qsTr("端口：")
        }
        ComboBox {
            id: portCombo
            Layout.fillWidth: true
            model: serialPortInfoModel
            textRole: "portName"
            delegate: ItemDelegate {
                id: delegate
                required property string portName
                required property string description

                width: ListView.view.width
                text: portName + " (" + description + ")"
                background: Rectangle {
                    anchors.fill: parent
                    color: delegate.hovered ? "#459BAC" : "transparent"
                }
            }
            popup.onOpened: serialPortInfoModel.refresh()
            onCurrentTextChanged: settings.port = currentText
        }

        Label {
            text: qsTr("波特率：")
        }
        ComboBox {
            id: baudCombo
            Layout.fillWidth: true
            model: serialPortInfoModel.baudRates
            onCurrentIndexChanged: settings.baudRate = model[currentIndex]
        }

        Label {
            text: qsTr("数据位：")
        }
        ComboBox {
            id: byteSizeBox
            Layout.fillWidth: true
            model: [5, 6, 7, 8]
            onCurrentIndexChanged: settings.byteSize = model[currentIndex]
        }

        Label {
            text: qsTr("校验位：")
        }
        ComboBox {
            id: parityBox
            Layout.fillWidth: true
            model: ["None", "Even", "Odd"]
            onCurrentIndexChanged: settings.parity = model[currentIndex]
        }

        Label {
            text: qsTr("停止位：")
        }
        ComboBox {
            id: stopBitsBox
            Layout.fillWidth: true
            model: [1, 2]
            onCurrentIndexChanged: settings.stopBits = model[currentIndex]
        }

        Button {
            text: qsTr("恢复默认配置")
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                settings.port = ""
                settings.baudRate = 115200
                settings.byteSize = 8
                settings.parity = "None"
                settings.stopBits = 1
                // 刷新界面
                refreshAllComboBoxes()
            }
        }
        Button {
            text: qsTr("应用配置")
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                serialPortManager.close()
                serialPortManager.open()
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: serialPortManager.isOpen ? qsTr("串口状态：" + serialPortManager.currentPortName
                                                  + "已连接") : qsTr("串口状态：未连接")
        }
        Button {
            text: qsTr("退出")
            Layout.alignment: Qt.AlignHCenter
            onClicked: Qt.quit()
        }
    }

    Component.onCompleted: refreshAllComboBoxes()

    function refreshAllComboBoxes() {
        // 端口
        portCombo.currentIndex = portCombo.find(settings.port)
        // 波特率
        baudCombo.currentIndex = baudCombo.model.indexOf(settings.baudRate)
        // 数据位
        byteSizeBox.currentIndex = byteSizeBox.model.indexOf(settings.byteSize)
        // 校验位
        parityBox.currentIndex = parityBox.model.indexOf(settings.parity)
        // 停止位
        stopBitsBox.currentIndex = stopBitsBox.model.indexOf(settings.stopBits)
    }
}
