import { Router, Request, Response, NextFunction } from 'express';
import path from 'path';
import fs from 'fs';
// TODO:
import { fileConversion } from '../../../extensions';
import { nullTerminateString } from '../../../conversion';

const router = Router();

const KANDR_PATH = path.join(__dirname, '../../../kandr');
const PARSED_DIR = path.join(__dirname, '../../../../src/kandr/parsed');
// const TEST_PAGE_NAME = 'page10';
// const TEST_PAGE_NAME = 'page56';
const TEST_PAGE_NAME = 'page65';

router.route('/')
  .get((req: Request, res: Response, next: NextFunction) => {
    try {
      const files = fs.readdirSync(KANDR_PATH, { withFileTypes: true });
      let filesParsed = 0;
      files.forEach((file: fs.Dirent<string>) => {
        if (file.isDirectory()) return;
        const fileParts = file.name.split('.');
        const fileType = fileParts[fileParts.length - 1];
        // TODO: parse file with native node
        const fileName = path.join(file.parentPath, file.name);
        const fileWrite = path.join(PARSED_DIR, file.name);
        if (fileType.toLowerCase() !== 'html') {
          try {
            fs.copyFileSync(fileName, fileWrite);
          } catch (err) {
            console.error('An error occurred: ', err);
          }
          return;
        }
        filesParsed++;
        // TODO: find out why nodemon gets overexcited over file parsing (may
        // be wanted behavior)
        // TODO: fix fonts and spacing - The conversion undoes a lot of heavy
        // lifting that the crazy ass font takes care of
        // TODO: get rid of conditional
        // if (fileParts[1] === TEST_PAGE_NAME) {
          const fileNamePass = nullTerminateString(fileName);
          const fileWritePass = nullTerminateString(fileWrite);
          fileConversion.ParseFile(fileNamePass, fileWritePass);
        // }
        if (file.name.toLowerCase() === 'kandr.html') {
          try {
            let fileBuffer = fs.readFileSync(fileWrite, 'utf-8');
            // TODO: find more elegant way of changing asset requests in HTML
            // maybe incorporate this into the fileConversion process?
            fileBuffer = fileBuffer
              .replace(/kandr\.page/g, '/parsed/kandr.page')
              .replace(/"stylesheet" href="/g, '"stylesheet" href="/')
              .replace(/src="/g, 'src="/');
            fs.writeFileSync(fileWrite, fileBuffer, { encoding: 'utf-8' });
          } catch (err) {
            console.error('An error occurred: ', err);
          }
        }
      });
      console.log(`${filesParsed} files parsed.`);
      console.log(`Parsing finished. New files placed in: ${PARSED_DIR}`);

      res.json({status: 'Data Read: Ok'}).status(200);
    } catch (err: any) {
      console.error(err);
      // next(err);
      res.status(500).json({status: 500, message: err.message });
    }
  });

export default router;
