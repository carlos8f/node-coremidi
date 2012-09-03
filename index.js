var driver = require('bindings')('synth')
  , es = require('event-stream')

exports.synth = function synth () {
  return new driver.synth();
};

exports.stream = function stream () {
  var synth = exports.synth()
    , stream = es.through(synth.send.bind(synth));

  stream.once('close', function () {
    delete synth;
  });

  return stream;
};