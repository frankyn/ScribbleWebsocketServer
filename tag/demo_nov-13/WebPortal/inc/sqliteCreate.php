<?php

	require_once  ("database.php");

	try
	{
		$DBH->exec( "CREATE TABLE users (userID INTEGER PRIMARY KEY, email TEXT, password TEXT, realName TEXT)" ) ;
		$DBH->exec( "CREATE TABLE apps (appID INTEGER PRIMARY KEY, appName TEXT, appChannel TEXT, appScript BLOB)" ) ;
		$DBH->exec( "CREATE TABLE assoc (assocID INTEGER PRIMARY KEY, userID INTEGER, appID INTEGER)" ) ;
		$password	=	md5( "abc123" ) ;
		$DBH->exec( "INSERT INTO users (email, password, realName), ('russellfolk@gmail.com', \"$password\", 'Russell');" ) ;
	}
	catch ( PDOException	$e )
	{
		echo	$e->getMessage	() ;
	}
?>