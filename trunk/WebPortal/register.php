<?php
//http://www.tutorialized.com/tutorial/Custom-an-Advanced-PHP-Membership-System/77094
	session_start	() ;
	require_once  ("inc/database.php");
	require_once  ("inc/class.User.php");
	$User	=	new	User	( $DBH ) ;
	require_once  ("inc/User.php");

	if	( isset	( $_POST[ 'submit' ] ) )
	{
		$data	=	$_POST ;
		$fail	=	0 ;
		if	( $data[ 'email' ]	==	"" )
		{
			echo	"Please enter your email!<br />" ;
			$fail	=	1 ;
		}

		if	( $data[ 'password1' ]	==	""	||	$data[ 'password2' ]	==	"" )
		{
			echo	"Please choose a password<br />" ;
			$fail	=	$fail + 2 ;
		}

		if	( $data[ 'password1' ]	!=	$data[ 'password2' ] )
		{
			if	( $fail	>=	2 )
				echo	"Passwords do not match! <br />" ;

			$fail++ ;
		}

		if	( $data[ 'realName' ]	==	"" )
		{
			echo	"Please enter your name!" ;
			$fail++ ;
		}
		else
		{
			//check to ensure email is not already taken
			//$sql		=	"SELECT email FROM users WHERE email=? LIMIT 1" ;
			//$query	=	$DBH->prepare	( $sql ) ;
			//$count	=	$query->execute	( array	( $data[ 'email' ] ) ) ;
			$sql			=	"SELECT email FROM users WHERE email='".$data[ 'email' ]."'? LIMIT 1" ;
			$count		=	$DBH->exec($sql) ;

			if	( $count > 0 )//$query->rowCount()	>	0 )
			{
				echo	"Email is already in use!<br />" ;
				$fail++ ;
			}
		}
	}

	if	( isset	( $fail ) )
	{
		if	( $fail	==	0 )
		{
			//Process the registration
			$sql		=	"INSERT INTO users (email, password, realName) VALUES (?, ?, ?)" ;
			$query	=	$DBH->prepare	( $sql ) ;
			$query->execute	( array	( $data[ 'email' ], md5	( $data[ 'password1' ] ), $data[ 'realName' ] ) ) ;
			if	( $query->rowCount	()	<	1 )
			{
				$err	=	$query->errorInfo	() ;
				echo	"Failed because: " . $err[ 2 ] ;
			}

			die	( "<script type='text/javascript'>window.location='index.php'; </script>" ) ;
		}
	}
	else
		$fail	=	1 ;

	if	( $fail	>=	1 )
	{
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
</head>
<body>

<article>

	<form action="<?php	echo	$_SERVER[ 'PHP_SELF' ];	?>" method="POST">
		E-mail Address:<span class="required">*</span><br />
		<input type="text" name="email" /><br />
		Password:<span class="required">*</span><br />
		<input type="password" name="password1" /><br />
		Re-Type Password:<span class="required">*</span><br />
		<input type="password" name="password2" /><br />
		Real Name:<span class="required">*</span><br />
		<input type="text" name="realName" /><br />
		<input type="submit" name="submit" value="Register!" />
	</form>

</article>

</body>
</html>

<?php
	}
?>