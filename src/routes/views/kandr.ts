import { Router, Request, Response, NextFunction } from 'express';
import path from 'path';

import { getContext } from '../../util';

const router = Router();

function basicGet(filePath: string) {
  return (req: Request, res: Response, next: NextFunction) => {
    const { ROOT_DIR, DEBUG } = getContext();
    const fileRootPath = path.join(ROOT_DIR, filePath);

    res.sendFile('kandr.html', { root: fileRootPath }, (err: Error) => {
      if (err) {
        next(err);
      } else {
        if (DEBUG) console.log('File sent');
      }

    });
  }
}

router.route('/')
  .get(basicGet('./kandr'));

router.route('/parsed')
  .get(basicGet('./kandr/parsed'));


export default router;
