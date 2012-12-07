<?php
session_start	() ;
require_once  ( "inc/database.php" ) ;
require_once  ( "inc/class.User.php" ) ;
$User	=	new	User	( $DBH ) ;
require_once  ( "inc/User.php" ) ;

$userID	=	$_SESSION[ 'user' ][ 'userID' ] ;
$appID	=	$_REQUEST['id'] ;
if(isset($_POST['submit']))
{
	$appName = $_POST['appName'] ;
	$script	=	$_POST['app'] ;
	$sql		=	"UPDATE apps SET appName = ?, appScript = ? WHERE appID = '$appID'" ;
	$query = $DBH->prepare($sql);
	$query->execute(array ( $appName, $script ));
	header( "Location: index.php" ) ;
}
else
{
	$sql 				= "SELECT * FROM apps WHERE appID = '$appID'" ;
	$query 			= $DBH->prepare($sql) ;
	$query->execute() ;
	$result			=	$query->fetch() ;
	$appName		=	$result['appName'] ;
	$script			=	$result['appScript'] ;
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
			id : "myApp"		// textarea id
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
		<input type="text" name="appName" value="<?php echo $appName; ?>" /><br />
		Script:<br />
		<textarea cols="60" id="myApp" name="app" rows="20"><?php echo $script; ?></textarea><br />
		<input type="submit" name="submit" value="Edit!" />
	</form>

</article>

</body>
</html>