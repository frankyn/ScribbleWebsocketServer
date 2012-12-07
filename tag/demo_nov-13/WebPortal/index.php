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
?>

<form action="<?php	$_SERVER[ 'PHP_SELF' ];	?>" method="POST">
	Email:<br />
	<input type="text" name="email" /><br />
	Password:<br />
	<input type="password" name="password" /><br />
	<input type="submit" name="submit" value="Login!" />
</form>

<?php
	}
	else
	{
		echo	"<table>".
"	<thead>".
"		<tr>".
"			<th colspan=\"4\">Welcome " ;
		echo	$User->get_field	( "realName" ) ;
		echo	"</th>".
"		</tr>".
"	</thead>".
"	<tbody>".
"		<tr>".
"			<th>Application</th>".
"			<th>Channel</th>".
"			<th>Control Script</th>".
"			<th>Actions</th>".
"		</tr>" ;

		try{
		$userID	=	$_SESSION[ 'user' ][ 'userID' ] ;
		$sql		=	"SELECT * FROM apps INNER JOIN assoc ON assoc.appID = apps.appID WHERE assoc.userID = $userID" ;
		$query	=	$DBH->prepare	( $sql ) ;
		$count	=	$query->execute	() ;
		$data		=	$query->fetchAll	() ;
		foreach ( $data as $row )
		{
			echo	"		<tr>".
"			<td>".$row['appName']."</td>".
"			<td>".$row['appChannel']."</td>".
"			<td>".$row['appScript']."</td>".
"			<td><a href=\"edit.php?id=".$row['appID']."\">Edit</a> <a href=\"delete.php?id=".$row['appID']."\">Delete</a></td>".
"		</tr>" ;
		}
		}catch (PDOException $e){$e->getMessage();}
		echo	"	</tbody>".
"	<tfoot>".
"		<tr>".
"			<td colspan=\"4\"><a href=\"add.php\">Add an App</a></td>".
"		</tr>".
"</table>" ;
//$User->logout();
	}
?>
