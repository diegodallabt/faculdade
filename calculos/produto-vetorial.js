// faz o produto das diagonais da matriz, tanto a principal quanto a inversa
function produtoVetorial() {
    var matriz = matrizIJK([3,1,2], [-2,2,5]);
    var primeiraDiag = [], segundaDiag = [];

    var y = 0;    
    for (let x = 1; x <= 3; x++) { 
        primeiraDiag[y] = (matriz[0][x] * matriz[1][x+1]);
        y++;
    }

    y=0;
    for (x = 3; x >= 1; x--) { 
        segundaDiag[y] = -(matriz[0][x] * matriz[1][x-1]);
        y++;
    }
    exibeDeterminante(somaVariaveis(primeiraDiag, segundaDiag));
}


// gera a matriz para fazer a multiplicação das diagonais
// i   j   k   i   j
// x1  y1  z1  x1  y1
// x2  y2  z2  x2  y2
function matrizIJK(v1, v2) {
    v1.push(v1[0], v1[1]);
    v2.push(v2[0], v2[1]);
    var vetAux = [v1, v2];

    return vetAux;
}

// coloca os resultados em um vetor somando variáveis repetidas
function somaVariaveis(pd, sd) {
    var vetAux = [pd[0] + sd[1], pd[1] + sd [0], pd[2] + sd [2]];
    return vetAux;
}

// exibe os resultados na tela
function exibeDeterminante(v) {
    console.log('O determinante do produto vetorial desses vetores é: ' + v[0] + 'i ' + v[1]+ 'j ' + v[2] + 'k'+ ' ou ' + `(${v[0]}, ${v[1]}, ${v[2]}).`);
}

produtoVetorial();