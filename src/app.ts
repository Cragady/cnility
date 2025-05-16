import express from 'express';

import path from 'path';
import routes from './routes';

process.env.DEBUG = 'false';
process.env.ROOT_DIR = __dirname;

const app = express();
const port = 3000;

express.urlencoded({ limit: '200mb', extended: true });
app.use(express.json({ limit: '200mb' }));
app.use(express.static(path.join(__dirname, './kandr')));
app.use(express.static(path.join(__dirname, './static')));

// API & View
app.use(routes);

app.listen(port, () => {
  // 🙉
  console.log(`🙈 ==> Server now listening to you... ON http://localhost:${port} 🌏!`);
});
