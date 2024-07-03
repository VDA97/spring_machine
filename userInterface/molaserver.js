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

        // Process the received value (e.g., validate, format, etc.)
        const processedValue = parseFloat(message);

        // Choose communication method for ESP32: WebSocket or TCP/IP
        // **Option 1: Using a WebSocket (if ESP32 supports it):**

        // Assuming you have a separate WebSocket server running on the ESP32:
       /* const esp32Ws = new WebSocket('ws://your_esp32_ip:esp32_port'); // Replace with ESP32 WebSocket address

        esp32Ws.onopen = () => {
            esp32Ws.send(processedValue.toString());
        };

        esp32Ws.onmessage = (esp32Message) => {
            console.log('Received response from ESP32:', esp32Message.data);
            // You can send a response back to the client here if needed
            ws.send(`ESP32 response: ${esp32Message.data}`);
        };*/

        // **Option 2: Using a TCP/IP socket (if ESP32 doesn't support WebSocket):**

        // Replace with your ESP32 IP address and port
       const esp32Socket = new net.Socket();
        esp32Socket.connect(8081, '192.168.0.91', () => {
            console.log('Connected to ESP32');
            esp32Socket.write(processedValue.toString());
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
