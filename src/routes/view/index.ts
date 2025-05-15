import { Router } from 'express';

const router = Router();

import kandr from './kandr';
import root from './root';

router.use('/', root);
router.use('/kandr', kandr);

export default router;

