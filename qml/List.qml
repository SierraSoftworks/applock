import QtQuick 1.0

ListView {

    property int topOffset: 40

    id: listView
    x: 0
    y: topOffset
    width: mainWindow.width
    height: mainWindow.height - topOffset
    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0
    anchors.top: parent.top
    anchors.topMargin: 50

    highlightMoveSpeed: -1
    highlightMoveDuration: 0
    highlight: Rectangle {
	width: mainWindow.width
	height: 80
	gradient: Gradient {
	    GradientStop {
		color: "#e00"
		position: 0
	    }
	    GradientStop {
		color: "#622"
		position: 0.9
	    }
	    GradientStop {
		color: "#a0000000"
		position: 0.91
	    }
	    GradientStop {
		color: "transparent"
		position: 1
	    }
	}
    }


    delegate:
	MouseArea {
     y: 0
	    width:  mainWindow.width
	    height: 80
	    Image {
		source: modelData.icon !== "" ? "file://" + modelData.icon : ""
		anchors.top: parent.top
		anchors.topMargin: 4
		anchors.left: parent.left
		anchors.leftMargin: 4
		width: 64
		height: 64
	    }
	    Text {
		text: modelData.name
		anchors.fill: parent
		anchors.leftMargin: 84
		anchors.bottomMargin: 16
		anchors.topMargin: 10
		verticalAlignment: "AlignTop"
		color: "#000"
		font.pixelSize: 30
	    }
	    Text {
		text: modelData.description
		anchors.fill: parent
		anchors.leftMargin: 84
		anchors.bottomMargin: 10
		anchors.topMargin: 10
		color: "#000"
		font.pixelSize: 20
		verticalAlignment: "AlignBottom"
	    }
	    Image {
		source: "qrc:/qml/Lock.png"
		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.verticalCenter: parent.verticalCenter
		height: 32
  anchors.verticalCenterOffset: -4
		fillMode: Image.PreserveAspectFit
		width: 32
		visible: modelData.locked
	    }

	    onClicked: onListItemClicked(index, modelData.name)
	}
}
