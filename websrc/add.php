<?php
	session_start	() ;
	require_once  ( "inc/database.php" ) ;
	require_once  ( "inc/class.User.php" ) ;
	$User	=	new	User	( $DBH ) ;
	require_once  ( "inc/User.php" ) ;

	if(isset($_POST['submit']))
	{
		$userID	=	$_SESSION[ 'user' ][ 'userID' ] ;
		$appName = $_POST['appName'] ;
		$channel = preg_replace( "/[^a-zA-Z0-9]/", "", $appName ) ;
		$channel = date("U").$channel ;
		$script	=	$_POST['app'] ;
		$sql	=	"INSERT INTO apps (appName, appChannel, appScript) VALUES  ('$appName', '$channel', '$script');";
		$query = $DBH->prepare($sql);
		$query->execute();
		$query = $DBH->prepare( "SELECT appID FROM apps ORDER BY appID DESC LIMIT 1" ) ;
		$query->execute() ;
		$app = $query->fetch() ;
		$appID = $app['appID'] ;
		$sql	=	"INSERT INTO assoc (userID, appID) VALUES ($userID, $appID);";
		$query = $DBH->prepare($sql);
		$query->execute();
		header( "Location: index.php" ) ;
	}


?>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en-us" />
<head>
	<meta charset="utf-8" />
	<title>Scribles Web Portal Interface</title>
	<link href="css/style.css" media="screen" rel="stylesheet" type="text/css" />
	<meta name="description" content="Web interface for Scribbles." />
	<meta name="viewport" content="width=device-width" />
	<script src="js/html5.js"></script>
	<script language="javascript" type="text/javascript" src="inc/editarea/edit_area/edit_area_full.js"></script>
	<script language="javascript" type="text/javascript">
		editAreaLoader.init({
			id : "newApp"		// textarea id
			,syntax: "lua"			// syntax to be uses for highgliting
			,start_highlight: true		// to display with highlight mode on start-up
		});
	</script>
</head>
<body>
<header>
	<ul>
		<li><a href="logout.php">Logout</a></li>
	</ul>
</header>
<article>

	<form action="<?php	$_SERVER[ 'PHP_SELF' ];	?>" method="POST">
		App Name:<br />
		<input type="text" name="appName" /><br />
		Script:<br />
		<textarea cols="60" id="newApp" name="app" rows="20"></textarea><br />
		<input type="submit" name="submit" value="Add!" />
	</form>

</article>

</body>
</html>