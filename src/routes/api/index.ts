import { Router } from 'express';
import testNative from './test_native'

const router = Router();

router.use('/test_native', testNative);

export default router;
