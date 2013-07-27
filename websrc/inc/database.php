<?php
//setup sqlite

	try
	{
		$DBH	=	new	PDO	( "sqlite:inc/users.sqlite" ) ;
	}
	catch	( PDOException	$e )
	{
		echo	$e->getMessage	() ;
	}

?>