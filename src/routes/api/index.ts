import { Router } from 'express';
import testNative from './test_native';
import conversion from './conversion';

const router = Router();

router.use('/test_native', testNative);
router.use('/conversion', conversion);

export default router;
