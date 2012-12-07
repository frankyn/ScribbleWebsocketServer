editAreaLoader.load_syntax["lua"] = {
	'COMMENT_SINGLE' : ['--']
	,'COMMENT_MULTI' : {'--[[' : ']]'}
	,'QUOTEMARKS' : {1: "'", 2: '"'}
	,'KEYWORD_CASE_SENSITIVE' : true
	,'KEYWORDS' : {
		'logic' : [ 'break','do','else','elseif','end','for','function','if','in','repeat','return','then','until','while' ],
		'operators' : [ 'and','not','or' ],
		'misc' : [ 'false','local','nil','true' ]
	}
	,'OPERATORS' :[
	'+','-','*','/','%','^','=','==','~=','<','>','<=','>='
	]
	,'DELIMITERS' :[

	]
	,'STYLES' : {
		'COMMENTS': 'color: #AAAAAA;'
		,'QUOTESMARKS': 'color: #6381F8;'
		,'KEYWORDS' : {
			'logic': 'color: #0000FF;',
			'operators': 'color: #990000;',
			'misc': 'color: #009900;'
			}
		,'OPERATORS' : 'color: #E775F0;'
		,'DELIMITERS' : ''
	}
};