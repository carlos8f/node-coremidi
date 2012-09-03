var midi = require('midi-api')({end: true})
  , coremidi = require('../')

describe('basic test', function () {
  it('plays chords', function (done) {
    midi
      .bank(2)
      .program(33)
      .rest(500)

    function maj7 (root) {
      midi
        .noteOn(root)
        .noteOn(root + 4)
        .noteOn(root + 7)
        .noteOn(root + 11)
        .rest(200)
    }

    maj7(44)
    midi.noteOff();
    maj7(45)
    midi.noteOff();
    maj7(46)
    midi.noteOff();
    maj7(47)
    midi.rest(1000);
    midi.noteOff();
    midi.rest(500);

    midi.pipe(coremidi.stream()).once('end', done);
  });
});