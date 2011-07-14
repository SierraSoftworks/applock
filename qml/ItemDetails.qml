import QtQuick 1.0

Rectangle {

    property alias itemName: itemPropName.text
    property alias itemPath: itemDBus.text
    property alias itemDescription: itemPath.text
    property alias itemStatus: itemPropStatus.text
    property alias itemIcon: itemIcon.source
    property alias buttonText: btnLock.text

    id: itemProperties
    x: 0
    y: mainWindow.height
width: mainWindow.width
height: mainWindow.height
color: "#64000000"
    z: 2
visible: true
    state: "Hidden"


    states: [
	State {
	    name: "Hidden"
	    PropertyChanges {
		target: itemProperties
		y: mainWindow.height
	    }
	},
	State {
	    name: "Shown"
	    PropertyChanges {
		target: itemProperties
		y: mainWindow.height - height
	    }
	}
    ]

    transitions: [
	Transition {
	    to: "*"
	    NumberAnimation { target: itemProperties; property: "y"; duration: 250; easing.type: Easing.InCubic }
	}
    ]



Rectangle {
 y: 198
 height: 300
 gradient: Gradient {
     GradientStop {
	 position: 0
	 color: "#00333333"
     }

     GradientStop {
	 position: 0.02
	 color: "#a0000000"
     }

     GradientStop {
	 position: 0.03
	 color: "#333333"
     }

     GradientStop {
	 position: 0.16
	 color: "#000000"
     }

     GradientStop {
	 position: 1
	 color: "#111111"
     }
 }
 anchors.right: parent.right
 anchors.rightMargin: 0
 anchors.left: parent.left
 anchors.leftMargin: 0
 anchors.bottom: parent.bottom
 anchors.bottomMargin: 0

 MouseArea {
     anchors.fill: parent

     Text {
	 id: itemPropName
	 x: 81
	 height: 64
	 color: "#ffffff"
	 text: "Item Name"
	 horizontalAlignment: Text.AlignHCenter
	 verticalAlignment: Text.AlignVCenter
	 wrapMode: Text.WordWrap
	 anchors.right: parent.right
	 anchors.rightMargin: 84
	 anchors.top: parent.top
	 anchors.topMargin: 10
	 anchors.left: parent.left
	 anchors.leftMargin: 84
	 font.pixelSize: 35
     }

     Image {
	 id: itemIcon
	 width: 64
	 height: 64
	 source: "../AppLock.png"
	 anchors.left: parent.left
	 anchors.leftMargin: 20
	 anchors.top: parent.top
	 anchors.topMargin: 14
	 visible: true
	 smooth: true
     }

     Text {
	 id: itemPropStatus
	 y: 308
	 color: "#ffffff"
	 text: "This application is currently unlocked. Click the lock button to lock it"
	 verticalAlignment: Text.AlignVCenter
	 horizontalAlignment: Text.AlignHCenter
	 anchors.left: parent.left
	 anchors.leftMargin: 20
	 anchors.bottom: parent.bottom
	 anchors.bottomMargin: 80
	 anchors.right: parent.right
	 anchors.rightMargin: 20
	 wrapMode: Text.WordWrap
	 font.pixelSize: 20
     }

     Text {
	 id: itemPath
	 color: "#ffffff"
	 text: "<b>Path:</b>"
	 wrapMode: Text.WordWrap
	 anchors.right: parent.right
	 anchors.rightMargin: 20
	 anchors.top: parent.top
	 anchors.topMargin: 90
	 anchors.left: parent.left
	 anchors.leftMargin: 20
	 font.pixelSize: 20
     }

     Text {
	 id: itemDBus
	 color: "#ffffff"
	 text: "<b>DBus Service:</b>"
	 wrapMode: Text.WordWrap
	 horizontalAlignment: Text.AlignLeft
	 anchors.right: parent.right
	 anchors.rightMargin: 20
	 anchors.top: parent.top
	 anchors.topMargin: 130
	 anchors.left: parent.left
	 anchors.leftMargin: 20
	 font.pixelSize: 20
     }




 }

 Rectangle {
     y: 241
     height: 59
gradient: Gradient {
  GradientStop {
      position: 0
      color: "#222222"
  }

  GradientStop {
      position: 0.47
      color: "#03000000"
  }
}
anchors.right: parent.right
anchors.rightMargin: 0
anchors.left: parent.left
anchors.leftMargin: 0
anchors.bottom: parent.bottom
anchors.bottomMargin: 0

     MouseArea {
	 x: 592
	 y: 0
	 width: 208
	 height: 59
	 anchors.bottom: parent.bottom
	 anchors.bottomMargin: 0
	 anchors.right: parent.right
	 anchors.rightMargin: 0

	 onClicked: onLockButtonClicked(itemPropName.text)

	 Text {
      id: btnLock
	     color: "#ffffff"
	     text: "Lock"
font.pixelSize: 40
	     horizontalAlignment: Text.AlignHCenter
	     verticalAlignment: Text.AlignVCenter
	     anchors.fill: parent
	 }
     }
 }






}

MouseArea {
 anchors.bottom: parent.bottom
 anchors.bottomMargin: 300
 anchors.right: parent.right
 anchors.rightMargin: 0
 anchors.left: parent.left
 anchors.leftMargin: 0
 anchors.top: parent.top
 anchors.topMargin: 0

 onClicked: hideItemProperties();
}


}

