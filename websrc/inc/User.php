<?php
	session_start	() ;
	require_once  ( "inc/database.php" ) ;
	require_once  ( "inc/class.User.php" ) ;
	$User	=	new	User	( $DBH ) ;
	require_once  ( "inc/User.php" ) ;

// Main user handler

	if	( !isset	( $_SESSION[ 'user' ][ 'loggedIn' ] )	or	$_SESSION[ 'user' ][ 'loggedIn' ]	==	false )
		$User->no_user	() ;	//	user is not logged in

	else
		$User->login	( $_SESSION[ 'user' ][ 'email' ], $_SESSION[ 'user' ][ 'password' ] ) ;

//test for logout
	if	( isset	( $_GET[ 'logout' ] )	&&	$_SESSION[ 'user' ][ 'loggedIn' ] )
	{
		$User->logout	() ;

		//forward the user away.
		die	( "
	    <script type='text/javascript'>
		    window.location='index.php';
	    </script>
    " ) ;
	}
?>