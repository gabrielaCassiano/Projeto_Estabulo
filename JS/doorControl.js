
const ip = 'http://192.168.137.239/door/'
let estadoPorta = document.getElementById("estadoPorta");

console.log(estadoPorta)

function sendCommand(command) {
    console.log(ip + command)
    fetch(ip + command)
    .then(response => response.text())
    .then(data => estadoPorta.innerText = data)
    .catch(error => console.error('Error:', error));
}