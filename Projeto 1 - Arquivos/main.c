#include <stdio.h>
#include <stdlib.h>

int main() {
    // Declaracao dos ponteiros de arquivo
    FILE *entrada, *inteiros, *reais;
    // Abertura dos arquivos de entrada e saída
    entrada = fopen("entrada.dat", "r");
    inteiros = fopen("inteiros.dat", "w");
    reais = fopen("reais.dat", "w");

    // Verifica se a abertura dos arquivos foi bem-sucedida
    if (entrada == NULL || inteiros == NULL || reais == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return 1;
    }

    // Array para armazenar a linha lida do arquivo de entrada
    char linha[50];

    // Loop para ler o arquivo de entrada linha por linha
    while (fgets(linha, sizeof(linha), entrada)) {
        double numero;
        // Tenta analisar a linha como um numero de ponto flutuante
        if (sscanf(linha, "%lf", &numero) == 1) {
            if (numero == (int)numero) {
                // Eh um número inteiro, calcula o complemento
                int complemento = 1000000 - (int)numero;
                // Escreve o numero inteiro e seu complemento no arquivo "inteiros.dat"
                fprintf(inteiros, "%d %d\n", (int)numero, complemento);
            } else {
                // Eh um numero real, calcula o complemento
                double complemento = 1000000 - numero;
                // Escreve o numero real e seu complemento no arquivo "reais.dat"
                fprintf(reais, "%.6lf %.6lf\n", numero, complemento);
            }
        }
    }

    // Fecha os arquivos abertos
    fclose(entrada);
    fclose(inteiros);
    fclose(reais);

    printf("Arquivos gerados com sucesso!\n");

    return 0;
}

