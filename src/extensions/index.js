/* @ts-expect-error */
const hello = require('./hello/hello.node');
/* @ts-expect-error */
const hello2 = require('./hello2/hello2.node');

module.exports = {
  hello,
  hello2,
}
