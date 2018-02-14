var SerialPort = require('serialport')

console.error('starting')
const express = require('express')
const app = express()
app.use(express.static('client/build'))
const server = require('http').createServer(app)
const io = require('socket.io')(server)

var port = new SerialPort('/dev/ttyUSB1', {
  baudRate: 57600
})

let lastDataPoint

// Received data in the following form:
// key1=value1:key2=value2:....:keyN=valueN
port.on('data', (data) => {
  const rawMeasurements = new Buffer(data).toString('ascii')
  const arrMeasurements = rawMeasurements.split(':')

  const result = arrMeasurements.map(item => {
    const split = item.split('=')
    const key = split[0]
    const valRaw = split[1]
    let value
    if (valRaw === '1') {
      value = 'true'
    } else if (valRaw === '-1') {
      value = 'false'
    } else {
      value = parseFloat(valRaw)
    }

    return {
      key,
      value
    }
  })

  lastDataPoint = result
})

// gets called whenever a client connects
io.on('connection', (socket) => {
  setInterval(() => {
    socket.emit('data', JSON.stringify(lastDataPoint))
  }, 1000)
})

server.listen(2999)
