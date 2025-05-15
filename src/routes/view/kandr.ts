import { Router, Request, Response, NextFunction } from 'express';
import path from 'path';

import { getContext } from '../../util';

const router = Router();

router.route('/')
  .get((req: Request, res: Response, next: NextFunction) => {
    const { ROOT_DIR, DEBUG } = getContext();
    const fileRootPath = path.join(ROOT_DIR, './kandr');

    res.sendFile('kandr.html', { root: fileRootPath }, (err: Error) => {
      if (err) {
        next(err);
      } else {
        if (DEBUG) console.log('File sent');
      }

    });
  });


export default router;
