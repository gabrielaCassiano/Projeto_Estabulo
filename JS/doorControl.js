function sendCommand(command) {
    fetch('https://192.168.1.184/esp32' + command, { method: 'POST'})
    .then(response => response.text())
    .then(data => console.log(data))
    .catch(error => console.error('Error:', error));
}