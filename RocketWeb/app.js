import express from 'express'
import path from 'path'
import { router } from './router/router.js'
import { fileURLToPath, URL } from 'url'; // in Browser, the URL in native accessible on window

const __filename = fileURLToPath(import.meta.url);
// Will contain trailing slash
const __dirname = path.dirname(__filename);
const __publicdirname = path.join(path.dirname(__filename), 'public');
const __bootstrap = path.join(path.dirname(__filename),'node_modules/bootstrap/dist/css');
const __jquery = path.join(path.dirname(__filename),'node_modules/jquery/dist');

var app = express();

app.set("port", process.env.PORT || 8000);
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));
app.use("/public", express.static(__publicdirname));
app.use("/img", express.static(path.join(__publicdirname,'img')));
app.use("/js", express.static(path.join(__publicdirname,'js')));
app.use('/css', express.static(__bootstrap));
app.use('/jquery', express.static(__jquery));
app.use(router);

app.listen(app.get("port"), function() {
    console.log("Server started on port " + app.get("port"));
})