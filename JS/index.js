const email = document.getElementById("email");
const senha = document.getElementById("pass"); 
const botaoSubmit = document.getElementById("submit");
const form = document.getElementById("loginFields");
const deslocamento = 3;
const deslocamentoNumero = 5;
const senhaCod = "JdelCPduld7579";
const emailValid = "gabiiv.cassiano@gmail.com"

form.addEventListener('submit', salvandoDados);



// codificar dados
function salvandoDados(event) {
    event.preventDefault()
    let dadoSenha = senha.value;
    let dadoEmail = email.value;

    email.value = "";
    senha.value = ""; 

    
    
//Verificacao se a senha que foi digitada e igual esta correta 

    let senhaCodificada = codifica(dadoSenha, deslocamento, deslocamentoNumero);
    verificarDados(senhaCodificada, dadoEmail);

}





// Codificação da senha
function codifica(senha, deslocamento, deslocamentoNumero) {
    const alfabeto = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.';
    const numero = "0123456789"
    let resultado = '';

    for (let i = 0; i < senha.length; i++) {
        let charAtual = senha[i];
    
        if(numero.indexOf(charAtual) < 0){
            let indiceAtual = alfabeto.indexOf(charAtual);
    
            let indiceCifrado = (indiceAtual + deslocamento) % alfabeto.length;
            let letraCifrada = alfabeto[indiceCifrado];
            resultado += letraCifrada;
        } else {
            let indiceAtual = numero.indexOf(charAtual);
    
            let indiceCifrado = (indiceAtual + deslocamentoNumero) % numero.length;
            let numeroCifrado = numero[indiceCifrado];
            resultado += numeroCifrado;
        }
    }

    return resultado;
}

function verificarDados(senhaCodificada, dadoEmail) {
    
    if (senhaCodificada != senhaCod && dadoEmail != emailValid) {
        alert("SENHA E EMAIL INVALIDOS, DIGITE NOVAMENTE")
    } else if(senhaCodificada != senhaCod){
        alert("SENHA INVALIDA, DIGITE NOVAMENTE")
    } else if(dadoEmail != emailValid) {
        alert("EMAIL INVALIDO, DIGITE NOVAMENTE")
    } else {
        window.location.href = "../mainPage.html";
    }
}