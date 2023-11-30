#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char ***leArquivo(char arquivo[], int *linhas, int *colunas){
    FILE *arq = NULL;
    char linha[1000];
    int tamanhoMaximo = 10000000;
    int qtdItensPorLinha = 12;
    int contador;

    arq = fopen(arquivo, "a");

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

int compraVeiculos(FILE *arq,  char arquivo[]){

    int tamanhoMaximo = 10000;
    char ***dados = (char***)malloc(tamanhoMaximo * sizeof(char**));
    int linhas = 0, colunas =0;
    char veiculoComprado[60];
    char marcaVeiculo[60];
    int qtdVeicPorMarca=0;

    printf("Digite a marca do veiculo que quer comprar: ");
    scanf(" %[^\n]", &marcaVeiculo); 
    fflush(stdin);

    dados = leArquivo(arquivo, &linhas, &colunas);
    int *codigoInterno = (int * )malloc(tamanhoMaximo * sizeof(int *));
    for(int i = 1; i <= linhas;i++){
        if(strcmp(marcaVeiculo, dados[i][2])==0){
            qtdVeicPorMarca++;
            codigoInterno[qtdVeicPorMarca] = i; 
        }else if(strcmp(marcaVeiculo, dados[i-1][2])==0) {
            printf("\nMarca %s nao encontrada", marcaVeiculo);
            break;
        }
    }

    printf("\n\n\n\n");

    for(int i = 1; i <= qtdVeicPorMarca;i++){
        printf("Codigo: %d|", codigoInterno[i]);
        for(int j = 0; j< colunas; j++){
            printf("%-40s| ", dados[i][j]); 
        }
        printf("\n");
    }

    int codigoVeiculo = 0;
    printf("Selecione por codigo qual veiculo deseja: \n");
    scanf("%d", &codigoVeiculo);

    FILE *arq2 = NULL;
    FILE *arqTmp = NULL;


    arqTmp = fopen(arquivo, "w");
    char veiculos_estoque[50] = "veiculos_estoque.csv";
    char veiculosOfertas[50] = "veiculos_ofertas.csv";
    remove(veiculosOfertas);

    arq2 = fopen(veiculos_estoque, "w");
    if(arq2 == NULL){
        printf("\n Nao foi possivel abrir arquivo\n");
        return 1;
    }
    printf("Chega aq");
    return 0;
    fprintf(arq2,"preco,ano,marca,modelo,condicao,combustivel,odometro,status,cambio,tamanho,tipo,cor\n");
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(i == codigoVeiculo){
                fprintf(arq2,"%s", dados[i][j]);
                fprintf(arq2, ",");
            }else{
                fprintf(arqTmp,"%s", dados[i][j]);
                fprintf(arqTmp, ",");
            }
        }
        fprintf(arq2, "\n");
    }
    

    free(arq);
    free(arq2);
    free(arqTmp);

    free(dados);
    free(dados);
    fclose(arq);
    fclose(arq2);
    fclose(arqTmp);

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