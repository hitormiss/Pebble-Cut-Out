// DEPENDENCY PEBBLE-CLAY
var Clay = require('pebble-clay');
var clayConfig = require('./clay-config');
var clay = new Clay(clayConfig);

//Pebble.addEventListener('webviewclosed', function(e) {
//	console.log(JSON.stringify(e));
//});
/////////////////////////////////////


var options = JSON.parse(localStorage.getItem('options'));
if (options === null) 
  options = { "URL" : "http://matthewkingston.com/pebble/background_polka.png"};
//options = { "URL" : "http://matthewkingston.com/pebble/background_polka.png';"}; 

var MAX_CHUNK_SIZE = 1500; 

function sendChunk(array, index, arrayLength) {
	// Determine the next chunk size
	var chunkSize;
	if(arrayLength - index < MAX_CHUNK_SIZE) {
		// Will only need one more chunk
		chunkSize = arrayLength - index;
	} 
	else {
		// Will require multiple chunks for remaining data
		chunkSize = MAX_CHUNK_SIZE;
	}

	// Prepare the dictionary
	var dict = {
		'DL_DataChunk': array.slice(index, index + chunkSize),
		'DL_ChunkSize': chunkSize,
		'DL_Index': index
	};

	// Send the chunk
	Pebble.sendAppMessage(dict, function() {
		// Success
		index += chunkSize;
		if(index < arrayLength) {
			// Send the next chunk
			sendChunk(array, index, arrayLength);
			console.log("chunk: "+ index);
		}
		else {
			// Complete!
			Pebble.sendAppMessage({'DL_Complete': 0});
		}
	}, function(e) {
		console.log('Failed to send chunk with index ' + index);
	});
}

function transmitImage(array) {
	var index = 0;
	var arrayLength = array.length;
	
	// Transmit the length for array allocation
	Pebble.sendAppMessage({'DL_DataLength': arrayLength}, function(e) {
		// Success, begin sending chunks
		sendChunk(array, index, arrayLength);
	}, function(e) {
		console.log('Failed to send data length to Pebble!');
	});
}

function processImage(responseData) {
	// Convert to a array
	var byteArray = new Uint8Array(responseData);
	var array = [];
	for(var i = 0; i < byteArray.byteLength; i++) {
		array.push(byteArray[i]);
	}

	// Send chunks to Pebble
	transmitImage(array);
}

function downloadImage(url) {
	console.log("download image: "+ url);
	var request = new XMLHttpRequest();
	request.onload = function() {
		processImage(this.response);
	};
	request.responseType = "arraybuffer";
	request.open("GET", url);
	request.send();
}

Pebble.addEventListener("ready", function(e) {
  console.log("Ready to go!");  
	
	if (options.IMAGE) {
		if (options.IMAGE.value) {
			if (options.IMAGE.value == 10) downloadImage(options.URL.value);
		}
	}
	
});

Pebble.addEventListener("appmessage", function(e) {
	console.log(JSON.stringify(e));
	//console.log("init download of image");
	//downloadImage(options.URL);
	//downloadImage("http://matthewkingston.com/pebble/background_polka.png");
});

Pebble.addEventListener('webviewclosed', function(e) {
	//console.log(JSON.stringify(e));
	if (e.response) {
	  options = JSON.parse(decodeURIComponent(e.response));
    localStorage.setItem('options', JSON.stringify(options));
		//console.log(JSON.stringify(options));
		//console.log(JSON.stringify(options.URL));
		//console.log(options.URL);
		console.log(options.URL.value);
  } 
	if (options.IMAGE) {
		if (options.IMAGE.value) {
			if (options.IMAGE.value == 10) {
				console.log("init download of image");
				downloadImage(options.URL.value);
			}
		}
	}
});
