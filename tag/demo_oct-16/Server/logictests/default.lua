runtimeVar = 0

function init ( ) 

end

function onmessage ( msg ) 
	return msg
end

function onconnect ( descriptor )
	runtimeVar = runtimeVar + 1
	return tostring ( runtimeVar )
end

function ondisconnect ( descriptor )
	runtimeVar = runtimeVar - 1
	return "BYE"
end
