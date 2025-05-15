import { Router, Request, Response, NextFunction } from 'express';
// import path from 'path';

const router = Router();

router.route('/')
  .get((req: Request, res: Response, next: NextFunction) => {
    res.send('Hello, DoopDerp! <3');
  });

export default router;
