

usersTable = {}
function init ( ) 
	storeid ( "users", tostring(0) )
	users = 0
end

function onMessage ( uid, msg )
	broadcast ( uid, msg )
	return ""
end

function onConnect ( uid )
	table.insert ( usersTable, uid )
	users = tonumber(getid("users")) + 1
	broadcast ( "User connected: "..tostring(uid) )	
	broadcast ( "Current users - "..tostring(users) )
	storeid ( "users", tostring(users) )
	return ""
end

function onDisconnect ( uid )
	table.remove ( uid )
	users = tonumber(getid("users")) - 1
	storeid ( "users", tostring(users) )
	return ""
end

