var api = require('midi-api')({end: true})
  , coremidi = require('../')

describe('basic test', function () {
  it('plays chords', function (done) {
    api
      .bank(2)
      .program(33)
      .rest(500)

    function maj7 (root) {
      api
        .noteOn(root)
        .noteOn(root + 4)
        .noteOn(root + 7)
        .noteOn(root + 11)
        .rest(200)
    }

    maj7(44)
    api.noteOff();
    maj7(45)
    api.noteOff();
    maj7(46)
    api.noteOff();
    maj7(47)
    api.rest(1000);
    api.noteOff();
    api.rest(500);

    api.pipe(coremidi.stream()).once('end', done);
  });
});