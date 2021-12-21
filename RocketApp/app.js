import express from 'express'
import { router } from './router/router.js'

const app = express()

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
  });


app.get('/', (req, resp) => resp.send('App is working')) 
app.use('/api', router)

app.listen(3000, () => console.log('RocketApp is listending on port 3000'))
