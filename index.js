var driver = require('bindings')('synth')
  , through = require('through')

module.exports = function () {
  var synth = new driver.synth()
    , stream = through(synth.send.bind(synth));

  stream.once('close', function () {
    delete synth;
  });

  return stream;
};