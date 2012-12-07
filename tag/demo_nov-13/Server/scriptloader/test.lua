function onmessage ( input )
	output = test ( "hello " )
	return output
end

function onconnect ( input ) 
	output = "on connect"
	return output
end

function ondisconnect ( input ) 
	output = "on disconnect"
	return output
end