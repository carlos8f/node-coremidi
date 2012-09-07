coremidi
========

allow Node.js to interact with CoreMIDI services on Mac OS platforms

Usage
-----

`coremidi` exports a function which returns a stream. MIDI messages are sent by
writing an array to the stream.

### Simple example

Playing middle C:

```javascript
var coremidi = require('coremidi')();

coremidi.write([144, 60, 127]);
setTimeout(function() {
  coremidi.write([128, 60, 0]);
}, 2000);
```

### Example with [midi-api](https://github.com/carlos8f/node-midi-api)

```javascript
var coremidi = require('coremidi')
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
```

- - -

### Developed by [Terra Eclipse](http://www.terraeclipse.com)
Terra Eclipse, Inc. is a nationally recognized political technology and
strategy firm located in Aptos, CA and Washington, D.C.

- - -

### License: MIT

- Copyright (C) 2012 Carlos Rodriguez (http://s8f.org/)
- Copyright (C) 2012 Terra Eclipse, Inc. (http://www.terraeclipse.com/)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the &quot;Software&quot;), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED &quot;AS IS&quot;, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.