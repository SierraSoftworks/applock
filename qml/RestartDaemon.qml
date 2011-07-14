import QtQuick 1.0

MouseArea {
    id: dlgMA
    function hide()
    {
	state = "Hidden";
    }

    function show()
    {
	state = "Shown"
    }

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width:  parent.width


    state: "Hidden"
    states: [
	    State {
		name: "Hidden"
		PropertyChanges {
		    target: dlgMA
		    x: dlgMA.parent.width
		}

	    },
	    State {
		name: "Shown"
		PropertyChanges {
		    target: dlgMA
		    x: 0
		}
	    }
	]

	transitions: [
	    Transition {
		to: "*"
		NumberAnimation { target: dlgMA;property: "x"; duration: 200 }
	    }
	]


    Rectangle {
	id: dlg
	width: parent.width / 2
	height: parent.height / 2
	anchors.centerIn: parent

	radius: 10
	smooth: true

	gradient: Gradient
	{
	    GradientStop {
		color: "#444"
		position: 0
	    }
	    GradientStop {
		color: "#111"
		position: 1
	    }
	}

 Button {
     id: btnStartDaemon
     y: 141
     width: 200
     text: "Start Daemon"
     anchors.horizontalCenter: parent.horizontalCenter
     drag.maximumX: 1000
     anchors.bottom: parent.bottom
     anchors.bottomMargin: 20

     onClicked: {
	 startDaemon();
	 hide();
     }
 }

 Text {
     id: text1
     color: "#ffffff"
     text: "Click <b>Start Daemon</b> to continue"
     anchors.bottom: parent.bottom
     anchors.bottomMargin: 100
     anchors.top: parent.top
     anchors.topMargin: 0
     anchors.right: parent.right
     anchors.rightMargin: 0
     anchors.left: parent.left
     anchors.leftMargin: 0
     font.pixelSize: 20
     wrapMode: Text.WordWrap
     font.family: "Ubuntu"
     verticalAlignment: Text.AlignVCenter
     horizontalAlignment: Text.AlignHCenter
 }


    }

}

