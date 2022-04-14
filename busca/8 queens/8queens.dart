import 'dart:math';
import 'dart:core';

typedef XadrezDef = List<List<bool>>;

int nRainhas = 10;
int N = nRainhas;
int nTestes = 10;

int tentativasProfundidade = 0;
int tentativasHillClimbing = 0;

void main() {
  Stopwatch cronometro = Stopwatch();
  XadrezDef xadrez = [];
  XadrezDef xadrezInicial = [];
  XadrezDef? resultado = [];

  int mediaProfundidade = 0;
  int mediaHillClimbing = 0;
  int mediaTempoProfundidade = 0;
  int mediaTempoHillClimbing = 0;

  int fracassosProfundidade = 0;
  int fracassosHillClimbing = 0;

  //Vai fazer N testes de profundidade e N de Hill Climbing
  //Media de tempo, numero de fracassos e numero de tentativas
  for (int i = 1; i <= nTestes; i++) {
    print('\n$iª Execução\n');

    //Os dois métodos recebem o mesmo xadrez
    xadrez = generatexadrez(1);
    xadrezInicial = deepCopy(xadrez);

    print('Xadrez inicial:');
    printXadrez(xadrez);

    print('-=-=Profundidade=--=-');

    tentativasProfundidade = 0;

    cronometro.start();
    resultado = profundidade(xadrez);
    cronometro.stop();
    if (resultado == null) {
      fracassosProfundidade++;
    }

    printXadrez(resultado ?? []);

    print('Tempo: ${cronometro.elapsedMilliseconds}ms');
    print('Tentativas: $tentativasProfundidade');
    mediaProfundidade += tentativasProfundidade;
    mediaTempoProfundidade += cronometro.elapsedMilliseconds;

    cronometro.reset();

    print('-=-=Hill Climbing=--=-');
    tentativasHillClimbing = 0;
    cronometro.start();
    resultado = hillClimbing(xadrezInicial);
    cronometro.stop();
    if (resultado == null) {
      fracassosHillClimbing++;
    }
    printXadrez(resultado ?? []);
    print('Tempo: ${cronometro.elapsedMilliseconds}ms');
    print('Tentativas: $tentativasHillClimbing');
    mediaHillClimbing += tentativasHillClimbing;
    mediaTempoHillClimbing += cronometro.elapsedMilliseconds;
  }

  print('=-=-=-=Resultados=-=-=-=');

  print(
      'Media do tempo de execução de profundidade: ${mediaTempoProfundidade / nTestes}ms');
  print(
      'Media do tempo de execução de Hill Climbing: ${mediaTempoHillClimbing / nTestes}ms');

  print('Media de tentativas de Profundidade: ${mediaProfundidade / nTestes}');
  print('Media de tentativas de Hill Climbing: ${mediaHillClimbing / nTestes}');

  print('Fracassos de Profundidade: $fracassosProfundidade');
  print('Fracassos de Hill Climbing: $fracassosHillClimbing');
}

//Função responsável por imprimir o xadrez
void printXadrez(XadrezDef xadrez) {
  if (xadrez.isNotEmpty) {
    for (int i = 0; i < N; i++) {
      print(xadrez[i].map((e) {
        if (e == true) {
          return 'R';
        } else {
          return '-';
        }
      }));
    }
  } else {
    print('Nenhuma solução encontrada');
  }
  print('\n');
}

//A função responsável por fazer cálculo de ataque de rainhas
int calcularAtaques(XadrezDef xadrez) {
  int totalh = 0;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (xadrez[i][j] == true) {
        //Ele remove 2 pois ele não pode atacar a si mesmo
        totalh -= 2;

        //Verifica todas as direções que uma rainha pode atacar

        for (int k = 0; k < N; k++) {
          if (xadrez[i][k] == true) {
            totalh++;
          }
          if (xadrez[k][j] == true) {
            totalh++;
          }
        }
        for (int k = i + 1, l = j + 1; k < N && l < N; k++, l++) {
          if (xadrez[k][l] == true) {
            totalh++;
          }
        }

        for (int k = i + 1, l = j - 1; k < N && l >= 0; k++, l--) {
          if (xadrez[k][l] == true) {
            totalh++;
          }
        }

        for (int k = i - 1, l = j + 1; k >= 0 && l < N; k--, l++) {
          if (xadrez[k][l] == true) {
            totalh++;
          }
        }

        for (int k = i - 1, l = j - 1; k >= 0 && l >= 0; k--, l--) {
          if (xadrez[k][l] == true) {
            totalh++;
          }
        }
      }
    }
  }

  //Enquanto não possuir as N rainhas, ele vai penalizar a heurística para o programa inserir as N rainhas
  if (nRainhasCol(xadrez) != nRainhas) totalh += nRainhas;

  //Divide por dois, já que ele calcula duas vezes os ataques
  return ((totalh) ~/ 2);
}

//Função do Hill Climbing
XadrezDef? hillClimbing(XadrezDef xadrez) {
  XadrezDef currentxadrez = deepCopy(xadrez);
  XadrezDef vizinho = [];
  int i = 0;

  //Irá executar apenas até 250 tentativas
  while (i++ != 250) {
    //Vai procurar um vizinho melhor
    vizinho = procuraMelhor(currentxadrez);

    //Se seu número de ataque for igual a 0, significa que ele não tem ataque
    if (calcularAtaques(currentxadrez) == 0) {
      return currentxadrez;
    }

    //Se não achou nenhum vizinho, significa que ele ficou sem solução
    if (vizinho.isEmpty) {
      //Será gerado uma nova possibilidade (porém seu número de tentativas permanecerá o mesmo)
      currentxadrez = generatexadrez(nRainhas);
    } else {
      //Se achou um vizinho, ele será o novo xadrez
      currentxadrez = deepCopy(vizinho);
    }
  }
  return null;
}

//Dart faz a passagem como referência, mas existem casos que deve ser feitos como cópia
//Essa função faz a cópia
XadrezDef deepCopy(XadrezDef source) {
  return source.map((e) => e.toList()).toList();
}

//Procura o vizinho melhor
XadrezDef procuraMelhor(XadrezDef xadrez) {
  int actualCost = calcularAtaques(xadrez);
  XadrezDef child = [];

  //Ele vai percorrendo todas as possibilidades
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      //Ele faz uma nova linha, testando cada inserção de uma rainha
      XadrezDef xadrezTemp = [];
      xadrezTemp = deepCopy(xadrez);

      //Vai testando uma rainha na linha, em cada coluna
      List<bool> rowTemp = List.generate(N, (index) => false);
      rowTemp[col] = true;

      //Copia a linha para a copia do xadrez
      //Onde só a linha vai alterar
      xadrezTemp[row] = List.from(rowTemp);

      //Calcula o ataque
      int tempHCost = calcularAtaques(xadrezTemp);

      tentativasHillClimbing++;

      //Se o xadrez atual não for igual ao xadrez melhor
      //E o ataque for menor que o ataque atual
      //Ele faz a troca para novas comparações
      if (xadrezTemp != xadrez &&
          tempHCost < actualCost &&
          xadrezTemp.isNotEmpty) {
        child = deepCopy(xadrezTemp);
        actualCost = tempHCost;
      }
    }
  }

  //Retorna o melhor, onde caso não tenha nenhum melhor, retorna um vetor vazio
  return child;
}

//Gera uma solução aleatória com N rainhas
XadrezDef generatexadrez(int numRainhas) {
  XadrezDef xadrez = [];
  for (int i = 0; i < N; i++) {
    List<bool> row = List.generate(N, (index) => false);
    if (numRainhas > 0) {
      int j = Random().nextInt(N);
      row[j] = true;
      numRainhas--;
    }
    xadrez.add(row);
  }
  return xadrez;
}

//Verifica se é possível inserir
bool possivelInserir(XadrezDef xadrez, int x, y) {
  for (int i = 0; i < xadrez.length; i++) {
    //Verifica na coluna e na linha
    if (xadrez[i][y] == true || xadrez[x][i] == true) {
      return false;
    }
  }

  //Verifica na diagonal
  for (int i = 0; i < xadrez.length; i++) {
    for (int j = 0; j < xadrez.length; j++) {
      if (xadrez[i][j] == true) {
        if ((j - y).abs() == (i - x).abs()) {
          return false;
        }
      }
    }
  }
  //Caso não achar, ele retorna true
  return true;
}

//Contabiliza todas as rainhas que achar
int nRainhasCol(XadrezDef xadrez) {
  int count = 0;
  for (int i = 0; i < xadrez.length; i++) {
    //Ele percorre todas linhas
    //Conta quantas rainhas tem na linha
    count += xadrez[i].where((element) => element).toList().length;
  }

  return count;
}

XadrezDef? profundidade(XadrezDef xadrez) {
  //Vai testar todas posições
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      //Caso não tenha rainha naquela posição
      //E for possível inserir uma nova rainha
      if (xadrez[i][j] == false && possivelInserir(xadrez, i, j)) {
        //Ele insere uma nova rainha
        xadrez[i][j] = true;
        tentativasProfundidade++;
        //Chama uma nova função para testar a posição com outras rainhas
        profundidade(xadrez);

        //Essa função recursiva, gera uma espécie de árvore
        //já que cada função (nó), gera novas funções (filhos)

        //Conseguiu atingir o objetivo
        if (nRainhasCol(xadrez) == nRainhas) {
          return xadrez;
        }

        //Caso não seja, ela vai tirar a rainha dessa posição
        xadrez[i][j] = false;
      }
    }
  }
  return null;
}
