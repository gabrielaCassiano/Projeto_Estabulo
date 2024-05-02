const email = document.getElementById("email");
const senha = document.getElementById("pass"); 
const botaoSubmit = document.getElementById("submit");
const deslocamento = 3;
const deslocamentoNumero = 5;
const senhaCod = "JdelCPduld7579";
const emailValid = "gabiiv.cassiano@gmail.com"

botaoSubmit.addEventListener('click', salvandoDados);



// codificar dados
function salvandoDados() {
    let dadoSenha = senha.value;
    let dadoEmail = email.value;

    email.value = "";
    senha.value = ""; 

    
    
//Verificacao se a senha que foi digitada e igual esta correta 

    let verificaSenha = codifica(dadoSenha, deslocamento, deslocamentoNumero);
    
    if(verificaSenha != senhaCod){
        alert("SENHA INVALIDA, DIGITE NOVAMENTE")
    } else if(dadoEmail != emailValid) {
        alert("EMAIL INVALIDO, DIGITE NOVAMENTE")
    } else {
        console.log("ENTROUUUUU")
    }
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