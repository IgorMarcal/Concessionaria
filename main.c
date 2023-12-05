#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
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

        sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
               (*dados)[contaLinhas]->preco, (*dados)[contaLinhas]->ano, (*dados)[contaLinhas]->marca,
               (*dados)[contaLinhas]->modelo, (*dados)[contaLinhas]->condicao, (*dados)[contaLinhas]->combustivel,
               (*dados)[contaLinhas]->odometro, (*dados)[contaLinhas]->status, (*dados)[contaLinhas]->cambio,
               (*dados)[contaLinhas]->tamanho, (*dados)[contaLinhas]->tipo, (*dados)[contaLinhas]->cor);
        contaLinhas++;
    }

    *linhas = contaLinhas;

    fclose(arq);
}

bool reescreverArquivos(Veiculo **veiculos, int codigoVeiculo, int qtdVeiculos, FILE *arquivo1, FILE *arquivo2, FILE *arquivo3, char novoValor[]) {
    time_t currentTime;
    time(&currentTime);
    char *dateTimeString = ctime(&currentTime);
    dateTimeString[strcspn(dateTimeString, "\r\n")] = 0;

    fprintf(arquivo2, "venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");
    fprintf(arquivo1, "preco,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");

    char historicoCompras[138] = "historico_compras.csv";
    fprintf(arquivo3, "venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor,dataHora\n");

    for (int i = 2; i < qtdVeiculos; i++) {
        if (i == codigoVeiculo) {
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



    int codigoInterno[tamanhoMaximo];
    for (int i = 2; i <= qtdVeiculos; i++) {
        if (strcmp(marcaVeiculo, dados[i]->marca) == 0) {
            encontrou = true;
            codigoInterno[qtdVeicPorMarca] = i;
        } else if (encontrou && strcmp(marcaVeiculo, dados[i]->marca) != 0) {
            break;
        }
        qtdVeicPorMarca++;
    }

    for (int i = 2; i <= qtdVeiculos; i++) {
        for (int j = 2; j <= qtdVeiculos; i++) {
       
    }
    }

    if (!encontrou) {
        printf("\nMarca %s nao encontrada", marcaVeiculo);
        return 1;
    }

    for (int i = 2; i < qtdVeicPorMarca; i++) {
        printf("Codigo: %d|", codigoInterno[i]);
        printf("%-40s| ", dados[i]->marca);
        printf("%-40s| ", dados[i]->modelo);

        printf("\n");
    }

    int codigoVeiculo = 0;
    printf("Selecione por codigo qual veiculo deseja: \n");
    scanf("%d", &codigoVeiculo);

    char veiculosEstoque[50] = "veiculos_estoque.csv";
    char veiculosOfertas[50] = "veiculos_ofertas.csv";
    char historicoDeCompras[50] = "historico_compras.csv";
    FILE *arq1;
    FILE *arq2;
    FILE *arq3;

    arq1 = fopen('veiculosOfertas',"a+");
    arq2 = fopen('veiculosEstoque',"a+");
    arq3 = fopen('historicoDeCompras',"a+");



    remove(veiculosOfertas);

    char novoValor[1000];
    printf("Digite o valor de venda pretendido: ");
    scanf(" %[^\n]", novoValor);

    bool retorno = reescreverArquivos(&dados, codigoVeiculo, qtdVeiculos, arq1, arq2, arq3, novoValor);

    for (int i = 0; i < qtdVeiculos; i++) {
        free(dados[i]);
    }
    free(dados);

    fclose(arq);

    return 0;
}

int main()
{
    int opc = 0;
    FILE *arq = NULL;
    char veiculosOfertas[50] = "veiculos_ofertas.csv";
    char arquivo2[20] = "arquivo2.txt";


    do {
        printf(" ________________________________________________\n");
        printf("|Escolha uma questao                             |\n");
        printf("|________________________________________________|\n");
        printf("|                                                |\n");
        printf("|0 - Zero para sair                              |\n");
        printf("|1 - QUESTAO 1 - Comprar Veículos                |\n");
        printf("|2 - QUESTAO 2 - Nome cientifico                 |\n");
        printf("|3 - QUESTAO 3 - Dados de peso                   |\n");
        printf("|4 - QUESTAO 4 - Nome Popular                    |\n");
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
        // case 2:
        //     imprimeNomeCientifico(arq, arquivo1);
        //     break;
        // case 3:
        //     dadosDePeso(arq,arquivo1);
        //     break;
        // case 4:
        //     imprimeNomePopular(arq, arquivo1);
        //     break;
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