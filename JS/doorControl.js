
const ip = 'http://192.168.137.126/'
const estadoPorta = document.getElementById("estadoPorta");
const estadoComida = document.getElementById("estadoComida");
const estadoLoad = document.getElementById("estadoDoComandoLoad")

console.log(estadoPorta)

function sendCommand(command) {
    if (command == 'food') {
        fetch(ip + 'peso/' + command)
        .then(response => response.text())
        .then(data => estadoComida.innerText = data)
        .catch(error => console.error('Error:', error));
    } else if (command == 'loadFood') {
        fetch(ip + 'peso/' + command)
        .then(response => response.text())
        .then(data => estadoLoad.innerText = data)
        .catch(error => console.error('Error:', error));
    } else {
        console.log(ip + command)
        fetch(ip + 'door/' + command)
        .then(response => response.text())
        .then(data => estadoPorta.innerText = data)
        .catch(error => console.error('Error:', error));
    }
}