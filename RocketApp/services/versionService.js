import SerialPort from 'serialport'
export class VersionService {
    port = 0;
    constructor() {
        this.port = new SerialPort('\\\\.\\COM3', {
            baudRate: 9600,
            autoOpen: false
        });
    }

    async getVersion() {
        return new Promise((resolve, reject) => {
            var versionRequest = [0x01, 0x00];
            var readData = 0;
            var timer = 0;
            this.port.open();
            this.port.on('error', (error) => {
                console.log("1st" + error.message);
                if (this.port.isOpen) this.port.close((err) => {});
                reject();
            })
            this.port.on('open', () => {
                this.port.write(Buffer.from(versionRequest), (err) => {
                    console.log('message written');
                    timer = setTimeout(()=> {
                        if (this.port.isOpen) this.port.close((err) => {});
                        console.log("Timeout getting version response");
                        resolve();
                    }, 5000);
                    this.port.on('data', (data) => {
                        console.log(data);
                        readData += data.toString();
                        this.port.close();
                        clearTimeout(timer);
                        resolve(readData);
                    })
                    this.port.on('error', (error)=> {
                        console.log("2nd" + error.message)
                        if (this.port.isOpen) this.port.close((err) => {});
                        reject();
                    })
                });
            })
        })
    }
}
