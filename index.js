var driver = require('bindings')('synth')
  , through = require('through')

exports.synth = function synth () {
  return new driver.synth();
};

exports.stream = function stream () {
  var synth = exports.synth()
    , stream = through(synth.send.bind(synth));

  stream.once('close', function () {
    delete synth;
  });

  return stream;
};