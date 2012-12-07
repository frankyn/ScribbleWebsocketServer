var Sprite_FN = function ( ) {
	this.x = 0;
	this.y = 0;	
}

Sprite_FN.prototype.error = function ( MSG ) {
	console.error ( "Sprite: " + MSG );
}

Sprite_FN.prototype.log = function ( MSG ) {
	console.log ( "Sprite: " + MSG );
}

Sprite_FN.prototype.setPosition = function ( x, y ) {
	this.x = x;
	this.y = y;	
	this.x0 = x;
	this.y0 = y;
}

Sprite_FN.prototype.dist = function ( s ) {
	return Math.sqrt( Math.pow ( s.x - this.x, 2 ) + Math.pow ( s.y - this.y, 2) );	
}

Sprite_FN.prototype.getX = function ( ) {
	return this.x;	
}

Sprite_FN.prototype.getY = function ( ) {
	return this.y;	
}

Sprite_FN.prototype.update = function ( dt ) { }
Sprite_FN.prototype.draw = function ( ctx ) { }