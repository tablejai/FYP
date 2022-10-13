const mongoose = require('mongoose');

const dataSchema = new mongoose.Schema({


    TimeStamp: {
        required: true,
        type: Date
    },

    IMUData: {
        require: true,
        type: Number

    }
})

module.exports = mongoose.model('Data', dataSchema)