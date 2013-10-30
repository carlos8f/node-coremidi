var coremidi = require('../')
  , midi = require('midi-api')()

midi
  .bank(2)
  .program(18)
  .rest(500)

//define some modes
var modes = {
  'ionian'        : [1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1],
  'dorian'        : [1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0],
  'phrygian'      : [1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0],
  'lydian'        : [1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1],
  'mixolydian'    : [1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1],
  'aeolian'       : [1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0],
  'locrian'       : [1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0]
}

var zeroOctaveNoteMap = {
	c 		: 12,
	'c#' 	: 13,		//some people are accustomed to using 'sharp' instead of 'flat'
  db 		: 13,
  d 		: 14,
  'd#' 	: 15,
  eb 		: 15,
  e 		: 16,
  f 		: 17,
  'f#' 	: 18,
  gb 		: 18,
  g 		: 19,
  'g#' 	: 20,
  ab 		: 20,
  a 		: 21,
  'a#' 	: 22,
  bb 		: 22,
  b 		: 23
}


function getNoteNumber(note) {
	//either note is a single c or d or e OR 
	//its flattened/sharpened like c# or db (b is for flat) OR
	//its with a octave like c2, d5, e3.
	//if there's a number at the end, it denotes the octave
	if(note.length == 1) {
		//just return from the zeroOctaveNoteMap noteMap assuming its a middle octave
		return zeroOctaveNoteMap[note] + (12 * 4);
	} else {
		//maybe its a flat or sharp note or its got an octave
		//try to convert the last index of the string to a number
		var octave = +note.substring(note.length-1);
		if(typeof octave === NaN) {
			//this is a flat or sharp note, assume its middle octave
			return zeroOctaveNoteMap[note] + (12 * 4);
		} else {
			//the note is specfied with an octave
			var theNote = note.substring(0, note.length-1);
			//calculate the number from this info
			return zeroOctaveNoteMap[theNote] + ((octave+1)*12)
		}
	}
}

function playMode (root, mode, intrvl) {
	//defaults
	root = getNoteNumber(root) || 60;
	mode = mode || 'ionian';
	intrvl = intrvl || 500;

	//generate 12 notes from root
	var chromaticNotes = [];
	for(var i=root; i<root+12; i++) chromaticNotes.push(i);
	
	//filter through the notes and get the required mode
	var melodyMode = [];
	for (var i=0; i<chromaticNotes.length; i++) {
		if(modes[mode][i]) melodyMode.push(chromaticNotes[i]);
	}

	console.log(melodyMode);

	//now we have the melody, lets play it (we could ve played it in the loop as well)
	for (var i=0; i<melodyMode.length; i++) {
		midi
    .noteOn(melodyMode[i])
    .rest(intrvl)
    .noteOff()
	}

	//since this is a mode, lets add the next octave of the root as well
	midi
		.noteOn(root + 12)
		.rest(intrvl)
    .noteOff()

	midi.pipe(coremidi());
	
}

playMode('c3', 'aeolian', 200);