import QtQuick 1.0


Rectangle {


    function getScrollBarHeight(containterHeight, itemHeight, totalItems)
    {
	var showPercentage = (totalItems > containterHeight / itemHeight) ?
		containterHeight / (itemHeight * totalItems) : 1

	return containterHeight * showPercentage
    }

    function getScrollBarPosition(containerHeight, itemHeight, currentTopItem, totalItems)
    {
	var showPercentage = (totalItems > containerHeight / itemHeight) ?
		currentTopItem / totalItems : 0

	return containerHeight * showPercentage
    }


    width: 5
    height: 400

}
