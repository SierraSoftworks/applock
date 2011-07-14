import QtQuick 1.0

MouseArea {

    property alias text: text1.text
    property color topColor: "#e00"
    property color bottomColor: "#622"
    property color clickTopColor: "#800"
    property color clickBottomColor: "#622"
    property alias textColor: text1.color
    property alias radius: fillRect.radius
    property alias smooth: fillRect.smooth

    x: 0
    y: 0
    width: 200
    height: 60

    states: [
	State {
	    name: "Clicked"
	    PropertyChanges {
		target: gradTop
		color: clickTopColor
	    }
	    PropertyChanges {
		target: gradBottom
		color: clickBottomColor
	    }
	}
	,
	State {
	    name: "Normal"
	    PropertyChanges {
		target: gradTop
		color: topColor
	    }
	    PropertyChanges {
		target: gradBottom
		color: bottomColor
	    }
	}

    ]

    onPressed: state = "Clicked"
    onReleased: state = "Normal"

    Rectangle {
	id: fillRect
	radius: 10
	gradient: Gradient {
	    GradientStop {
		id: gradTop
		position: 0
		color: "#ee0000"
	    }

	    GradientStop {
		id: gradBottom
		position: 1
		color: "#662222"
	    }
	}
	smooth: true
	anchors.fill: parent



 Text {
     id: text1
     x: 88
     y: 23
     color: "#ffffff"
     text: "Unlock All"
     verticalAlignment: Text.AlignVCenter
     horizontalAlignment: Text.AlignHCenter
     anchors.horizontalCenter: parent.horizontalCenter
     anchors.verticalCenter: parent.verticalCenter
     font.pixelSize: 20
 }
    }
}
