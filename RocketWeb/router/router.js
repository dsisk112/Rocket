
import express  from 'express'

export const router = express.Router()

router.get('/', (req,res) => { 
    console.log("Start Page");
    res.render("index");
}); 