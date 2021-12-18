import express  from 'express'
import { getVersionHandler } from '../handlers/versionHandler.js';

export const router = express.Router()

router.get('/version', getVersionHandler); 


