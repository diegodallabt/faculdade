function bubbleSort(lista, tamanho){
    var aux;
    if (tamanho < 1) return lista;

    for (var i = 0; i < tamanho; i++) {
        if(lista[i] > lista[i+1]){
            aux = lista[i];
            lista[i] = lista[i+1];
            lista[i+1] = aux;
        }
        bubbleSort(lista, tamanho-1);
    }
    
}

function main() {
    var lista = [10, 99, 1, 33, 9];
    console.log("Lista desordenada: ", lista)
    bubbleSort(lista, lista.length-1);
    console.log("Lista ordenada: ", lista)
}

main();


