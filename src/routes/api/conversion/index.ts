import { Router, Request, Response, NextFunction } from 'express';
import path from 'path';
import fs from 'fs';
// TODO:
import { fileConversion } from '../../../extensions';
import { badPngPhotoArr, nullTerminateString } from '../../../conversion';

const router = Router();

const KANDR_PATH = path.join(__dirname, '../../../kandr');
const PARSED_DIR = path.join(__dirname, '../../../../src/kandr/parsed');
const FIXED_FONTS_DIR = path.join(__dirname, '../../../../src/fixed_fonts');
// const TEST_PAGE_NAME = 'page10';
// const TEST_PAGE_NAME = 'page56';
const TEST_PAGE_NAME = 'page65';

const F2_WOFF = 'f2.woff';
const F3_WOFF = 'f3.woff'; // NOTE: this font also has one glyph that is in the private use area as a kerning glyph

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

router.route('/parse-woffs')
  .get(parseWoffs());

function rootGet() {
  return (req: Request, res: Response, next: NextFunction) => {
    try {
      copyFixedFonts();
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

              // TODO: implement replacement(s) for ff2 corrected fonts
              const FF12_CSS_STR = '@font-face{font-family:ff12;src:url(/parsed/f12.woff)format("woff");}.ff12{font-family:ff12;line-height:0.666000;font-style:normal;font-weight:normal;visibility:visible;}';
              const FF3_LIGMA_CSS_STR = '@font-face{font-family:ff3_ligma;src:url(/parsed/f3_ligma.woff)format("woff");}.ff3_ligma{font-family:ff3_ligma;line-height:0.931000;font-style:normal;font-weight:normal;visibility:visible;}';
              let replacementString = '';

              replacementString = FF12_CSS_STR + '\n' + FF3_LIGMA_CSS_STR;

              fileBuffer = fileBuffer
                .replace(FF12_CSS_STR, replacementString);

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
            // NOTE: ff2 and ff3 are safe to replace
            const fileNumString = file.name.replace(/[A-Za-z.]/g, '');
            const fileNum = parseInt(fileNumString);
            fileBuffer = fileBuffer
              .replace(/bg.*\.png/g, '/parsed/$&');

            if (badPngPhotoArr.includes(fileNum)) {
              fileBuffer = fileBuffer
                .replace(/<img.*?\/>/g, '');
            }

            fileBuffer = fileBuffer.replace(/ff3/g, 'ff3_ligma');
            // TODO: implement ff2 replacements

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

function copyFixedFonts() {
  try {
    const files = fs.readdirSync(FIXED_FONTS_DIR, { withFileTypes: true });
    files.forEach((file: fs.Dirent<string>) => {
      if (file.isDirectory()) return;
      const fileName = path.join(file.parentPath, file.name);
      const fileWrite = path.join(PARSED_DIR, file.name);
      fs.copyFileSync(fileName, fileWrite);
    });
  } catch(err: any) {
    console.error('Error in copying fixed fonts!\n' + err);
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

function parseWoffs() {
  return (req: Request, res: Response, next: NextFunction) => {
    try {
      const files = fs.readdirSync(KANDR_PATH, { withFileTypes: true });
      files.forEach((file: fs.Dirent<string>) => {
        if (file.isDirectory()) return;
        const fileParts = file.name.split('.');
        const fileType = fileParts[fileParts.length - 1];

        if (fileType.toLowerCase() !== 'woff') return;
        if (file.name.toLowerCase() !== F3_WOFF) return;
        // NOTE: use the following when actually reading, parsing, and re-writing
        // the font file
        // if (file.name.toLowerCase() !== F2_WOFF) return;

        const fileName = nullTerminateString(path.join(file.parentPath, file.name));
        const fileWrite = nullTerminateString(path.join(PARSED_DIR, file.name));

        // NOTE: only f2.woff and f3.woff includes invalid/private-use unicode
        // f3.woff includes a private use kerning character italicized
        // `fi` - 0xe66d
        // NOTE: f2.woff includes valid glyphs, but at wrong positions that the
        // file converter takes care of. Also has a set of 'small' capital letters
        // that should probably be extracted into its own font file
        try {
          console.log(`Parsing woff file: ${file.name}`);
          fileConversion.ParseWoff(fileName, fileWrite);
        } catch (err: any) {
          console.error(err);
          return;
        }
        return;

      });
      console.log("WOFF Parsing finished!");
      res.json({ status: 200, msg: 'Data Read: OK' }).status(200);
    } catch (err: any) {
      console.error(err);
      res.status(500).json({ status: 500, message: err.message });
    }
  }
}

export default router;
