function InsertionSort(lista, tamanho){
    let aux, j;
    for (let i = 0; i < tamanho; i++) {
        aux = lista[i];
        j = i-1;
        lista[0] = aux;

        while(x<lista[j]) {
            lista[j+1] = lista[j];
            j--;
        }

        lista[j+1] = aux;
    }
    print('Lista ordenada: ');
}

function main(){
    var lista = ['10', '99', '1', '33', '9'];
    print('Lista desordenada: ');
    InsertionSort(lista, lista.length);
}