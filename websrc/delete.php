<?php
	session_start	() ;
	require_once  ( "inc/database.php" ) ;
	require_once  ( "inc/class.User.php" ) ;
	$User	=	new	User	( $DBH ) ;
	require_once  ( "inc/User.php" ) ;

	$userID	=	$_SESSION[ 'user' ][ 'userID' ] ;
	$appID	=	$_REQUEST['id'] ;

	$sql		=	"DELETE FROM apps WHERE appID = '$appID'" ;
	$query = $DBH->prepare($sql);
	$query->execute();
	$sql		=	"DELETE FROM assoc WHERE appID = '$appID'" ;
	$query = $DBH->prepare($sql);
	$query->execute();
	header( "Location: index.php" ) ;

?>