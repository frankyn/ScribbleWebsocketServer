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
	<header>
		<ul>
			<li><a href="logout.php">Logout</a></li>
		</ul>
	</header>
	<article>
		<table>
			<thead>
				<tr>
					<th colspan="4">Welcome <?php echo	$User->get_field	( "realName" ) ; ?></th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<th>Application</th>
					<th>Channel</th>
					<th>Actions</th>
				</tr>
<?php
	try
	{
		$userID	=	$_SESSION[ 'user' ][ 'userID' ] ;
		$sql		=	"SELECT * FROM apps INNER JOIN assoc ON 'apps'.'appID' = 'assoc'.'appID' WHERE 'assoc'.'userID' = '$userID'" ;
		$query	=	$DBH->prepare	( $sql ) ;
		$count	=	$query->execute	() ;
		$data		=	$query->fetchAll	() ;
		foreach ( $data as $row )
		{?>
				<tr>
					<td><?php echo $row['appName']; ?></td>
					<td><?php echo $row['appChannel']; ?></td>
					<td><a href="edit.php?id=<?php echo $row['appID']; ?>">Edit</a> <a onclick="javascript:if ( confirm ( 'Are you sure you want to delete this app?' )  ) { return true; } else { return false; }" href="delete.php?id=<?php echo $row['appID']; ?>">Delete</a></td>
				</tr>
<?php
		}
	}
	catch (PDOException $e){$e->getMessage();}
?>
			</tbody>
			<tfoot>
				<tr>
					<td colspan="4"><a href="add.php">Add an App</a></td>
				</tr>
			</table>
		</article>
	</body>
</head>