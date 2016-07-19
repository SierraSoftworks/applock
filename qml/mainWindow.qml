import QtQuick 1.0
import Sierra 1.1

Rectangle {
    id: mainWindow
    width: 800
    height: 430
    gradient: Gradient {
	GradientStop {
	    position: 0
	    color: "#ffffff"
	}

	GradientStop {
	    position: 0.75
	    color: "#ffffff"
	}

	GradientStop {
	    position: 1
	    color: "#aaaaaa"
	}
    }


    function isDBus(itemName)
    {
	return daemon.IsService(itemName);
    }

    function isComplex(itemName)
    {
	return daemon.IsComplex(itemName);
    }

    function isScript(itemName)
    {
	return daemon.IsScript(itemName);
    }


    function isLocked(itemName)
    {
	return daemon.IsMonitored(itemName)
    }

    function appIcon(itemName)
    {
	return "file://" + daemon.getAppIcon(itemName)
    }

    function getDaemonStatus()
    {
	return daemon.DaemonStatus();
    }





    function hideItemProperties()
    {
	itemDetails.state = "Hidden"
    }

    function showNotification(text)
    {
	notificationBar.show(text);
    }

    function showNotificationTimeout(text, time)
    {
	notificationBar.showWithDelay(text,time);
    }

    function hideNotification() {
	notificationBar.hide();
    }




    function onListItemClicked(index, name)
    {
	listView.currentIndex = index

	if(isComplex(name))
	{
	    itemLoadingCompleteComplex(name, appIcon(name), daemon.getAppDescription(name));
	}
	else if(isDBus(name))
	{
	    itemLoadingCompleteDBus(name,appIcon(name),daemon.getAppDBus(name));
	}
	else if(isScript(name))
	{
	    itemLoadingCompleteScript(name,appIcon(name),daemon.getAppCMDLine(name));
	}
	else
	{
	    itemLoadingCompletePath(name,appIcon(name),daemon.getAppPath(name));
	}
    }

    function onLockButtonClicked(itemName)
    {
	if(isLocked(itemName))
	{
	    if(isComplex(itemName))
	    {
		console.log("Unlocking Complex App: " + itemName);
		daemon.RemoveComplex(itemName);
		showNotificationTimeout("Please apply changes manually", 2000);
		console.log("This App type requires the daemon to be restarted for changes to be applied");
		console.log("Restart it from the menu by clicking Apply Changes");
	    }
	    else if(isDBus(itemName))
	    {
		console.log("Unlocking DBus Service: " + itemName)
		daemon.RemoveDBus(itemName);
		showNotificationTimeout("Please apply changes manually",2000);
		console.log("This App type requires the daemon to be restarted for changes to be applied");
		console.log("Restart it from the menu by clicking Apply Changes");
	    }
	    else if(isScript(itemName))
	    {
		console.log("Unlocking Script: " + itemName);
		daemon.RemoveCMDLine(itemName);
	    }
	    else
	    {
		console.log("Unlocking App: " + itemName);
		daemon.RemoveApp(itemName);
	    }
	}
	else
	{
	    if(isComplex(itemName))
	    {
		console.log("Locking Complex App: " + itemName);
		daemon.AddComplex(itemName);
		showNotificationTimeout("Please apply changes manually",2000);
		console.log("This App type requires the daemon to be restarted for changes to be applied");
		console.log("Restart it from the menu by clicking Apply Changes");
	    }
	    else if(isScript(itemName))
	    {
		console.log("Locking Script: " + itemName);
		daemon.AddCMDLine(itemName);
	    }
	    else if(isDBus(itemName))
	    {
		console.log("Locing DBus App: " + itemName);
		daemon.AddDBus(itemName);
		showNotificationTimeout("Please apply changes manually",2000);
		console.log("This App type requires the daemon to be restarted for changes to be applied");
		console.log("Restart it from the menu by clicking Apply Changes");
	    }
	    else
	    {
		console.log("Locking App: " + itemName);
		daemon.AddApp(itemName, daemon.getAppPath(itemName));
	    }
	}

	hideItemProperties();
    }

    function menuButtonClicked()
    {
	if(menu.state == "Hidden")
	    menu.state = "Shown";
	else
	    menu.state = "Hidden";
    }

    function restartDaemonClicked()
    {
	menu.state = "Hidden"
	//showNotification("Restarting Daemon");
	daemon.ExitDaemon();
	daemonDialog.show();
    }

    function minimizeClicked()
    {
	daemon.ShowTaskSwitcher();
    }

    function exitClicked()
    {
	daemon.Exit();
    }


    function startDaemon()
    {
	showNotificationTimeout("Starting Daemon, please wait", 2000);
	menu.daemonStatus = "<b>Daemon Status:</b> " + getDaemonStatus();
    }




    function itemLoadingCompleteDBus(itemname, itemImagePath, itemdbus)
    {
	itemDetails.itemIcon = itemImagePath
	itemDetails.itemDescription = "This is a DBus Launched Application";
	itemDetails.itemPath = "<b>DBus Service:</b> " + itemdbus

	loadingComplete(itemname);
    }

    function itemLoadingCompleteComplex(itemname, itemImagePath, itemdbus)
    {
	itemDetails.itemIcon = itemImagePath
	itemDetails.itemDescription = "This is a Complex Application";
	itemDetails.itemPath = "<b>Description:</b> " + itemdbus

	loadingComplete(itemname);
    }

    function itemLoadingCompletePath(itemname, itemImagePath, itempath)
    {
	itemDetails.itemIcon = itemImagePath
	itemDetails.itemDescription = "This is a binary application";
	itemDetails.itemPath = "<b>Binary Path:</b> " + itempath

	loadingComplete(itemname);
    }

    function itemLoadingCompleteScript(itemname, itemImagePath, itempath)
    {
	itemDetails.itemIcon = itemImagePath
	itemDetails.itemDescription = "This is a script";
	itemDetails.itemPath = "<b>Script Path:</b> " + itempath

	loadingComplete(itemname);
    }

    function loadingComplete(itemname)
    {

	itemDetails.itemName = itemname

	if(isLocked(itemname))
	{
	    itemDetails.itemStatus = "This application is currently locked. Click <u>Unlock</u> to unlock it."
	    itemDetails.buttonText = "Unlock"

	}
	else
	{
	    itemDetails.itemStatus = "This application is currently unlocked. Click <u>Lock</u> to secure it."
	    itemDetails.buttonText = "Lock"
	}

	itemDetails.state = "Shown"
    }

    function updateList()
    {
	showNotification("Loading Desktop Files");
	listView.model = daemon.applist
	hideNotification()

	menu.daemonStatus = "<b>Daemon Status:</b> " + getDaemonStatus();
    }

    function unlockAll()
    {
	menu.state = "Hidden";
	showNotificationTimeout("This feature has not yet been implemented");
    }

    AppLock {
	onApplicationLoaded: showNotification("Loading")
	onApplicationLoadingComplete: updateList()
	onApplicationLocked: hideNotification()
	onApplicationUnlocked: hideNotification()
	onError:console.log("Error: " + message)
    }

    List {
	id: listView
	topOffset: header.height - 10
    }

    ItemDetails {
	id: itemDetails
    }

    Header {
	id: header
    }

    NotificationBar
    {
	id: notificationBar
	topOffset: header.height - 10
	height: 80
    }

    Menu {
	id: menu
    }

    RestartDaemon {
	id: daemonDialog
    }

    }
