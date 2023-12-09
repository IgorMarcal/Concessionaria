#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    char venda[100];
    char preco[100];
    char ano[100];
    char marca[100];
    char modelo[100];
    char condicao[100];
    char combustivel[100];
    char odometro[100];
    char status[100];
    char cambio[100];
    char tamanho[100];
    char tipo[100];
    char cor[100];
    int codigoInterno;
} Veiculo;

typedef struct {
    char nome[100];
    float taxa;
} Marca;

void leArquivo(char arquivo[], Veiculo ***dados, int *linhas) {
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL) {
        printf("Falha ao abrir o arquivo para leitura.\n");
        return;
    }

    char linha[1000];
    int tamanhoMaximo = 10000000;
    int codigoInternoAtual = 1; 

    *dados = (Veiculo **)malloc(tamanhoMaximo * sizeof(Veiculo *));
    if (*dados == NULL) {
        printf("Erro ao alocar memoria.\n");
        fclose(arq);
        return;
    }

    int contaLinhas = 0;
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        linha[strcspn(linha, "\r\n")] = 0;

        (*dados)[contaLinhas] = (Veiculo *)malloc(sizeof(Veiculo));
        if ((*dados)[contaLinhas] == NULL) {
            printf("Erro ao alocar memoria para veiculo.\n");
            fclose(arq);
            return;
        }

        strcpy((*dados)[contaLinhas]->preco, linha);
        strcpy((*dados)[contaLinhas]->ano, linha);
        strcpy((*dados)[contaLinhas]->marca, linha);
        (*dados)[contaLinhas]->codigoInterno = codigoInternoAtual;
        codigoInternoAtual++;
        
        sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
            (*dados)[contaLinhas]->preco, (*dados)[contaLinhas]->ano, (*dados)[contaLinhas]->marca,
            (*dados)[contaLinhas]->modelo, (*dados)[contaLinhas]->condicao, (*dados)[contaLinhas]->combustivel,
            (*dados)[contaLinhas]->odometro, (*dados)[contaLinhas]->status, (*dados)[contaLinhas]->cambio,
            (*dados)[contaLinhas]->tamanho, (*dados)[contaLinhas]->tipo, (*dados)[contaLinhas]->cor);

        contaLinhas++;
    }
  


    *linhas = contaLinhas;

    fclose(arq);
}

bool reescreverArquivos(Veiculo **veiculos, int codigoVeiculo, int qtdVeiculos, char  veiculosEstoque[],char veiculosOfertas[],char historicoDeCompras[], char novoValor[]) {
    time_t currentTime;
    time(&currentTime);
    char *dateTimeString = ctime(&currentTime);
    dateTimeString[strcspn(dateTimeString, "\r\n")] = 0;

    FILE *arquivo1;
    FILE *arquivo2;
    FILE *arquivo3;

    if (access(veiculosEstoque, F_OK) == -1) {
        arquivo2 = fopen(veiculosEstoque, "a+");
        fprintf(arquivo2,"venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");
    }else{
        arquivo2 = fopen(veiculosEstoque, "a+");
    }

    if (access(historicoDeCompras, F_OK) == -1) {
        arquivo3 = fopen(historicoDeCompras, "a+");
        fprintf(arquivo3, "venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor,dataHora\n");
    }else{
        arquivo3 = fopen(historicoDeCompras, "a+");
    }

    arquivo1 = fopen(veiculosOfertas, "w");
    fflush(arquivo2);
    fprintf(arquivo1,"preco,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");
    fflush(arquivo1);

    for (int i = 2; i < qtdVeiculos; i++) {
        if (i == codigoVeiculo) {
            strcpy(veiculos[i]->venda, novoValor);
            fprintf(arquivo2, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", veiculos[i]->venda, veiculos[i]->ano, veiculos[i]->marca,
                veiculos[i]->modelo, veiculos[i]->condicao, veiculos[i]->combustivel, veiculos[i]->odometro,
                veiculos[i]->status, veiculos[i]->cambio, veiculos[i]->tamanho, veiculos[i]->tipo, veiculos[i]->cor);
            
            fprintf(arquivo3, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", veiculos[i]->venda, veiculos[i]->ano, veiculos[i]->marca,
                veiculos[i]->modelo, veiculos[i]->condicao, veiculos[i]->combustivel, veiculos[i]->odometro,
                veiculos[i]->status, veiculos[i]->cambio, veiculos[i]->tamanho, veiculos[i]->tipo, veiculos[i]->cor, dateTimeString);
        } else {
            fprintf(arquivo1, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", veiculos[i]->preco, veiculos[i]->ano, veiculos[i]->marca,
                veiculos[i]->modelo, veiculos[i]->condicao, veiculos[i]->combustivel, veiculos[i]->odometro,
                veiculos[i]->status, veiculos[i]->cambio, veiculos[i]->tamanho, veiculos[i]->tipo, veiculos[i]->cor);
        }
    }

    return true;
}

char *filtrosVeiculos(int *qtdFiltros) {
    char *filtrosUtilizados = (char *)malloc(200 * sizeof(char));
    filtrosUtilizados[0] = '\0'; 
    char filtroMarca[20], filtroAno[20], filtroPreco[20], filtroModelo[20];
    char filtroCondicao[20], filtroCombustivel[20], filtroOdometro[20], filtroStatus[20];
    char filtroCambio[20], filtroTamanho[20], filtroTipo[20], filtroCor[20];
    *qtdFiltros = 0;

    printf("Quer utilizar filtro de marca? (sim/nao): ");
    scanf(" %s", filtroMarca);
    if (strcmp(filtroMarca, "sim") == 0) {
        printf("Digite a marca do veiculo que deseja: ");
        scanf(" %[^\n]", filtroMarca);
        strcat(filtrosUtilizados, "Marca ");
        strcat(filtrosUtilizados, filtroMarca);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de ano? (sim/nao): ");
    scanf(" %s", filtroAno);
    if (strcmp(filtroAno, "sim") == 0) {
        printf("Digite o ano do veiculo que deseja: ");
        scanf(" %[^\n]", filtroAno);
        strcat(filtrosUtilizados, "Ano ");
        strcat(filtrosUtilizados, filtroAno);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de preco? (sim/nao): ");
    scanf(" %s", filtroPreco);
    if (strcmp(filtroPreco, "sim") == 0) {
        printf("Digite o preco desejado: ");
        scanf(" %[^\n]", filtroPreco);
        strcat(filtrosUtilizados, "Preco ");
        strcat(filtrosUtilizados, filtroPreco);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de modelo? (sim/nao): ");
    scanf(" %s", filtroModelo);
    if (strcmp(filtroModelo, "sim") == 0) {
        printf("Digite o modelo do veiculo que deseja: ");
        scanf(" %[^\n]", filtroModelo);
        strcat(filtrosUtilizados, "Modelo ");
        strcat(filtrosUtilizados, filtroModelo);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de condicão? (sim/nao): ");
    scanf(" %s", filtroCondicao);
    if (strcmp(filtroCondicao, "sim") == 0) {
        printf("Digite a condicão do veiculo que deseja: ");
        scanf(" %[^\n]", filtroCondicao);
        strcat(filtrosUtilizados, "Condicão ");
        strcat(filtrosUtilizados, filtroCondicao);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de combustivel? (sim/nao): ");
    scanf(" %s", filtroCombustivel);
    if (strcmp(filtroCombustivel, "sim") == 0) {
        printf("Digite o tipo de combustivel desejado: ");
        scanf(" %[^\n]", filtroCombustivel);
        strcat(filtrosUtilizados, "Combustivel ");
        strcat(filtrosUtilizados, filtroCombustivel);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de odometro? (sim/nao): ");
    scanf(" %s", filtroOdometro);
    if (strcmp(filtroOdometro, "sim") == 0) {
        printf("Digite a faixa de odometro desejada: ");
        scanf(" %[^\n]", filtroOdometro);
        strcat(filtrosUtilizados, "Odometro ");
        strcat(filtrosUtilizados, filtroOdometro);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de status? (sim/nao): ");
    scanf(" %s", filtroStatus);
    if (strcmp(filtroStatus, "sim") == 0) {
        printf("Digite o status desejado: ");
        scanf(" %[^\n]", filtroStatus);
        strcat(filtrosUtilizados, "Status ");
        strcat(filtrosUtilizados, filtroStatus);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de cambio? (sim/nao): ");
    scanf(" %s", filtroCambio);
    if (strcmp(filtroCambio, "sim") == 0) {
        printf("Digite o tipo de cambio desejado: ");
        scanf(" %[^\n]", filtroCambio);
        strcat(filtrosUtilizados, "Cambio ");
        strcat(filtrosUtilizados, filtroCambio);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de tamanho? (sim/nao): ");
    scanf(" %s", filtroTamanho);
    if (strcmp(filtroTamanho, "sim") == 0) {
        printf("Digite o tamanho desejado: ");
        scanf(" %[^\n]", filtroTamanho);
        strcat(filtrosUtilizados, "Tamanho ");
        strcat(filtrosUtilizados, filtroTamanho);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de tipo? (sim/nao): ");
    scanf(" %s", filtroTipo);
    if (strcmp(filtroTipo, "sim") == 0) {
        printf("Digite o tipo desejado: ");
        scanf(" %[^\n]", filtroTipo);
        strcat(filtrosUtilizados, "Tipo ");
        strcat(filtrosUtilizados, filtroTipo);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    printf("Quer utilizar filtro de cor? (sim/nao): ");
    scanf(" %s", filtroCor);
    if (strcmp(filtroCor, "sim") == 0) {
        printf("Digite a cor desejada: ");
        scanf(" %[^\n]", filtroCor);
        strcat(filtrosUtilizados, "Cor ");
        strcat(filtrosUtilizados, filtroCor);
        strcat(filtrosUtilizados, ", ");
        (*qtdFiltros)++;
    }

    int tamanho = strlen(filtrosUtilizados);
    if (tamanho > 0) {
        filtrosUtilizados[tamanho - 2] = '\0'; 
    }


    return filtrosUtilizados;
}

Veiculo **buscaMarca(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorMarca = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char marca[50];
    printf("Qual marca deseja buscar? \n");
    scanf(" %[^\n]", marca);
    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(marca, dados[i]->marca) == 0) {
            resultados[qtdVeicPorMarca] = dados[i];
            qtdVeicPorMarca++;
        }
    }

    if (qtdVeicPorMarca == 0) {
        printf("Nenhum veiculo encontrado para a marca %s\n", marca);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorMarca * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorMarca;
    return resultados;
}

Veiculo **buscaPreco(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorPreco = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char preco[20];
    printf("Qual preco deseja buscar? \n");
    scanf(" %[^\n]", preco);
    
    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(preco, dados[i]->preco) == 0) {
            resultados[qtdVeicPorPreco] = dados[i];
            qtdVeicPorPreco++;
        }
    }

    if (qtdVeicPorPreco == 0) {
        printf("Nenhum veiculo encontrado para o preco %s\n", preco);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorPreco * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorPreco;
    return resultados;
}

Veiculo **buscaAno(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorAno = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char ano[20];
    printf("Qual ano deseja buscar? \n");
    scanf(" %[^\n]", ano);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(ano, dados[i]->ano) == 0) {
            resultados[qtdVeicPorAno] = dados[i];
            qtdVeicPorAno++;
        }
    }

    if (qtdVeicPorAno == 0) {
        printf("Nenhum veiculo encontrado para o ano %s\n", ano);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorAno * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorAno;
    return resultados;
}

Veiculo **buscaModelo(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorModelo = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char modelo[50];
    printf("Qual modelo deseja buscar? \n");
    scanf(" %s", modelo);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(modelo, dados[i]->modelo) == 0) {
            resultados[qtdVeicPorModelo] = dados[i];
            qtdVeicPorModelo++;
        }
    }

    if (qtdVeicPorModelo == 0) {
        printf("Nenhum veiculo encontrado para o modelo %s\n", modelo);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorModelo * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorModelo;
    return resultados;
}


Veiculo **buscaCombustivel(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorCombustivel = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char combustivel[50];
    printf("Qual tipo de combustivel deseja buscar? \n");
    scanf(" %[^\n]", combustivel);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(combustivel, dados[i]->combustivel) == 0) {
            resultados[qtdVeicPorCombustivel] = dados[i];
            qtdVeicPorCombustivel++;
        }
    }

    if (qtdVeicPorCombustivel == 0) {
        printf("Nenhum veiculo encontrado para o tipo de combustivel %s\n", combustivel);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorCombustivel * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorCombustivel;
    return resultados;
}


Veiculo **buscaOdometro(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorOdometro = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char odometro[50];
    printf("Digite a faixa de odometro desejada: ");
    scanf(" %[^\n]", odometro);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strstr(dados[i]->odometro, odometro) != NULL) {
            resultados[qtdVeicPorOdometro] = dados[i];
            qtdVeicPorOdometro++;
        }
    }

    if (qtdVeicPorOdometro == 0) {
        printf("Nenhum veiculo encontrado para a faixa de odometro %s\n", odometro);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorOdometro * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorOdometro;
    return resultados;
}

Veiculo **buscaStatus(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorStatus = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char status[50];
    printf("Digite o status desejado: ");
    scanf(" %[^\n]", status);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(dados[i]->status, status) == 0) {
            resultados[qtdVeicPorStatus] = dados[i];
            qtdVeicPorStatus++;
        }
    }

    if (qtdVeicPorStatus == 0) {
        printf("Nenhum veiculo encontrado para o status %s\n", status);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorStatus * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorStatus;
    return resultados;
}

Veiculo **buscaCambio(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorCambio = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char cambio[50];
    printf("Digite o tipo de cambio desejado: ");
    scanf(" %[^\n]", cambio);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(dados[i]->cambio, cambio) == 0) {
            resultados[qtdVeicPorCambio] = dados[i];
            qtdVeicPorCambio++;
        }
    }

    if (qtdVeicPorCambio == 0) {
        printf("Nenhum veiculo encontrado para o tipo de cambio %s\n", cambio);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorCambio * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorCambio;
    return resultados;
}

Veiculo **buscaTamanho(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorTamanho = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char tamanho[50];
    printf("Digite o tamanho desejado: ");
    scanf(" %[^\n]", tamanho);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(dados[i]->tamanho, tamanho) == 0) {
            resultados[qtdVeicPorTamanho] = dados[i];
            qtdVeicPorTamanho++;
        }
    }

    if (qtdVeicPorTamanho == 0) {
        printf("Nenhum veiculo encontrado para o tamanho %s\n", tamanho);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorTamanho * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorTamanho;
    return resultados;
}

Veiculo **buscaTipo(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorTipo = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char tipo[50];
    printf("Digite o tipo desejado: ");
    scanf(" %[^\n]", tipo);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(dados[i]->tipo, tipo) == 0) {
            resultados[qtdVeicPorTipo] = dados[i];
            qtdVeicPorTipo++;
        }
    }

    if (qtdVeicPorTipo == 0) {
        printf("Nenhum veiculo encontrado para o tipo %s\n", tipo);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorTipo * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorTipo;
    return resultados;
}

Veiculo **buscaCor(Veiculo **dados, int *qtdVeiculos) {
    int qtdVeicPorCor = 0;
    Veiculo **resultados = malloc(*qtdVeiculos * sizeof(Veiculo *));
    char cor[50];
    printf("Digite a cor desejada: ");
    scanf(" %[^\n]", cor);

    for (int i = 0; i < *qtdVeiculos; i++) {
        if (strcmp(dados[i]->cor, cor) == 0) {
            resultados[qtdVeicPorCor] = dados[i];
            qtdVeicPorCor++;
        }
    }

    if (qtdVeicPorCor == 0) {
        printf("Nenhum veiculo encontrado para a cor %s\n", cor);
        free(resultados);
        return NULL;
    }

    resultados = realloc(resultados, qtdVeicPorCor * sizeof(Veiculo *));
    
    *qtdVeiculos = qtdVeicPorCor;
    return resultados;
}


void listarVeiculosEncontrados(Veiculo **resultados, int qtdVeiculos) {
    if (resultados == NULL || qtdVeiculos == 0) {
        printf("Nenhum veiculo encontrado.\n");
        return;
    }

    printf("Veiculos encontrados:\n");

    for (int i = 0; i < qtdVeiculos; i++) {
        printf("Codigo: %d | Marca: %s | Preco: %s | Modelo: %s | Ano: %s | Cor: %s | Odometro: %s\n",
               resultados[i]->codigoInterno,
               resultados[i]->preco,
               resultados[i]->marca,
               resultados[i]->modelo,
               resultados[i]->ano,
               resultados[i]->cor,
               resultados[i]->odometro);
    }
}


int compraVeiculos(FILE *arq, char arquivo[]) {
    Veiculo **dados;
    int qtdVeiculos;
    int tamanhoMaximo = 10000;
    int linhas = 0, colunas = 0;
    char marcaVeiculo[60];
    int qtdVeicPorMarca = 2;
    bool encontrou = false;

    int qtdFiltros = 0;
    leArquivo(arquivo, &dados, &qtdVeiculos);
    char filtroMarca[20], filtroAno[20], filtroPreco[20], filtroModelo[20];
    char filtroCondicao[20], filtroCombustivel[20], filtroOdometro[20], filtroStatus[20];
    char filtroCambio[20], filtroTamanho[20], filtroTipo[20], filtroCor[20];
    Veiculo **resultados = NULL;

    printf("Quer utilizar filtro de marca? (sim/nao): ");
    scanf(" %s", filtroMarca);
    Veiculo **resultadosMarca = NULL;
    if(strcmp(filtroMarca,"sim") == 0){ 
        resultadosMarca = buscaMarca(dados, &qtdVeiculos);
        printf("%d", qtdVeiculos);
        listarVeiculosEncontrados(resultadosMarca, qtdVeiculos);
        resultados = resultadosMarca;
    } 

    printf("Quer utilizar filtro de preco? (sim/nao): ");
    scanf(" %s", filtroPreco); 
    Veiculo **resultadosPreco = NULL;
    if(strcmp(filtroPreco, "sim") == 0) {
        resultadosPreco = buscaPreco(resultados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosPreco, qtdVeiculos);
        resultados = resultadosPreco;

    }

    printf("Quer utilizar filtro de ano? (sim/nao): ");
    scanf(" %s", filtroAno);
    Veiculo **resultadosAno = NULL;
    if(strcmp(filtroAno, "sim") == 0) {
        resultadosAno = buscaAno(resultados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosAno, qtdVeiculos);
        resultados = resultadosAno;
    }
    
    
    printf("Quer utilizar filtro de modelo? (sim/nao): ");
    scanf(" %s", filtroModelo);
    Veiculo **resultadosModelo = NULL;
    if(strcmp(filtroModelo, "sim") == 0) {
        resultadosModelo = buscaModelo(resultadosAno, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosModelo, qtdVeiculos);
        resultados = resultadosModelo;
    }
    


    printf("Quer utilizar filtro de combustivel? (sim/nao): ");
    scanf(" %s", filtroCombustivel);
    Veiculo **resultadosCombustivel = NULL;
    if(strcmp(filtroCombustivel, "sim") == 0) {
        resultadosCombustivel = buscaCombustivel(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosCombustivel, qtdVeiculos);
        resultados = resultadosCombustivel;
    }
    
    
    printf("Quer utilizar filtro de odometro? (sim/nao): ");
    scanf(" %s", filtroOdometro);
    Veiculo **resultadosOdometro = NULL;
    if(strcmp(filtroOdometro, "sim") == 0) {
        resultadosOdometro = buscaOdometro(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosOdometro, qtdVeiculos);
        resultados = resultadosOdometro;
    }

    printf("Quer utilizar filtro de status? (sim/nao): ");
    scanf(" %s", filtroStatus);
    Veiculo **resultadosStatus = NULL;
    if(strcmp(filtroStatus, "sim") == 0) {
        resultadosStatus = buscaStatus(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosStatus, qtdVeiculos);
        resultados = resultadosStatus;
    }

    printf("Quer utilizar filtro de cambio? (sim/nao): ");
    scanf(" %s", filtroCambio);
    Veiculo **resultadosCambio = NULL;
    if(strcmp(filtroCambio, "sim") == 0) {
        resultadosCambio = buscaCambio(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosCambio, qtdVeiculos);
        resultados = resultadosCambio;
    }

    printf("Quer utilizar filtro de tamanho? (sim/nao): ");
    scanf(" %s", filtroTamanho);   
    Veiculo **resultadosTamanho = NULL;
    if(strcmp(filtroTamanho, "sim") == 0) {
        resultadosTamanho = buscaTamanho(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosTamanho, qtdVeiculos);
        resultados = resultadosTamanho;
    }

    printf("Quer utilizar filtro de tipo? (sim/nao): ");
    scanf(" %s", filtroTipo);
    Veiculo **resultadosTipo =  NULL;
    if(strcmp(filtroTipo, "sim") == 0) {
        resultadosTipo = buscaTipo(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosTipo, qtdVeiculos);
        resultados = resultadosTamanho;
    }

    printf("Quer utilizar filtro de cor? (sim/nao): ");
    scanf(" %s", filtroCor);
    Veiculo **resultadosCor = NULL;
    if(strcmp(filtroCor, "sim") == 0) {
        resultadosCor = buscaCor(dados, &qtdVeiculos);
        listarVeiculosEncontrados(resultadosCor, qtdVeiculos);
        resultados = resultadosTamanho;
    }
    
    int codigoVeiculo = 0;
    listarVeiculosEncontrados(resultados, qtdVeiculos);
    printf("Selecione por codigo qual veiculo deseja: \n");
    scanf("%d", &codigoVeiculo);

    char veiculosEstoque[50] = "veiculos_estoque.csv";
    char veiculosOfertas[50] = "veiculos_ofertas.csv";
    char historicoDeCompras[50] = "historico_compras.csv";

    char novoValor[1000];
    printf("Digite o valor de venda pretendido: ");
    scanf(" %[^\n]", novoValor);

    bool retorno = reescreverArquivos(dados, codigoVeiculo, qtdVeiculos, veiculosEstoque, veiculosOfertas, historicoDeCompras, novoValor);

    for (int i = 0; i < qtdVeiculos; i++) {
        free(dados[i]);
    }
    free(dados);

    fclose(arq);

    return 0;
}

int registrarVenda() {
    char estoqueVeiculos[128] = "veiculos_estoque.csv";
    int qtdVeiculos;
    Veiculo **dados;
    Veiculo **dadosEstoque;
    char marcaVeiculo[128];

    time_t currentTime;
    time(&currentTime);
    char *dateTimeString = ctime(&currentTime);
    dateTimeString[strcspn(dateTimeString, "\r\n")] = 0;

    printf("Digite a marca do veiculo que quer vender: ");
    scanf(" %[^\n]", marcaVeiculo);
    fflush(stdin);

    leArquivo(estoqueVeiculos, &dados, &qtdVeiculos);

    int codigoInterno[qtdVeiculos];
    bool encontrou = false;
    int qtdVeicPorMarca = 0;
    for (int i = 0; i < qtdVeiculos; i++) {
        if (strcmp(marcaVeiculo, dados[i]->marca) == 0) {
            encontrou = true;
            codigoInterno[qtdVeicPorMarca] = i;
            qtdVeicPorMarca++;
        } else if (encontrou && strcmp(marcaVeiculo, dados[i]->marca) != 0) {
            break;
        }

    }

    if (!encontrou) {
        printf("\nMarca %s nao encontrada", marcaVeiculo);
        return 1;
    }

    for (int i = 0; i < qtdVeicPorMarca; i++) {
        printf("Codigo: %d|", codigoInterno[i]);
        printf("%-40s| ", dados[codigoInterno[i]]->preco);
        printf("%-40s| ", dados[codigoInterno[i]]->ano);
        printf("%-40s| ", dados[codigoInterno[i]]->marca);
        printf("%-40s| ", dados[codigoInterno[i]]->modelo);
        printf("%-40s| ", dados[codigoInterno[i]]->cor);
        printf("\n");
    }

    int codigoVeiculo = 0;
    printf("Selecione por codigo qual veiculo deseja: \n");
    scanf("%d", &codigoVeiculo);

    leArquivo(estoqueVeiculos, &dadosEstoque, &qtdVeiculos);

    FILE *arquivo2;
    char historicoVendas[138] = "historico_vendas.csv";
    if (access(historicoVendas, F_OK) == -1) {
        arquivo2 = fopen(historicoVendas, "a+");
        fprintf(arquivo2,"venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor, data\n");
    }else{
        arquivo2 = fopen(historicoVendas, "a+");
    }

    for (int i = 0; i < qtdVeiculos; i++) {
        if (i == codigoVeiculo) {
            fprintf(arquivo2, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", dados[i]->preco, dados[i]->ano, dados[i]->marca,
                    dados[i]->modelo, dados[i]->condicao, dados[i]->combustivel, dados[i]->odometro,
                    dados[i]->status, dados[i]->cambio, dados[i]->tamanho, dados[i]->tipo, dados[i]->cor, dateTimeString);

        }
    }

    FILE *arquivo = fopen("veiculos_estoque.csv", "r");
    FILE *arquivo_temp = fopen("veiculos_estoque_temp.csv", "w");

    if (arquivo == NULL || arquivo_temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    char linha[1000];
    int contador = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (contador != codigoVeiculo) {
            fprintf(arquivo_temp, "%s", linha);
        }
        contador++;
    }

    fclose(arquivo);
    fclose(arquivo_temp);

    remove("veiculos_estoque.csv");
    rename("veiculos_estoque_temp.csv", "veiculos_estoque.csv");
    
    return 0;
}

void salvarTaxas() {
    char veiculos[128] = "veiculos_ofertas.csv";
    char marcas[128] = "marcas.csv";
    FILE *arquivoMarcas;
    FILE *arquivoVeiculos;

    arquivoMarcas = fopen(marcas, "a+");
    arquivoVeiculos = fopen(veiculos, "a+");

    if (arquivoVeiculos == NULL || arquivoMarcas == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    char linha[1000];
    Marca *marcasArray[100];  
    int qtdMarcas = 0;

    while (fgets(linha, sizeof(linha), arquivoVeiculos) != NULL) {
        linha[strcspn(linha, "\r\n")] = 0;

        Veiculo veiculo;
        sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
               veiculo.preco, veiculo.ano, veiculo.marca, veiculo.modelo, veiculo.condicao, veiculo.combustivel,
               veiculo.odometro, veiculo.status, veiculo.cambio, veiculo.tamanho, veiculo.tipo, veiculo.cor);

        int marcaExistente = 0;
        for (int i = 0; i < qtdMarcas; i++) {
            if (strcmp(veiculo.marca, marcasArray[i]->nome) == 0) {
                marcaExistente = 1;
                break;
            }
        }

        if (!marcaExistente) {
            Marca *novaMarca = malloc(sizeof(Marca));
            strcpy(novaMarca->nome, veiculo.marca);
            novaMarca->taxa = 0.0;  
            marcasArray[qtdMarcas++] = novaMarca;
        }
    }

    char marcaDesejada[100];
    printf("Digite a marca desejada: ");
    scanf("%s", marcaDesejada);

    int indiceMarca = -1;
    for (int i = 0; i < qtdMarcas; i++) {
        if (strcmp(marcaDesejada, marcasArray[i]->nome) == 0) {
            indiceMarca = i;
            break;
        }
    }

    if (indiceMarca != -1) {
        printf("Digite a nova taxa para a marca %s: ", marcaDesejada);
        scanf("%f", &marcasArray[indiceMarca]->taxa);
    } else {
        printf("Marca não encontrada.\n");
    }

    for (int i = 0; i < qtdMarcas; i++) {
        fprintf(arquivoMarcas, "%s,%.2f\n", marcasArray[i]->nome, marcasArray[i]->taxa);
    }

    fclose(arquivoVeiculos);
    fclose(arquivoMarcas);

    for (int i = 0; i < qtdMarcas; i++) {
        free(marcasArray[i]);
    }
}

void listarVeiculos(Veiculo **dados, int qtdVeiculos) {
    char marcaDesejada[100];
    
    printf("Digite a marca desejada para filtrar os veiculos: ");
    scanf(" %[^\n]", marcaDesejada);

    printf("Veiculos disponiveis da marca %s:\n", marcaDesejada);
    for (int i = 0; i < qtdVeiculos; i++) {
        if (strcmp(dados[i]->marca, marcaDesejada) == 0) {
            printf("%d | ", i);
            printf("%s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s\n", 
                dados[i]->preco, dados[i]->ano, dados[i]->marca, dados[i]->modelo,
                dados[i]->condicao, dados[i]->combustivel, dados[i]->odometro,
                dados[i]->status, dados[i]->cambio, dados[i]->tamanho,
                dados[i]->tipo, dados[i]->cor);
        }
    }
}


void alterarAtributos() {
    char opcao[2];
    char arquivo[50];
    char atributo[50];
    char novoValor[1000];

    printf("Deseja alterar de estoque_veiculos.csv (1) ou veiculos_ofertas.csv (2)? ");
    scanf("%s", opcao);

    if (strcmp(opcao, "1") == 0) {
        strcpy(arquivo, "estoque_veiculos.csv");
    } else if (strcmp(opcao, "2") == 0) {
        strcpy(arquivo, "veiculos_ofertas.csv");
    } else {
        printf("Opcao invalida.\n");
        return;
    }

    Veiculo **dados;
    int qtdVeiculos;
    
    leArquivo(arquivo, &dados, &qtdVeiculos);
    listarVeiculos(dados, qtdVeiculos);

    int codigoVeiculo;
    printf("Digite o codigo do veiculo que deseja alterar: ");
    scanf("%d", &codigoVeiculo);
    
    printf("Atributos disponiveis para alteracaoo:\n");
    printf("Preco\n");
    printf("Ano\n");
    printf("Marca\n");
    printf("Modelo\n");
    printf("Condicão\n");
    printf("Combustivel\n");
    printf("Odometro\n");
    printf("Status\n");
    printf("Cambio\n");
    printf("Tamanho\n");
    printf("Tipo\n");
    printf("Cor\n");

    printf("Digite o atributo que deseja alterar: ");
    scanf("%s", atributo);
    int indiceVeiculo = codigoVeiculo;

    printf("Digite o novo valor para %s: ", atributo);
    scanf(" %[^\n]", novoValor);
    char atributoLower[50];
    for (int i = 0; i < strlen(atributo); i++) {
        atributoLower[i] = tolower(atributo[i]);
    }
    atributoLower[strlen(atributo)] = '\0';

    if (indiceVeiculo >= 0 && indiceVeiculo < qtdVeiculos) {
        if (strcmp(atributoLower, "preco") == 0) {
            strcpy(dados[indiceVeiculo]->preco, novoValor);
        } else if (strcmp(atributoLower, "ano") == 0) {
            strcpy(dados[indiceVeiculo]->ano, novoValor);
        } else if (strcmp(atributoLower, "marca") == 0) {
            strcpy(dados[indiceVeiculo]->marca, novoValor);
        } else if (strcmp(atributoLower, "modelo") == 0) {
            strcpy(dados[indiceVeiculo]->modelo, novoValor);
        } else if (strcmp(atributoLower, "condicao") == 0) {
            strcpy(dados[indiceVeiculo]->condicao, novoValor);
        } else if (strcmp(atributoLower, "combustivel") == 0) {
            strcpy(dados[indiceVeiculo]->combustivel, novoValor);
        } else if (strcmp(atributoLower, "odometro") == 0) {
            strcpy(dados[indiceVeiculo]->odometro, novoValor);
        } else if (strcmp(atributoLower, "status") == 0) {
            strcpy(dados[indiceVeiculo]->status, novoValor);
        } else if (strcmp(atributoLower, "cambio") == 0) {
            strcpy(dados[indiceVeiculo]->cambio, novoValor);
        } else if (strcmp(atributoLower, "tamanho") == 0) {
            strcpy(dados[indiceVeiculo]->tamanho, novoValor);
        } else if (strcmp(atributoLower, "tipo") == 0) {
            strcpy(dados[indiceVeiculo]->tipo, novoValor);
        } else if (strcmp(atributoLower, "cor") == 0) {
            strcpy(dados[indiceVeiculo]->cor, novoValor);
        } else {
            printf("Atributo invalido.\n");
            return;
        }
        

        FILE *arq = fopen(arquivo, "w");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo para escrita.\n");
            return;
        }

        fprintf(arq, "preco,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");

        for (int i = 0; i < qtdVeiculos; i++) {
            fprintf(arq, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", dados[i]->preco, dados[i]->ano, dados[i]->marca,
                    dados[i]->modelo, dados[i]->condicao, dados[i]->combustivel, dados[i]->odometro,
                    dados[i]->status, dados[i]->cambio, dados[i]->tamanho, dados[i]->tipo, dados[i]->cor);
        }

        fclose(arq);

        printf("Atributo alterado com sucesso. Segue veiculo com novos valores\n");
        printf("%s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s\n", 
                dados[indiceVeiculo]->preco, dados[indiceVeiculo]->ano, dados[indiceVeiculo]->marca, dados[indiceVeiculo]->modelo,
                dados[indiceVeiculo]->condicao, dados[indiceVeiculo]->combustivel, dados[indiceVeiculo]->odometro,
                dados[indiceVeiculo]->status, dados[indiceVeiculo]->cambio, dados[indiceVeiculo]->tamanho,
                dados[indiceVeiculo]->tipo, dados[indiceVeiculo]->cor);
    } else {
        printf("Codigo de veiculo invalido.\n");
    }


    printf("Veiculos disponiveis apos a alteracão:\n");
    listarVeiculos(dados, qtdVeiculos);


    for (int i = 0; i < qtdVeiculos; i++) {
        free(dados[i]);
    }
    free(dados);
}

int main()
{
    int opc = 0;
    FILE *arq = NULL;
    char veiculosOfertas[50] = "veiculos_ofertas.csv";

    do {
        printf(" ________________________________________________\n");
        printf("|Escolha uma questao                             |\n");
        printf("|________________________________________________|\n");
        printf("|                                                |\n");
        printf("|0 - Zero para sair                              |\n");
        printf("|1 - QUESTAO 1 - Comprar Veiculos                |\n");
        printf("|2 - QUESTAO 2 - Vender Veiculo                  |\n");
        printf("|3 - QUESTAO 3 - Dados de peso                   |\n");
        printf("|4 - QUESTAO 4 - Alterar atributo de veiculo     |\n");
        printf("|5 - QUESTAO 5 - Compara com arquivo 2           |\n");
        printf("|6 - QUESTAO 6 - Backup do arquivo 1             |\n");
        printf("|7 - QUESTAO 7 - Backup binario                  |\n");
        printf("|8 - QUESTAO 8 - Struct De pessoa                |\n");
        printf("|9 - QUESTAO 9 - Struct De materia               |\n");
        printf("|10 - QUESTAO 10 - Imprime todos os dados arq 1  |\n");
        printf("|________________________________________________|\n");
        scanf("%d", &opc);
    }while(opc <0 || opc > 10);

    switch(opc){
        case 1:
            compraVeiculos(arq, veiculosOfertas);
            break;
        case 2:
            registrarVenda();
            break;
        case 3:
            salvarTaxas();
            break;
        case 4:
            alterarAtributos();
            break;
        // case 5:
        //     imprimeFelinos(arq, arquivo1, arquivo2);
        //     break;
        // case 6:
        //     backupArquivo(arq, arquivo1, arquivo2);
        //     break;
        // case 7:
        //     backupArquivoBinario(arq, arquivo1, arquivo2);
        //     break;
        // case 8:
        //     lePessoa();
        //     break;
        // case 9:
        //     MediaAlunos();
        //     break;
        // case 10:
        //     imprimeDadosArquivo(arq, arquivo1);
        //     break;
    }

}