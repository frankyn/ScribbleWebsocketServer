<?php
	session_start	() ;
	require_once  ( "inc/database.php" ) ;
	require_once  ( "inc/class.User.php" ) ;
	$User	=	new	User	( $DBH ) ;
	require_once  ( "inc/User.php" ) ;

	if(isset($_POST['submit']))
	{
		//User is loggin in
		$data	=	$_POST ;
		if	( $data[ 'email' ]	==	""	||	$data[ 'password' ]	==	"" )
			die	( "No email or password.<br />Please <a href=\"{$_SERVER['PHP_SELF']}\">try again</a>" ) ;

	if	( !$User->login	( $data[ 'email' ], md5	( $data[ 'password' ] ) ) )
		die	( "Invalid email or password.<br />Please <a href=\"{$_SERVER['PHP_SELF']}\">try again</a>" ) ;

	else
		die	( "<script type='text/javascript'>window.location='index.php'; </script>" ) ;
	}

	if	( !$_SESSION[ 'user' ][ 'loggedIn' ] )
	{
		include	( "login.html" ) ;
	}
	else
	{
		include  ("myApps.php");
	}
?>
