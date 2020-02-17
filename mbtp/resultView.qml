import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4

Item {
//    anchors.fill: parent
    id: window;
RowLayout {
    anchors.fill: parent
    Item {
        id: tableItem
        Layout.fillHeight: true
        Layout.minimumWidth: 300
        Layout.minimumHeight: 300
        ColumnLayout{
            anchors.fill: parent
            spacing: 0

            Rectangle{
                id: headlineTestCases
                color: "steelblue"
                border.color: "grey"

                Text{
                    id: textheadlineTestCases
                    color: "white"
                    text: "Test Cases"
                    font.pointSize: 12; font.bold: true
                }
                Layout.fillWidth: true
                height: textheadlineTestCases.height
            }

            TableView  {
                id: testCasesTable
                Layout.fillHeight: true
                Layout.fillWidth: true
                TableViewColumn{
                    width: 40
                    role: "one"
                    title: "#"
                }
                TableViewColumn{
                    width: 100
                    role: "two"
                    title: "Testcase"
                }
                TableViewColumn{
                    width: 100
                    role: "three"
                    title: "Prio"
                }
                TableViewColumn{
                    width: 40
                    role: "four"
                    title: "Cluster"
                }

                model: mbtpModel
            }
        }
    }
    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.minimumWidth: 300
        Layout.minimumHeight: 300
        clip: true
        Image {
            width: parent.width
            height: parent.height
            id: viewItem
            fillMode: Image.PreserveAspectFit
            verticalAlignment: Image.AlignLeft
            horizontalAlignment: Image.AlignTop
            cache: false
            smooth: true
            clip: true
            objectName: "viewItem"
            source: "image://resultImage/b"
            MouseArea{
                anchors.fill: parent
                drag.target: parent
                onWheel: {
                    if(Qt.ControlModifier & wheel.modifiers){
                        if(parent.scale + wheel.angleDelta.y / 4000 > 0.01 & parent.scale + wheel.angleDelta.y / 4000 < 10){
                            parent.scale += wheel.angleDelta.y / 4000
                        }
                    }
                }
            }
        }
    }
}
}
