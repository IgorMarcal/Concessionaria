#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

char ***leArquivo(char arquivo[], int *linhas, int *colunas){
    FILE *arq = NULL;
    char linha[1000];
    int tamanhoMaximo = 10000000;
    int qtdItensPorLinha = 12;
    int contador;

    arq = fopen(arquivo, "r");

    if(arq == NULL){
        printf("Falha ao abrir o arquivo para leitura.\n");
    }

    char **vetorLinhas = (char ** )malloc(tamanhoMaximo * sizeof(char *));
    char ***dados = (char ***)malloc(tamanhoMaximo * sizeof(char **));

    if(vetorLinhas == NULL || dados == NULL){
        printf("Erro ao alocar memoria.\n");
        fclose(arq);
    }

    int contaLinhas = 0;
    while (!feof(arq)){
        fgets(linha, 1000, arq);
        vetorLinhas[contaLinhas] = (char *)malloc(strlen(linha) +1 );
        if(strlen(linha) > 1){
            strcpy(vetorLinhas[contaLinhas], linha);
            contaLinhas++;
        }
    }

    for (int i = 0; i < contaLinhas; i++) {
        dados[i] = (char **)malloc(qtdItensPorLinha * sizeof(char *));
        
    }
    for (int i = 0; i < contaLinhas; i++) {
        char *linha_copiada = strdup(vetorLinhas[i]);
        char *token = strtok(linha_copiada, ",");
        contador = 0;
        while (token != NULL && contador < qtdItensPorLinha) {

            dados[i][contador] = strdup(token);
            contador++;
            token = strtok(NULL, ",");
        }
        free(linha_copiada);
    }

    *linhas  = contaLinhas;
    *colunas = qtdItensPorLinha;

    fclose(arq);
    return dados;

}


bool reescreArquivos(char ***dados[],char arq1[], char arq2[], int codigoVeiculo, int *linhas, int *colunas, char novoValor[]){
    FILE *arquivo1 = NULL;
    FILE *arquivo2 = NULL;
    FILE *arquivo3 = NULL;
    char historicoCompras[138] = "historico_compras.csv";

    char ***ptrDados = *dados;   

    if (access(arq2, F_OK) == -1) {
        arquivo2 = fopen(arq2, "a+");
        fprintf(arquivo2,"venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");
    }else{
        arquivo2 = fopen(arq2, "a+");
    }

    arquivo1 = fopen(arq1, "w");
    fflush(arquivo2);
    fprintf(arquivo1,"preco,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");
    fflush(arquivo1);

    if (access(historicoCompras, F_OK) == -1) {
        arquivo3 = fopen(historicoCompras, "a+");
        fprintf(arquivo3,"venda,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor, dataHora\n");
    }else{
        arquivo3 = fopen(historicoCompras, "a+");
    }

    if(arquivo1 == NULL  || arquivo2 == NULL || arquivo3 == NULL ){
        printf("\n Nao foi possivel abrir arquivo\n");
        return false;
    }
    fflush(arquivo3);


    time_t currentTime;
    time(&currentTime);
    char *dateTimeString = ctime(&currentTime);
    
    int colunaData =(*colunas);
    colunaData++; 
    // Realoca memória para incluir uma coluna adicional apenas para a linha indicada por codigoVeiculo

    // Move os dados da cor do veículo (índice 10) para a nova coluna
ptrDados[codigoVeiculo] = (char **)realloc(ptrDados[codigoVeiculo], colunaData * sizeof(char *));

// Adiciona os dados à nova coluna (último índice)
ptrDados[codigoVeiculo][colunaData - 1] = "NovaColuna";



    char ***novaMatriz = (char ***)malloc(*linhas * sizeof(char **));
    for (int i = 0; i < *linhas; i++) {
        novaMatriz[i] = (char **)malloc(13 * sizeof(char *));
        
        for (int j = 0; j < *colunas; j++) {
            novaMatriz[i][j] = strdup(ptrDados[i][j]);
        }
    }

    
    for (int i = 2; i < *linhas; i++) {
        if (i == codigoVeiculo) {
            for (int k = 1; k < *colunas; k++) {
                if (k == 1) {
                    fprintf(arquivo2, "%s,", novoValor);
                }

                fprintf(arquivo2, "%s", ptrDados[i][k]);

                if (k < *colunas - 1) {
                    fprintf(arquivo2, ",");
                }
            }
    
            for (int m = 1; m < 12; m++) {

                printf("%s,", ptrDados[i][m]);

                if (m == 1) {
                    fprintf(arquivo3, "%s,", novoValor);
                }

                fprintf(arquivo3, "%s", novaMatriz[i][m]);

                if (m < 12) {
                    fprintf(arquivo2, ",");
                    fprintf(arquivo3, ",");
                }
                if(m==12){
                    fprintf(arquivo3, ",%s", time(&currentTime));
                }

            }

        }else{
            for (int j = 0; j < *colunas; j++) {
                fprintf(arquivo1, "%s", ptrDados[i][j]);
                if (j < 11) {
                    fprintf(arquivo1, ",");
                }
            }
            
        }
    }
    
    fclose(arquivo1);
    fclose(arquivo2);
    fclose(arquivo3);

    return true;

}

int compraVeiculos(FILE *arq,  char arquivo[]){

    int tamanhoMaximo = 10000;
    char ***dados = (char***)malloc(tamanhoMaximo * sizeof(char**));
    int linhas = 0, colunas =0;
    char veiculoComprado[60];
    char marcaVeiculo[60];
    int qtdVeicPorMarca=2;
    bool encontrou = false;

    printf("Digite a marca do veiculo que quer comprar: ");
    scanf(" %[^\n]", &marcaVeiculo); 
    fflush(stdin);

    dados = leArquivo(arquivo, &linhas, &colunas);
    int codigoInterno[tamanhoMaximo];
    for(int i = 2; i <= linhas;i++){
        if(strcmp(marcaVeiculo, dados[i][2])==0){
            encontrou = true;
            codigoInterno[qtdVeicPorMarca] = i; 
        }else if (encontrou && strcmp(marcaVeiculo, dados[i][2]) != 0) {
            break;
        }
        qtdVeicPorMarca++;
    }

    if(!encontrou){
        printf("\nMarca %s nao encontrada", marcaVeiculo);
        return 1;
    }


    for(int i = 2; i < qtdVeicPorMarca;i++){
        printf("Codigo: %d|", codigoInterno[i]);
        for(int j = 0; j< colunas; j++){
            printf("%-40s| ", dados[i][j]); 
        }
        printf("\n");
    }

    int codigoVeiculo = 0;
    printf("Selecione por codigo qual veiculo deseja: \n");
    scanf("%d", &codigoVeiculo);

    char veiculosEstoque[50] = "veiculos_estoque.csv";
    char veiculosOfertas[50] = "veiculos_ofertas.csv";
    remove(veiculosOfertas);
    
    char novoValor[1000];
    printf("Digite o valor de venda pretendido: ");
    scanf(" %[^\n]", &novoValor);

    bool retorno = reescreArquivos(&dados, veiculosOfertas, veiculosEstoque, codigoVeiculo, &linhas, &colunas, novoValor);

    free(arq);
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