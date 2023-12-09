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

        sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
            (*dados)[contaLinhas]->preco, (*dados)[contaLinhas]->ano, (*dados)[contaLinhas]->marca,
            (*dados)[contaLinhas]->modelo, (*dados)[contaLinhas]->condicao, (*dados)[contaLinhas]->combustivel,
            (*dados)[contaLinhas]->odometro, (*dados)[contaLinhas]->status, (*dados)[contaLinhas]->cambio,
            (*dados)[contaLinhas]->tamanho, (*dados)[contaLinhas]->tipo, (*dados)[contaLinhas]->cor);

        // printf("Veiculo %d:\n", contaLinhas + 1);
        // printf("Preco: %s\n", (*dados)[contaLinhas]->preco);
        // printf("Ano: %s\n", (*dados)[contaLinhas]->ano);
        // printf("Marca: %s\n", (*dados)[contaLinhas]->marca);

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

int compraVeiculos(FILE *arq, char arquivo[]) {
    Veiculo **dados;
    int qtdVeiculos;
    int tamanhoMaximo = 10000;
    int linhas = 0, colunas = 0;
    char marcaVeiculo[60];
    int qtdVeicPorMarca = 2;
    bool encontrou = false;

    printf("Digite a marca do veiculo que quer comprar: ");
    scanf(" %[^\n]", marcaVeiculo);
    fflush(stdin);

    leArquivo(arquivo, &dados, &qtdVeiculos);


    int codigoInterno[qtdVeiculos];
    for (int i = 1; i <= qtdVeiculos; i++) {
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

    for (int i = 1; i < qtdVeicPorMarca; i++) {
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
    
    printf("Digite a marca desejada para filtrar os veículos: ");
    scanf(" %[^\n]", marcaDesejada);

    printf("Veículos disponíveis da marca %s:\n", marcaDesejada);
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
    
    printf("Atributos disponíveis para alteracaoo:\n");
    printf("Preco\n");
    printf("Ano\n");
    printf("Marca\n");
    printf("Modelo\n");
    printf("Condição\n");
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
        printf("Codigo de veículo invalido.\n");
    }


    printf("Veiculos disponiveis após a alteração:\n");
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