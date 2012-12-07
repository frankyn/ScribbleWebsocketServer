users = 0
function init ( ) 

end

function onmessage ( msg )
	
	return ""
end

function onconnect ( uid )
	users = users + 1
	storeid ( "users", tostring(users) )
	broadcast ( "User connected" )	
	broadcast ( "Current users - "..tostring(users) )
	
	return ""
end

function ondisconnect ( uid )
	users = users - 1
	storeid ( "users", tostring(users) )
	broadcast ( "User disconnected" )
	broadcast ( "Current users - "..tostring(users) )
	
	return ""
end

