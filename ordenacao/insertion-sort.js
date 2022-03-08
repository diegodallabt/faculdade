function insertionSort(lista, tamanho) {
    for (let i = 1; i < tamanho; i++){
        let j = i;
        while(j > 0 && lista[j] < lista[j-1]){
            let aux = lista[j];
            lista[j] = lista[j-1];
            lista[j-1] = aux;
            j--;
        }
    }
}

function main() {
    var lista = [10, 99, 1, 33, 9];
    console.log("Lista desordenada: ", lista)
    insertionSort(lista, lista.length);
    console.log("Lista ordenada: ", lista)
}

main();