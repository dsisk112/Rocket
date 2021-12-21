import { VersionService } from '../services/versionService.js'

class VersionController {
    constructor() {
    }

    static async getVersionHandler(req, resp, next) {
        const versionService = new VersionService();
        await versionService.getVersion()
        .then((version) => {
            resp.send(version);
        }) 
        .catch((err) => {
            console.log("Error getting version");
            resp.sendStatus(401);
        });
    }
}

export default VersionController;


