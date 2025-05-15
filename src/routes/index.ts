import path from 'path';
import { Router, Request, Response, NextFunction } from 'express';
import apiRoutes from './api';
import viewRoutes from './view';
import { getContext } from '../util';

const router = Router();

// API Routes
router.use('/api', apiRoutes);
// View Routes
router.use('/', viewRoutes);

// If no API or View routes are hit, send the base HTML
router.use((req: Request, res: Response, next: NextFunction) => {
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
