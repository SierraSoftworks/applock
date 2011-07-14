import QtQuick 1.0

Rectangle {
    id: header
x: 0
y: 79
z: 10
    height: 55
gradient: Gradient {
 GradientStop {
     position: 0
     color: "#333333"
 }

 GradientStop {
     position: 0.86
     color: "#111111"
 }

 GradientStop {
     position: 0.87
     color: "#96000000"
 }

 GradientStop {
     position: 1
     color: "#00111111"
 }
}
anchors.right: parent.right
anchors.rightMargin: 0
anchors.left: parent.left
anchors.leftMargin: 0
anchors.top: parent.top
anchors.topMargin: 0


MouseArea {
 id: mouseAreaMenu
 x: 100
 y: 0
 width: 594
 height: 55
 anchors.rightMargin: 100
 anchors.leftMargin: 100
 anchors.fill: parent
 onClicked: menuButtonClicked()
 onPressed: menuButtonRect.color = "#000"
 onReleased: menuButtonRect.color = "transparent"

 Rectangle {
     id: menuButtonRect
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: -5
    width: 250
    height: 40
    z: 11
    color: "transparent"
    border.color: "#666"
    border.width: 1
    radius: 5
    smooth: true
    Text {

	id: headerText
	anchors.fill: parent
	color: "#ffffff"
	text: "AppLock"
	anchors.horizontalCenter: parent.horizontalCenter
	anchors.verticalCenter: parent.verticalCenter
	anchors.bottomMargin: 5
	font.pixelSize:30
	verticalAlignment: "AlignVCenter"
	horizontalAlignment: "AlignHCenter"
	z: 12
    }
 }


}

MouseArea {
 id: mouseAreaClose
 y: 0
 height: 55
 width:  100
 anchors.top: parent.top
 anchors.topMargin: 0
 anchors.bottom: parent.bottom
 anchors.bottomMargin: 0
 anchors.right: parent.right
 anchors.rightMargin: 0
 onClicked: exitClicked()

 onPressed: closeText.color = "#e00";
 onReleased: closeText.color = "#fff";

 Text {
     id: closeText
     x: 36
     y: 20
     color: "#ffffff"
     text: "X"
     font.bold: true
     anchors.horizontalCenter: parent.horizontalCenter
     anchors.verticalCenter: parent.verticalCenter
     anchors.verticalCenterOffset: -5
     verticalAlignment: "AlignVCenter"
     font.pixelSize: 34
 }
}

MouseArea {
 id: mouseAreaMinimize
 width: 100
 anchors.left: parent.left
 anchors.leftMargin: 0
 anchors.bottom: parent.bottom
 anchors.bottomMargin: 0
 anchors.top: parent.top
 anchors.topMargin: 5
 onClicked: minimizeClicked()

 onPressed: {
     minimizeGradTop.color = "#e00"
     minimizeGradBottom.color = "#622"
 }

 onReleased: {
     minimizeGradTop.color = "#fff"
     minimizeGradBottom.color = "#aaa"
 }

 Rectangle {
     x: 5
     y: 11
     width: 40
     height: 30
     radius: 5
    anchors.topMargin: 2

     gradient: Gradient {
	 GradientStop {
	     id: minimizeGradTop
	     position: 0
	     color: "#ffffff"
	 }

	 GradientStop {
	     id: minimizeGradBottom
	     position: 1
	     color: "#aaaaaa"
	 }
     }
     anchors.horizontalCenter: parent.horizontalCenter
     anchors.top: parent.top
     smooth: true
 }
}
}
