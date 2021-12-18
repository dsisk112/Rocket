import { VersionController } from "../controllers/versionController.js";
import { VersionService } from "../services/versionService.js";


export function getVersionHandler (req, resp, next) {
    let versionController = new VersionController(new VersionService());
    resp.send(versionController.getVersion());
}