var coremidi = require('../')
  , midi = require('midi-api')()

midi
  .bank(2)
  .program(4)
  .rest(500)

function maj7 (root) {
  midi
    .noteOn(root)
    .noteOn(root + 4)
    .noteOn(root + 7)
    .noteOn(root + 11)
    .rest(1000)
    .noteOff()
}

maj7(60)
maj7(61)
maj7(62)
maj7(63)

midi.pipe(coremidi());
