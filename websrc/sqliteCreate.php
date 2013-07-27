<?php

	require_once  ("inc/database.php");

	try
	{
		$DBH->exec( "CREATE TABLE users (userID INTEGER PRIMARY KEY, email TEXT, password TEXT, realName TEXT)" ) ;
		$DBH->exec( "CREATE TABLE apps (appID INTEGER PRIMARY KEY, appName TEXT, appChannel TEXT, appScript BLOB)" ) ;
		$DBH->exec( "CREATE TABLE assoc (assocID INTEGER PRIMARY KEY, userID INTEGER, appID INTEGER)" ) ;
	}
	catch ( PDOException	$e )
	{
		echo	$e->getMessage	() ;
	}
?>