function inverte(str) {
    if (str === "") {
        return "";
    } else {
        return inverte(str.substr(1)) + str.charAt(0);
        // retorna a frase sem a primeira letra + a concatenção da primeira letra no final da frase
    }
}

let inversaoString = inverte("Diego Dalla");
console.log(inversaoString)