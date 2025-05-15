import { hello, hello2 } from '../extensions';

function testExtensions() {
  console.log(hello.hello());
  console.log(hello2.hello());
}

function rootDir() {
  return process.env.ROOT_DIR || '';
}

function debug() {
  return process.env.DEBUG === 'true';
}

function getContext() {
  return {
    ROOT_DIR: rootDir(),
    DEBUG: debug(),
  }
}

export {
  testExtensions,
  getContext,
  rootDir,
  debug,
}
