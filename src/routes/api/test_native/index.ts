import { Router, Request, Response, NextFunction } from 'express';
import { hello, hello2 } from '../../../extensions';

const router = Router();

router.route('/')
  .get((req: Request, res: Response, next: NextFunction) => {
    try {
      console.log(hello.hello());
      console.log(hello2.hello());
      res.json({status: 'Ok'}).status(200);
    } catch (err: any) {
      console.error(err);
      // next(err);
      res.status(500).json({status: 500, message: 'Error: native extensions test'});
    }
  });

export default router;
