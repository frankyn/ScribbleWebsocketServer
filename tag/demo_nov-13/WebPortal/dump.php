<?php
	require_once  ( "inc/database.php" ) ;
	$result = $DBH->query	( "SELECT * FROM users" ) ;

echo	"<table><tbody>" ;

	foreach	( $result as $row )
	{
		echo	"<tr><td>".$row['userID']."</td><td>".$row['email']."</td><td>".$row['password']."</td><td>".$row['realName']."</td></tr>" ;
	}
echo	"</tbody></table>" ;
$result = $DBH->query	( "SELECT * FROM apps" ) ;

echo	"<table><tbody>" ;

foreach	( $result as $row )
{
	echo	"<tr><td>".$row['appID']."</td><td>".$row['appName']."</td><td>".$row['appChannel']."</td><td>".$row['appScript']."</td></tr>" ;
}
echo	"</tbody></table>" ;
$result = $DBH->query	( "SELECT * FROM assoc" ) ;

echo	"<table><tbody>" ;

foreach	( $result as $row )
{
	echo	"<tr><td>".$row['assocID']."</td><td>".$row['appID']."</td><td>".$row['userID']."</td><td>".$row['realName']."</td></tr>" ;
}
echo	"</tbody></table>" ;

?>