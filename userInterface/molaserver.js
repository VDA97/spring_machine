const WebSocket = require('ws');
const net = require('net'); // For TCP/IP socket (if chosen)

const wss = new WebSocket.Server({ port: 8080 }); // Replace with your desired port

wss.on('connection', (ws) => {
    console.log('Client connected');

    ws.on('message', (message) => {
        console.log('Received value from client:', message);
        const data = message.toString('utf-8');
        const jsonData = JSON.parse(data);
        console.log('Received value from client:', jsonData);

        // Replace with your ESP32 IP address and port
       const esp32Socket = new net.Socket();
        esp32Socket.connect(8081, '192.168.0.91', () => {
            console.log('Connected to ESP32');
            esp32Socket.write(data);
        });

        esp32Socket.on('data', (esp32Data) => {
            console.log('Received response from ESP32:', esp32Data.toString());
            // You can send a response back to the client here if needed
            ws.send(`ESP32 response: ${esp32Data.toString()}`);
        });

        esp32Socket.on('error', (error) => {
            console.error('Error connecting to ESP32:', error);
        });
    });
});

console.log('Server listening on port 8080');
