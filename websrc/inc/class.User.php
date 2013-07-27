<?php

class User
{
	private	$DBH;				//Database Handler
	private	$userData; //Users information

	public	function	__construct	( PDO $DBH )
	{
		//$this->DBH	=	&$DBH ;
		$this->DBH	=	new	PDO	( "sqlite:inc/users.sqlite" ) ;
	}

	//check to see if user data is valid
	public	function	validate_user	( $email, $password )
	{
		$sql		=	"SELECT userID FROM users WHERE email=? AND password=?" ;
		$query	=	$this->DBH->prepare	( $sql ) ;
		$count	=	$query->execute	( array	( $email, $password ) ) ;

		if	( $count <= 0 )
			return false;

		return true;
	}

	public	function	login	( $email, $password )
	{

		if	( !$this->validate_user	( $email, $password ) )
		{
			$this->logout	() ;
			return false ;
		}

		//get the users data from MySQL
		$sql		=	"SELECT * FROM users WHERE email=? AND password=?" ;
		$query	=	$this->DBH->prepare	( $sql );
		$query->execute	( array	( $email, $password ) ) ;
		//$sql	=	"SELECT * FROM users WHERE email='".$email."' AND password='".$password."' LIMIT 1";
		//echo $sql;
		//$result	=	$this->DBH->query($sql);
		$result		=	$query->fetch	( PDO::FETCH_ASSOC ) ;
//		print_r($result);
//		echo "<br><br>$email $password<br><br>";
		//set up the sessions
		$_SESSION[ 'user' ]['email'] = $result['email'];
		$_SESSION['user']['password']= $result['password'];
		$_SESSION['user']['userID']=$result['userID'];
		$_SESSION['user']['realName']=$result['realName'];
//		print_r($_SESSION['user']);

		$_SESSION[ 'user' ][ 'loggedIn' ]	=	true ;

		//set a reference to the session for local use
		$this->userData	=	&$_SESSION[ 'user' ] ;

		return	true ;
	}

	public	function	logout ()
	{
		unset	( $_SESSION[ 'user' ] ) ;
		session_destroy() ;
		$this->no_user	() ;
		return	true ;
	}

	public	function	get_field( $fieldName )
	{
		if	( isset	( $this->userData[ $fieldName ] ) )
			return	$this->userData[ $fieldName ] ;

		//error if it does not exist
		die	( "Membership Error!" ) ;
	}

	public	function	no_user	()
	{
		if	( !isset	( $_SESSION[ 'user' ] ) )
		{
			if	( isset	( $_SESSION[ 'user' ] ) )
				unset	( $_SESSION[ 'user' ] ) ;

			$this->userData	=	&$_SESSION[ 'user' ] ;
		}
		else
			$this->userData	=	&$_SESSION[ 'user' ] ;
	}
}

?>