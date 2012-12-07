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
		$channel = preg_replace( "/[^a-zA-Z0-9\s]/", "", $appName ) ;
		$script	=	$_POST['app'] ;
		echo "I bet this works...";
		$sql	=	"INSERT INTO apps (appName, appChannel, appScript) VALUES  ('$appName', '$channel', '$script');";
		$query = $DBH->prepare($sql);
		$query->execute();echo "I don't think this does";
		print_r($query->errorInfo());
		$appID	=	$query->rowCount();
		echo "<br>$appID<br>";
		$sql	=	"INSERT INTO assoc (userID, appID) VALUES ($userID, $appID);";
		$query = $DBH->prepare($sql);
		$query->execute();
		echo	"Did stuff" ;die();
	}


?>

<form action="<?php	$_SERVER[ 'PHP_SELF' ];	?>" method="POST">
	App Name:<br />
	<input type="text" name="appName" /><br />
	Script:<br />
	<textarea name="app"></textarea><br />
	<input type="submit" name="submit" value="Add!" />
</form>
