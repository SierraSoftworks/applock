import QtQuick 1.0


Rectangle {
    property alias text: notificationText.text
    property int topOffset: 30
    property alias displayTime: timer.interval


    function show(notifyText)
    {
	text = notifyText;
	state = "Shown"
    }

    function showWithDelay(notifyText, time)
    {
	text = notifyText;
	displayTime = time;
	state = "Shown"
	timer.start();
    }

    function hide()
    {
	state = "Hidden"
    }

    Timer {
	id: timer
	repeat: false
	triggeredOnStart: false

	onTriggered: state = "Hidden"

    }

    id: notificationBar
    y: 0 - height
    width: parent.width
    height: 60
    anchors.horizontalCenter: parent.horizontalCenter
    gradient: Gradient {
	GradientStop {
	    position: 0.00;
	    color: "#f3e680";
	}
	GradientStop {
	    position: 0.94;
	    color: "#c0a010";
	}
	GradientStop {
	    position: 0.95;
	    color: "#00000080";
	}
	GradientStop {
	    position: 1.00;
	    color: "#00000000";
	}
    }
    smooth: true

states: [
 State {
     name: "Hidden"
     PropertyChanges {
	 target: notificationBar
	 y: 0 - height
     }
 }
 ,
 State {
     name: "Shown"
     PropertyChanges {
	 target: notificationBar
	 y: topOffset
     }
 }
]

transitions: [
 Transition {
     to: "*"
     NumberAnimation { target: notificationBar; property: "y"; duration: 200 }
 }
]

Text {
 id: notificationText
 text: "Loading..."
 anchors.bottomMargin: 5
 anchors.leftMargin: 20
 horizontalAlignment: Text.AlignLeft
 verticalAlignment: Text.AlignVCenter
 anchors.fill: parent
 font.pixelSize: 30
}
}

