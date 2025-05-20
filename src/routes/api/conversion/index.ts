import { Router, Request, Response, NextFunction } from 'express';
import path from 'path';
import fs from 'fs';
// TODO:
import { fileConversion } from '../../../extensions';
import { nullTerminateString } from '../../../conversion';

const router = Router();

const KANDR_PATH = path.join(__dirname, '../../../kandr');
const PARSED_DIR = path.join(__dirname, '../../../../src/kandr-parsed');
const TEST_PAGE_NAME = 'page10';

router.route('/')
  .get((req: Request, res: Response, next: NextFunction) => {
    try {
      const files = fs.readdirSync(KANDR_PATH, { withFileTypes: true });
      let filesParsed = 0;
      files.forEach((file: fs.Dirent<string>) => {
        const fileParts = file.name.split('.');
        const fileType = fileParts[fileParts.length - 1];
        if (fileType.toLowerCase() !== 'html') return;
        filesParsed++;
        // TODO: get rid of conditional
        if (fileParts[1] === TEST_PAGE_NAME) {
          // TODO: parse file with native node
          const fileName = nullTerminateString(path.join(file.parentPath, file.name));
          const fileWrite = nullTerminateString(path.join(PARSED_DIR, file.name));
          fileConversion.ParseFile(fileName, fileWrite);
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
