import { Router, Request, Response, NextFunction } from 'express';
import path from 'path';
import fs from 'fs';
// TODO:
import { fileConversion } from '../../../extensions';
import { badPngPhotoArr, nullTerminateString } from '../../../conversion';

const router = Router();

const KANDR_PATH = path.join(__dirname, '../../../kandr');
const PARSED_DIR = path.join(__dirname, '../../../../src/kandr/parsed');
// const TEST_PAGE_NAME = 'page10';
// const TEST_PAGE_NAME = 'page56';
const TEST_PAGE_NAME = 'page65';

router.route('/')
  .get(rootGet());

router.route('/log-kandr-files')
  .get(logKandrFiles(KANDR_PATH));

router.route('/longest')
  .get(logLongestKandrFile(KANDR_PATH));

router.route('/log-kandr-parsed-files')
  .get(logKandrFiles(PARSED_DIR));

router.route('/parsed-longest')
  .get(logLongestKandrFile(PARSED_DIR));

function rootGet() {
  return (req: Request, res: Response, next: NextFunction) => {
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
            if (file.name.toLowerCase() === 'kandr.css') {
              let fileBuffer = fs.readFileSync(fileName, 'utf-8');
              fileBuffer = fileBuffer.replace(/url\(/g, 'url(/parsed/');
              fs.writeFileSync(fileWrite, fileBuffer, { encoding: 'utf-8' });
            } else {
              const nameStub = file.name
                .replace('bg', '')
                .replace('.png', '');
              if (fileType.toLowerCase() === 'png') {
                const nameNum = parseInt(nameStub, 16);
                if (badPngPhotoArr.includes(nameNum)) return;
              }
              fs.copyFileSync(fileName, fileWrite);
            }
          } catch (err) {
            console.error('An error occurred: ', err);
          }
          return;
        }
        filesParsed++;
        // TODO: fix fonts and spacing - The conversion undoes a lot of heavy
        // lifting that the crazy ass font takes care of
        const fileNamePass = nullTerminateString(fileName);
        const fileWritePass = nullTerminateString(fileWrite);
        fileConversion.ParseFile(fileNamePass, fileWritePass);
        let fileBuffer = fs.readFileSync(fileWrite, 'utf-8');
        if (file.name.toLowerCase() === 'kandr.html') {
          try {
            // NOTE: This covers most files that are requested from client
            fileBuffer = fileBuffer
              .replace(/kandr\.page/g, '/parsed/kandr.page')
              .replace(/"stylesheet" href="/g, '"stylesheet" href="/parsed/')
              .replace(/src="/g, 'src="/parsed/');
          } catch (err) {
            console.error('An error occurred in replacing kandr.html file reqs: ', err);
          }
        } else {
          try {
            const fileNumString = file.name.replace(/[A-Za-z.]/g, '');
            const fileNum = parseInt(fileNumString);
            fileBuffer = fileBuffer
              .replace(/bg.*\.png/g, '/parsed/$&');

            if (badPngPhotoArr.includes(fileNum)) {
              fileBuffer = fileBuffer
                .replace(/<img.*?\/>/g, '');
            }
          } catch (err: any) {
            console.error('An error occurred in replacing bg photos: ', err);
          }
        }
        fs.writeFileSync(fileWrite, fileBuffer, { encoding: 'utf-8' });
      });
      console.log(`${filesParsed} files parsed.`);
      console.log(`Parsing finished. New files placed in: ${PARSED_DIR}`);

      res.json({ status: 'Data Read: Ok' }).status(200);
    } catch (err: any) {
      console.error(err);
      // next(err);
      res.status(500).json({ status: 500, message: err.message });
    }
  }
}

function logKandrFiles(path: string) {
  return (req: Request, res: Response, next: NextFunction) => {
    try {
      const files = fs.readdirSync(path, { withFileTypes: true });
      let filesParsed = 0;
      files.forEach((file: fs.Dirent<string>) => {
        if (file.isDirectory()) return;
        // const fileName = path.join(file.parentPath, file.name);
        console.log(file.name);
        // console.log(fileName);
      });

      res.json({ status: 'Data Read: Ok' }).status(200);
    } catch (err: any) {
      console.error(err);
      // next(err);
      res.status(500).json({ status: 500, message: err.message });
    }
  }
}

function logLongestKandrFile(path: string) {
  return (req: Request, res: Response, next: NextFunction) => {
    try {
      const files = fs.readdirSync(path, { withFileTypes: true });
      let longestName = "";
      let longestArr: string[] = [];
      files.forEach((file: fs.Dirent<string>) => {
        if (file.isDirectory()) return;
        const fileParts = file.name.split('.');
        const fileType = fileParts[fileParts.length - 1];
        if (fileType.toLowerCase() !== 'png') return;
        if (file.name.charAt(0).toLowerCase() !== 'b') return;
        if (file.name.length >= longestName.length) {
          if (file.name.length > longestName.length) longestArr = [];
          longestName = file.name;
          longestArr.push(file.name);
        }
      });
      console.log(`Longest char count: ${longestName.length} | Longest name: ${longestName}`);
      res.json({ status: 'Data Read: Ok', data: { length: longestName.length, name: longestName, longestArr } });
    } catch (err: any) {
      console.error(err);
      res.status(500).json({ status: 500, message: err.message });
    }
  }
}

export default router;
