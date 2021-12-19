const path = require('path');

const backend = {
    mode: 'production',
    entry: [
        './RocketApp/app.js'
    ],
    output: {
        filename: './bundle/rocket-app.js',
        path: path.resolve(__dirname),
    },
    target: 'node',
};

module.exports = [
    Object.assign({}, backend)
];