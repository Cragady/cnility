import express, { NextFunction, Request, Response } from 'express';
import { createRequire } from 'module';

import path from 'path';
import { hello, hello2 } from './extensions';


const DEBUG = false;

const app = express();
const port = 3000;

console.log(hello.hello());
console.log(hello2.hello());
console.log('setting static files');
express.urlencoded({extended: true});
app.use(express.json());
app.use(express.static(path.join(__dirname, './kandr')));

app.get('/', (req: Request, res: Response) => {
  res.send('Hello, DoopDerp! <3');
});

app.get('/kandr', (req: Request, res: Response, next: NextFunction) => {
  const fileRootPath = path.join(__dirname, './kandr');
  res.sendFile('kandr.html', { root: fileRootPath }, (err: Error) => {
    if (err) {
      next(err);
    } else {
      if (DEBUG) console.log('File sent');
    }
  });
});

app.listen(port, () => {
  // 🙉
  console.log(`🙈 ==> Server now listening to you... ON http://localhost:${port} 🌏!`);
});
