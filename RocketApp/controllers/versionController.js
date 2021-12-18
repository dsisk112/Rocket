import { VersionService } from '../services/versionService.js'

export class VersionController {
    constructor(versionService) {
        this._versionService = versionService
    }

    getVersion() {
        return (this._versionService.getVersion());
    }
}


