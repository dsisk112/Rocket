import { VersionService } from '../services/versionService.js'

class VersionController {
    constructor() {
    }

    static async getVersionHandler(req, resp, next) {
        const versionService = new VersionService();
        const version = await versionService.getVersion();
        resp.send(version);
    }
}

export default VersionController;


