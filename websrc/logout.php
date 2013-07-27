<?php
	session_start	() ;
	require_once  ( "inc/database.php" ) ;
	require_once  ( "inc/class.User.php" ) ;
	require_once  ( "inc/User.php" ) ;
	$User	=	new	User	( $DBH ) ;
	$User->logout	() ;
	header ("Location: index.php") ;
?>