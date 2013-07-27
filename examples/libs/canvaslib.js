var Canvas_FN = function ( width, height ) {
	this.id = 'real_ctx';
	this.bid = 'buffer_ctx';
	this.width = width;
	this.height = height;

	this.canvas_elem = document.createElement('canvas');
	this.canvas_elem.id = this.id;
	this.canvas_elem.height = height;
	this.canvas_elem.width = width;

	this.buffer_elem = document.createElement('canvas');
	this.buffer_elem.id = this.id;
	this.buffer_elem.height = height;
	this.buffer_elem.width = width;
	this.buffer_elem.visible = false;
	document.body.appendChild(this.canvas_elem);
	    
	this.canvas_ctx = this.canvas_elem.getContext("2d");
	this.buffer_ctx = this.buffer_elem.getContext("2d");

}

Canvas_FN.prototype.error = function ( MSG ) {
	console.error ( MSG );	
}

Canvas_FN.prototype.log = function ( MSG ) {
	console.log ( MSG );
}

Canvas_FN.prototype.resize = function ( width, height ) {
	this.width = width;
	this.height = height;
	$ ( "#" + this.id ).width ( this.width );
	$ ( "#" + this.id ).height ( this.height );

	$ ( "#" + this.bid ).width ( this.width );
	$ ( "#" + this.bid ).height ( this.height);	
}

Canvas_FN.prototype.ctx = function ( ) {
	return this.buffer_ctx;
}

Canvas_FN.prototype.swap = function ( ) {
	var img = this.buffer_ctx.getImageData(0, 0, this.width, this.height);
	this.canvas_ctx.putImageData(img, 0, 0);
	this.buffer_ctx.clearRect (0, 0, this.width, this.height);
}