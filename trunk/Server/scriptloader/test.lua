global_test_var = ""
function init ( ) 

end

function onBeat ( ) 

end

function onMessage ( desc, input ) 
	global_test_var = "ONMESSAGE"
	t = {}
	t.name = "hello"
	test ( desc )
	test ( json.encode ( t ) )
end

function onConnect ( input ) 
	output = "on connect"
end

function onDisconnect ( input ) 
	output = "on disconnect"
end
