/* @ts-expect-error */
const hello = require('./hello/hello.node');
/* @ts-expect-error */
const hello2 = require('./hello2/hello2.node');
/* @ts-expect-error */
const fileConversion = require('./file_conversion/file_conversion.node');

module.exports = {
  hello,
  hello2,
  fileConversion,
}
