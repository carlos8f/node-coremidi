var api = require('midi-api')
  , coremidi = require('../')

describe('basic test', function () {
  it('plays a note', function (done) {
    api({end: true})
      .bank(0)
      .program(0)
      .noteOn(60)
      .rest(2000)
      .noteOff()
      .pipe(coremidi.stream())
      .once('end', done);
  });
});