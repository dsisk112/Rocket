import SerialPort from 'serialport'
export class VersionService {
    port = 0;
    constructor() {
        this.port = new SerialPort('\\\\.\\COM3', {
            baudRate: 9600});
    }


    getVersion() {
     var versionRequest = [ 0x01, 0x00 ];
     this.port.write(Buffer.from(versionRequest), (err) => {
        if (err) {
            return console.log('Error: ' + err.message);
        }
        console.log('message written');
     })
     this.port.on('data', (data) => {
         console.log(data);
         readData += data.toString();
     })
     var rocketVersion = JSON.stringify({"version": "1.0.0"});
    }
}
