const express = require('express');
const Model = require('../model/model')

const router = express.Router()

//Post Method
router.post('/post', (req, res) => {
    console.log(`Name received = ${req.body.name}`)
    res.send('Received Thanks')
})

module.exports = router;