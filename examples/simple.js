var coremidi = require('../')();

coremidi.write([144, 60, 127]);
setTimeout(function() {
  coremidi.write([128, 60, 0]);
}, 2000);
