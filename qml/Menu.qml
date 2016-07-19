import QtQuick 1.0

Rectangle {

    property alias daemonStatus:  daemonStatus.text

    id: menu
    width: mainWindow.width
    height: mainWindow.height - header.height + 7
    color: "transparent"

    x: 0
    y: 0 - height

    state: "Hidden"

    states: [
	State {
	    name: "Hidden"
	    PropertyChanges {
		target: menu
		y: 0 - menu.height
	    }
	}
	,
	State {
	    name: "Shown"
	    PropertyChanges {
		target: menu
		y: header.height - 7
	    }
	    PropertyChanges {
		target: daemonStatus
		text: "<b>Daemon Status:</b> " + getDaemonStatus();
	    }
	}

    ]

    transitions: [
	Transition {
	    to: "*"
	    SmoothedAnimation {
		property: "y"
		target: menu
		duration: 400
	    }
	}
    ]


    Rectangle {

	id: menuArea

	anchors.top: parent.top
	anchors.left: parent.left
	anchors.right: parent.right
	height: 200
 gradient: Gradient {
     GradientStop {
	 position: 0
	 color: "#111111"
     }

     GradientStop {
	 position: 0.91
	 color: "#000000"
     }

     GradientStop {
	 position: 0.92
	 color: "#80000000"
     }

     GradientStop {
	 position: 1
	 color: "#00000000"
     }
 }



    Flickable {
	id: flickable1
	contentHeight: menuArea.height
	contentWidth: 2 * menuArea.width
	boundsBehavior: Flickable.DragOverBounds
	anchors.fill: parent
	flickableDirection: Flickable.HorizontalFlick
	flickDeceleration: -1

	MouseArea {
	    id: mousearea1
     width: menuArea.width
     anchors.bottom: parent.bottom
     anchors.left: parent.left
     anchors.top: parent.top

	    Text {
		anchors.left:  parent.left
		anchors.bottom: parent.bottom
		anchors.leftMargin: 20
		anchors.bottomMargin: 50
		font.pixelSize: 20
		color: "#fff"
		text: "<b>Version:</b> 0.3.3"
	    }

	    Text {
		id: daemonStatus
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.leftMargin: 20
		anchors.rightMargin: 20
		anchors.topMargin: 40
		height: 20

		font.pixelSize: 20
		horizontalAlignment: "AlignLeft"
		verticalAlignment: "AlignVCenter"
		color: "#fff"

		text: "<b>Daemon Status:</b> Not Running"
	    }

	    Button {
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		width: 200
		height: 60
		anchors.bottomMargin: 40
		anchors.rightMargin: 20


		onClicked: restartDaemonClicked()

		text: "Apply Changes"
		opacity: 1
	    }




	}

	MouseArea {
	    id: mousearea2
     width: menuArea.width
	    anchors.left: mousearea1.right
	    anchors.top: mousearea1.top
	    anchors.bottom: mousearea1.bottom

	    Button {
		text: "Unlock All"
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.bottomMargin: 40
		anchors.rightMargin: 20
		width: 200
		height: 60

		onClicked: unlockAll();
	    }

     Text {
	 color: "#fdfdfd"
	 text: "Powered by <b>Sierra Softworks</b> Technology"
  scale: 1
  z: 0
  rotation: 0
  transformOrigin: Item.Center
  horizontalAlignment: Text.AlignHCenter
	 wrapMode: Text.WordWrap
	 verticalAlignment: Text.AlignVCenter
	 anchors.right: parent.right
	 anchors.rightMargin: 20
	 anchors.left: parent.left
	 anchors.leftMargin: 20
	 anchors.top: parent.top
	 anchors.topMargin: 20
	 font.pixelSize: 30
     }
    }


    }
    }

    MouseArea {
 anchors.bottom: parent.bottom
 anchors.bottomMargin: 0
 anchors.right: parent.right
 anchors.rightMargin: 0
 anchors.left: parent.left
 anchors.leftMargin: 0
 anchors.top: menuArea.bottom
 anchors.topMargin: 0

 onClicked: menu.state = "Hidden"
    }
}
