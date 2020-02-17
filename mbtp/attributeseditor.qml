import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: element
    TabBar {
        id: bar
        anchors.right: addAttribute.left
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        TabButton {
            text: qsTr("Attributes")
        }
        TabButton {
            text: qsTr("Requirements")
        }
    }

    StackLayout {
        anchors.bottomMargin: 0
        anchors.top: bar.bottom
        anchors.right: parent.right
        anchors.bottom: save.top
        anchors.left: parent.left
        anchors.topMargin: 0
        currentIndex: bar.currentIndex
        Item {
            id: attributesTab
            TableView {
                id: weiTable
                boundsBehavior: Flickable.StopAtBounds
                reuseItems: false
                rowSpacing: 5
                columnSpacing: 50
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 20
                anchors.topMargin: 20
                objectName: "weiTable"
                anchors.fill: parent
                model: weiModel
                delegate: Row {
                    Text {
                        id: txtItm
                        text: display
                        width: 100
                        font.pointSize: if(model.row == 0){return 15}else if(model.column == 0){return 15}else{return 12}
                        visible: if(model.row == 0 || model.column == 0){return true}else{return false}
                    }
                    SpinBox{
                        id: box
                        height: 32
                        editable: true
                        visible: !txtItm.visible
                        stepSize: 1
                        from: 0
                        to: 100
                        value: parseInt(display)
                        onValueChanged: {
                            if(visible)
                                display = "" + value
                        }
                    }
                }
                ScrollBar.horizontal: ScrollBar{
                    visible: {if(weiTable.contentWidth > weiTable.width){return true}else{return false}}
                    id: hScroll1
                    anchors.top: weiTable.bottom
                    anchors.left: weiTable.left
                    anchors.right: weiTable.right
                    height: 7
                    contentItem: Rectangle{
                        height: 5
                        color: "black"
                    }
                    background: Rectangle{
                        height: 7
                        color: "lightgray"
                    }
                }

                ScrollBar.vertical: ScrollBar{
                    visible: {if(weiTable.contentHeight > weiTable.height){return true}else{return false}}
                    id: vScroll1
                    anchors.top: weiTable.top
                    anchors.left: weiTable.right
                    anchors.bottom: weiTable.bottom
                    width: 7
                    contentItem: Rectangle{
                        width: 5
                        color: "black"
                    }
                    background: Rectangle{
                        width: 7
                        color: "lightgray"
                    }
                }
            }
        }



        Item {
            id: requirementsTab
            TableView {
                id: reqTable
                boundsBehavior: Flickable.StopAtBounds
                reuseItems: false
                rowSpacing: 5
                columnSpacing: 100
                anchors.rightMargin: 20
                anchors.leftMargin: 20
                anchors.bottomMargin: 20
                anchors.topMargin: 20
                objectName: "reqTable"
                anchors.fill: parent
                model: reqModel
                delegate: Row{
                    height: 32
                    Text {
                        id: txtItm2
                        text: display
                        width: 100
                        font.pointSize: if(model.row == 0){return 15}else if(model.column == 0){return 15}else{return 10}
                        visible: if(model.row == 0 || model.column == 0){return true}else{return false}
                    }
                    SpinBox{
                        id: box2
                        height: 30
                        editable: true
                        visible: !txtItm2.visible
                        stepSize: 1
                        from: 0
                        to: 100
                        value: parseInt(display)
                        onValueChanged: {
                            if(visible)
                                display = "" + value
                        }
                    }
                }
                ScrollBar.horizontal: ScrollBar{
                    visible: {if(reqTable.contentWidth > reqTable.width){return true}else{return false}}
                    id: hScroll2
                    anchors.top: reqTable.bottom
                    anchors.left: reqTable.left
                    anchors.right: reqTable.right
                    height: 7
                    contentItem: Rectangle{
                        height: 5
                        color: "black"
                    }
                    background: Rectangle{
                        height: 7
                        color: "lightgray"
                    }
                }

                ScrollBar.vertical: ScrollBar{
                    visible: {if(reqTable.contentHeight > reqTable.height){return true}else{return false}}
                    id: vScroll2
                    anchors.top: reqTable.top
                    anchors.left: reqTable.right
                    anchors.bottom: reqTable.bottom
                    width: 7
                    contentItem: Rectangle{
                        width: 5
                        color: "black"
                    }
                    background: Rectangle{
                        width: 7
                        color: "lightgray"
                    }
                }
            }
        }
    }

    Button {
        id: save
        objectName: "saveButton"
        text: qsTr("Save")
        font.pixelSize: height/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
    }

    Button {
        id: close
        objectName: "closeButton"
        text: qsTr("Close")
        font.pixelSize: height/2
        anchors.right: save.left
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    Button {
        id: reLayout
        objectName: "forceRelayout"
        text: qsTr("Reset Layout")
        font.pixelSize: height/2
        anchors.right: close.left
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onClicked: {
            reqTable.forceLayout()
            weiTable.forceLayout()
        }
    }

    Button {
        id: importComplexity
        x: 130
        y: 440
        text: qsTr("Import Cyclomatic Complexity")
        font.pixelSize: height/2
        objectName: "importCC"
        anchors.bottom: parent.bottom
        anchors.right: close.left
        anchors.bottomMargin: 0
        anchors.rightMargin: 158
    }

    Button {
        id: addAttribute
        width: 70
        objectName: "addAttributeButton"
        text: qsTr("Add\nAttribute")
        display: AbstractButton.TextOnly
        anchors.rightMargin: 0
        anchors.bottom: bar.bottom
        anchors.right: renameAttribute.left
        anchors.top: parent.top
        anchors.bottomMargin: 0
    }

    Button {
        id: renameAttribute
        x: -2
        y: 1
        width: 70
        text: qsTr("Rename\nAttribute")
        display: AbstractButton.TextOnly
        anchors.rightMargin: 0
        objectName: "renameAttributeButton"
        anchors.right: removeAttribute.left
        anchors.bottomMargin: 0
        anchors.bottom: bar.bottom
        anchors.top: parent.top
    }

    Button {
        id: removeAttribute
        width: 70
        objectName: "removeAttributeButton"
        text: qsTr("Remove\nAttribute")
        display: AbstractButton.TextOnly
        anchors.bottom: bar.bottom
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottomMargin: 0
    }



}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
