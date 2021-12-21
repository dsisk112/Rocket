import express  from 'express'
// import { getVersionHandler } from '../handlers/versionHandler.js';
import VersionController from '../controllers/versionController.js'

export const router = express.Router()

router.get('/version', VersionController.getVersionHandler); 


