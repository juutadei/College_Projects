#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 30
#define TAM_TITULO 50
#define TAM_DIRETOR 30
#define TAM_PAIS 30
#define TAM_RRN 5
#define TAM_CHAVE 5
#define TAM_ARQUIVO 192

// Registro de dados
struct registro {
    char titulo[TAM_TITULO];
    char titulo_original[TAM_TITULO];
    char diretor[TAM_DIRETOR];
    int ano;
    char pais[TAM_PAIS];
    int nota;
    char chave[TAM_CHAVE];
};

// Structs Indice
struct indice_primario {
    char chave[TAM_CHAVE];
    int rrn;
};

struct indice_secundario {
    char titulo[TAM_TITULO];
    char chave[TAM_CHAVE];
};

int criar_arquivos() {
    int aux = 1;

    FILE *arquivo_movies;
    arquivo_movies = fopen("movies.dat", "r");
    if(arquivo_movies == NULL) {
        FILE *arquivo_movies;
        arquivo_movies = fopen("movies.dat", "w");
        aux = 0;
        fclose(arquivo_movies);
    }
    else {
        fclose(arquivo_movies);
    }

    FILE *arquivo_primaria;
    arquivo_primaria = fopen("iprimary.idx", "a+");
    fclose(arquivo_primaria);

    FILE *arquivo_secundaria;
    arquivo_secundaria = fopen("ititle.idx", "a+");
    fclose(arquivo_secundaria);

    return aux;
}

void listar_filmes() {
    FILE *arquivo_dados = fopen("movies.dat", "r");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados.\n");
        exit(1);
    }

    while(!feof(arquivo_dados)) {
        // Ler o registro do arquivo de dados
        char registro[TAM_ARQUIVO];
        if (fread(registro, TAM_ARQUIVO, 1, arquivo_dados) != 1) {
            fclose(arquivo_dados);
            return;
        }

        char *titulo = strtok(registro, "@");
        char *titulo_original = strtok(NULL, "@");
        char *diretor = strtok(NULL, "@");
        char *ano_str = strtok(NULL, "@");
        char *pais = strtok(NULL, "@");
        char *nota_str = strtok(NULL, "@");
        char *codigo = strtok(NULL, "@");

        // Exibir as informacoes na tela
        if(*(titulo + 0) != 42) {
            printf("Titulo: %s\n", titulo);
            printf("Titulo Original: %s\n", titulo_original);
            printf("Diretor: %s\n", diretor);
            printf("Ano de Lancamento: %s\n", ano_str);
            printf("Pais: %s\n", pais);
            printf("Nota: %s\n\n", nota_str);
        }

        fgetc(arquivo_dados);
    }

    // Fechar o arquivo de dados
    fclose(arquivo_dados);
}

void buscar_titulo(char tituloP[50]) {
    // Abrir o arquivo de dados para leitura
    int num, tamanho;
    char cod[5], aux[50], rrn[5];
    int posicao=0, ok=1;

    for(int i=0;i<50;i++) {
        aux[i] = 0;
    }

    FILE *arquivo_secundario = fopen("ititle.idx", "r");

    fscanf(arquivo_secundario, "%d", &num);
    fgetc(arquivo_secundario);

    for(int i=0;i<num;i++) {
        for(int j=0;j<50;j++) {
            aux[j] = fgetc(arquivo_secundario);

            if(aux[j] == 10) {
                tamanho = j-6;
                break;
            }
        }

        for(int j=0;j<tamanho;j++) {
            if(aux[j]!=tituloP[j]) {
                ok=0;
                break;
            }
        }

        if(ok==1) {
            rrn[0] = aux[tamanho+1];
            rrn[1] = aux[tamanho+2];
            rrn[2] = aux[tamanho+3];
            rrn[3] = aux[tamanho+4];
            rrn[4] = aux[tamanho+5];
        }

        ok=1;
    }

    fclose(arquivo_secundario);

    FILE *arquivo_chave = fopen("iprimary.idx", "r");
    fseek(arquivo_chave, 0, SEEK_SET);

    fscanf(arquivo_chave, "%d", &num);
    fgetc(arquivo_chave);

    for(int j=0;j<num;j++) {
        cod[0] = fgetc(arquivo_chave);
        cod[1] = fgetc(arquivo_chave);
        cod[2] = fgetc(arquivo_chave);
        cod[3] = fgetc(arquivo_chave);
        cod[4] = fgetc(arquivo_chave);
        fgetc(arquivo_chave);
        fscanf(arquivo_chave, "%d", &posicao);
        fgetc(arquivo_chave);

        ok=1;

        for(int z=0;z<5;z++) {
            if(cod[z]!=rrn[z]) {
                ok=0;
                break;
            }
        }

        if(ok==1) {
            posicao = (192*posicao)+posicao;
            break;
        }
    }

    fclose(arquivo_chave);

    FILE *arquivo_dados = fopen("movies.dat", "r");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados.\n");
        exit(1);
    }

    fseek(arquivo_dados, posicao, SEEK_SET);

    // Ler o registro do arquivo de dados
    char registro[TAM_ARQUIVO];
    if (fread(registro, TAM_ARQUIVO, 1, arquivo_dados) != 1) {
        printf("Registro nao encontrado.\n");
        fclose(arquivo_dados);
        return;
    }

    // Extrair os campos individuais do registro
    /*for(int j=0;j<192;j++) {
      char c = fgetc(arquivo_dados);
      if() {

      }
    }*/
    char *titulo = strtok(registro, "@");
    char *titulo_original = strtok(NULL, "@");
    char *diretor = strtok(NULL, "@");
    char *ano_str = strtok(NULL, "@");
    char *pais = strtok(NULL, "@");
    char *nota_str = strtok(NULL, "@");
    char *codigo = strtok(NULL, "@");

    // Exibir as informacoes do registro na tela

    ok = 1;

    for(int i=0;i<5;i++) {
        if(*(codigo+i) != rrn[i]) {
            ok=0;
        }
    }

    if(ok==1 && *(titulo+0)!=42) {
        printf("Titulo: %s\n", titulo);
        printf("Titulo Original: %s\n", titulo_original);
        printf("Diretor: %s\n", diretor);
        printf("Ano de Lancamento: %s\n", ano_str);
        printf("Pais: %s\n", pais);
        printf("Nota: %s\n", nota_str);
        fclose(arquivo_dados);
        return;
    }
    else if(strcmp(codigo, rrn)==0 && *(titulo+0)==42) {
        printf("Filme nao encontrado\n");
        fclose(arquivo_dados);
        return;
    }

    // Fechar o arquivo de dados
    fclose(arquivo_dados);
}

void modificar_nota(char rrn[5]) {
    // Abrir o arquivo de dados para leitura
    FILE *arquivo_chave = fopen("iprimary.idx", "r");
    fseek(arquivo_chave, 0, SEEK_SET);

    int num;
    char cod[5];
    int posicao=0;

    fscanf(arquivo_chave, "%d", &num);
    fgetc(arquivo_chave);

    for(int j=0;j<num;j++) {
        cod[0] = fgetc(arquivo_chave);
        cod[1] = fgetc(arquivo_chave);
        cod[2] = fgetc(arquivo_chave);
        cod[3] = fgetc(arquivo_chave);
        cod[4] = fgetc(arquivo_chave);
        fgetc(arquivo_chave);
        fscanf(arquivo_chave, "%d", &posicao);
        fgetc(arquivo_chave);

        int ok=1;

        for(int z=0;z<5;z++) {
            if(cod[z]!=rrn[z]) {
                ok=0;
                break;
            }
        }

        if(ok==1) {
            posicao = (192*posicao)+posicao;
            break;
        }
    }

    fclose(arquivo_chave);

    FILE *arquivo_dados = fopen("movies.dat", "r+");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados.\n");
        exit(1);
    }

    fseek(arquivo_dados, posicao, SEEK_SET);

    // Ler o registro do arquivo de dados
    char registro[TAM_ARQUIVO];
    if (fread(registro, TAM_ARQUIVO, 1, arquivo_dados) != 1) {
        printf("Registro nao encontrado.\n");
        return;
    }

    // Extrair os campos individuais do registro
    /*for(int j=0;j<192;j++) {
      char c = fgetc(arquivo_dados);
      if() {

      }
    }*/
    char *titulo = strtok(registro, "@");
    char *titulo_original = strtok(NULL, "@");
    char *diretor = strtok(NULL, "@");
    char *ano_str = strtok(NULL, "@");
    char *pais = strtok(NULL, "@");
    char *nota_str = strtok(NULL, "@");
    char *codigo = strtok(NULL, "@");

    // Exibir as informacoes do registro na tela

    fseek(arquivo_dados, posicao, SEEK_SET);

    if(strcmp(codigo, rrn)==0 && *(titulo+0)!=42) {
        char nova_nota[1];
        int aux=0;
        printf("Digite a nova nota: ");
        scanf("%s", nova_nota);
        printf("\nNota alterada");

        while(aux<5) {
            char c = fgetc(arquivo_dados);
            if(c==64) {
                aux++;
            }
        }

        fprintf(arquivo_dados, "%s", nova_nota);
        fclose(arquivo_dados);
        return;
    }
    else if(strcmp(codigo, rrn)==0 && *(titulo+0)==42) {
        printf("Filme nao encontrado\n");
        fclose(arquivo_dados);
        return;
    }

    // Fechar o arquivo de dados
    fclose(arquivo_dados);
}

// Funcao para gerar a chave primÃ¯Â¿Â½ria
char *gerar_chave(struct registro *registro) {
    char *chave = (char *)malloc(TAM_CHAVE), a[3];
    int aux=0, j=2, i=0;
    while(*(registro->diretor+aux)!=32 && *(registro->diretor+aux)!=0) {
        aux++;
    }
    aux--;
    for(int z=2;z>=0;z--) {
        a[z] = *(registro->diretor+aux);
        aux--;
    }
    sprintf(chave, "%s%.2d", a, registro->ano % 100);
    return chave;
}

// Funcao para carregar os indice do disco para a memoria principal
void carregar_indices(struct indice_primario **indice_primario,
                      struct indice_secundario **indice_secundario, int *num_indice_primario, int *num_indice_secundario) {
    // Abrir o arquivo de indice primario
    FILE *arquivo_indice_primario = fopen("iprimary.idx", "r");
    if (arquivo_indice_primario == NULL) {
        printf("Erro ao abrir o arquivo de indice primario.\n");
        exit(1);
    }

    // Ler o cabeÃ¯Â¿Â½alho do arquivo de indice primario
    int num_registros;
    fscanf(arquivo_indice_primario, "%d", &num_registros);
    if(*num_indice_primario != 0) {
        *num_indice_primario = num_registros;
    }

    // Alocar memoria para o indice primario
    *indice_primario = (struct indice_primario *)malloc(sizeof(struct indice_primario) * num_registros);

    // Ler os registros do arquivo de indice primario
    for (int i = 0; i < num_registros; i++) {
        fscanf(arquivo_indice_primario, "%s %d", (*indice_primario)[i].chave,
               &(*indice_primario)[i].rrn);
    }

    // Fechar o arquivo de indice primario
    fclose(arquivo_indice_primario);

    // Abrir o arquivo de indice secundario
    FILE *arquivo_indice_secundario = fopen("ititle.idx", "r+");
    if (arquivo_indice_secundario == NULL) {
        printf("Erro ao abrir o arquivo de indice secundario.\n");
        exit(1);
    }

    // Ler o cabeÃ¯Â¿Â½alho do arquivo de indice secundario
    fscanf(arquivo_indice_secundario, "%d", &num_registros);
    if(*num_indice_secundario != 0) {
        *num_indice_secundario = num_registros;
    }

    // Alocar memoria para o indice secundario
    *indice_secundario = (struct indice_secundario *)malloc(sizeof(struct indice_secundario) * num_registros);

    // Ler os registros do arquivo de indice secundario
    for (int i = 0; i < num_registros; i++) {
        fscanf(arquivo_indice_secundario, "%s %s", (*indice_secundario)[i].titulo,
               (*indice_secundario)[i].chave);
    }

    // Fechar o arquivo de idice secundario
    fclose(arquivo_indice_secundario);
}

// Funcao para salvar os indices na memoria principal para o disco
void salvar_indices(struct indice_primario *indice_primario,
                    struct indice_secundario *indice_secundario, int num_registros_primario, int num_registros_secundario) {
    // Abrir o arquivo de indice primario para escrita
    FILE *arquivo_indice_primario = fopen("iprimary.idx", "r+");
    if (arquivo_indice_primario == NULL) {
        printf("Erro ao abrir o arquivo de indice primario.\n");
        exit(1);
    }

    // Escrever o cabecalho do arquivo de indice primario
    fprintf(arquivo_indice_primario, "%d\n", num_registros_primario);

    fseek(arquivo_indice_primario, 0, SEEK_END);

    // Escrever os registros do arquivo de indice primario
    fprintf(arquivo_indice_primario, "%s %d\n", indice_primario[num_registros_primario-1].chave,
            indice_primario[num_registros_primario-1].rrn);

    // Fechar o arquivo de indice primario
    fclose(arquivo_indice_primario);

    // Abrir o arquivo de indice secundario para escrita
    FILE *arquivo_indice_secundario = fopen("ititle.idx", "r+");
    if (arquivo_indice_secundario == NULL) {
        printf("Erro ao abrir o arquivo de indice secundario.\n");
        exit(1);
    }

    fprintf(arquivo_indice_secundario, "%d\n", num_registros_secundario);

    fseek(arquivo_indice_secundario, 0, SEEK_END);

    fprintf(arquivo_indice_secundario, "%s %s\n", indice_secundario[num_registros_secundario-1].titulo,
            indice_secundario[num_registros_secundario-1].chave);

    fclose(arquivo_indice_secundario);
}

// Funcao para inserir um registro no arquivo de dados e atualizar os indices
void inserir_registro(struct registro *novo_registro, struct indice_primario **indice_primario,
                      struct indice_secundario **indice_secundario, int *num_registros_primario, int *num_registros_secundario) {
    // Abrir o arquivo de dados para leitura e escrita
    FILE *arquivo_dados = fopen("movies.dat", "a+");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados.\n");
        exit(1);
    }

    // Gerar a chave primaria para o novo registro
    strcpy(novo_registro->chave, gerar_chave(novo_registro));

    // Posicionar o cursor no final do arquivo para insercao
    fseek(arquivo_dados, 0, SEEK_END);

    if(strcmp(novo_registro->titulo, novo_registro->titulo_original)==0) {
        strcpy(novo_registro->titulo_original, "idem");
    }

    // Escrever o novo registro no arquivo de dados
    int tamanho = strlen(novo_registro->titulo) + strlen(novo_registro->titulo_original) + strlen(novo_registro->diretor) + strlen(novo_registro->pais) + strlen(novo_registro->chave) + 12;
    fprintf(arquivo_dados, "%s@%s@%s@%d@%s@%d@%s@",
            novo_registro->titulo, novo_registro->titulo_original, novo_registro->diretor,
            novo_registro->ano, novo_registro->pais, novo_registro->nota, novo_registro->chave);

    for(int i=tamanho;i<192;i++) {
        fprintf(arquivo_dados, "#");
    }

    fprintf(arquivo_dados, "\n");

    // Atualizar o indice primario
    *(num_registros_primario)+=1;
    *indice_primario = (struct indice_primario *)realloc(*indice_primario, sizeof(struct indice_primario) * (*num_registros_primario));
    strcpy((*indice_primario)[*num_registros_primario - 1].chave, novo_registro->chave);
    (*indice_primario)[*num_registros_primario - 1].rrn = *num_registros_primario - 1;

    // Atualizar o indice secundario
    (*num_registros_secundario)++;
    *indice_secundario = (struct indice_secundario *)realloc(*indice_secundario, sizeof(struct indice_secundario) * (*num_registros_secundario));
    strcpy((*indice_secundario)[*num_registros_secundario - 1].titulo, novo_registro->titulo);
    strcpy((*indice_secundario)[*num_registros_secundario - 1].chave, novo_registro->chave);

    // Fechar o arquivo de dados
    fclose(arquivo_dados);
}

// Funcao para remover um registro pelo RRN
void remover_registro(char rrn[5], struct indice_primario **indice_primario, int *num_registros_primario) {
    // Abrir o arquivo de dados para leitura
    FILE *arquivo_dados = fopen("movies.dat", "r+");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados.\n");
        exit(1);
    }

    int i=0;

    char *codigo=NULL;

    do {
        // Calcular a posicao no arquivo de dados com base no RRN
        long posicao = 192*i+i;

        // Posicionar o cursor no RRN do registro a ser buscado
        fseek(arquivo_dados, posicao, SEEK_SET);

        // Ler o registro do arquivo de dados
        char registro[TAM_ARQUIVO];
        if (fread(registro, TAM_ARQUIVO, 1, arquivo_dados) != 1) {
            printf("Registro nao encontrado.\n");
            fclose(arquivo_dados);
            return;
        }

        char *titulo = strtok(registro, "@");
        char *titulo_original = strtok(NULL, "@");
        char *diretor = strtok(NULL, "@");
        char *ano_str = strtok(NULL, "@");
        char *pais = strtok(NULL, "@");
        char *nota_str = strtok(NULL, "@");
        codigo = strtok(NULL, "@");

        // Exibir as informacoes do registro na tela

        if(strcmp(codigo, rrn)==0 && *(titulo+0)!=42) {
            fseek(arquivo_dados, posicao, SEEK_SET);
            fprintf(arquivo_dados, "*|");
            printf("Filme excluido!\n");
            fclose(arquivo_dados);
            return;
        }

        i++;
    }while(strcmp(codigo, rrn)!=0);

    // Fechar o arquivo de dados
    fclose(arquivo_dados);
}

// Funcao para buscar um filme por RRN e exibir na tela
void buscar_registro(char rrn[5]) {
    // Abrir o arquivo de dados para leitura
    FILE *arquivo_chave = fopen("iprimary.idx", "r");
    fseek(arquivo_chave, 0, SEEK_SET);

    int num;
    char cod[5];
    int posicao=0;

    fscanf(arquivo_chave, "%d", &num);
    fgetc(arquivo_chave);

    for(int j=0;j<num;j++) {
        cod[0] = fgetc(arquivo_chave);
        cod[1] = fgetc(arquivo_chave);
        cod[2] = fgetc(arquivo_chave);
        cod[3] = fgetc(arquivo_chave);
        cod[4] = fgetc(arquivo_chave);
        fgetc(arquivo_chave);
        fscanf(arquivo_chave, "%d", &posicao);
        fgetc(arquivo_chave);

        int ok=1;

        for(int z=0;z<5;z++) {
            if(cod[z]!=rrn[z]) {
                ok=0;
                break;
            }
        }

        if(ok==1) {
            posicao = (192*posicao)+posicao;
            break;
        }
    }

    fclose(arquivo_chave);

    FILE *arquivo_dados = fopen("movies.dat", "r");
    if (arquivo_dados == NULL) {
        printf("Erro ao abrir o arquivo de dados.\n");
        exit(1);
    }

    fseek(arquivo_dados, posicao, SEEK_SET);

    // Ler o registro do arquivo de dados
    char registro[TAM_ARQUIVO];
    if (fread(registro, TAM_ARQUIVO, 1, arquivo_dados) != 1) {
        printf("Registro nao encontrado.\n");
        fclose(arquivo_dados);
        return;
    }

    char *titulo = strtok(registro, "@");
    char *titulo_original = strtok(NULL, "@");
    char *diretor = strtok(NULL, "@");
    char *ano_str = strtok(NULL, "@");
    char *pais = strtok(NULL, "@");
    char *nota_str = strtok(NULL, "@");
    char *codigo = strtok(NULL, "@");

    // Exibir as informacoes do registro na tela

    if(strcmp(codigo, rrn)==0 && *(titulo+0)!=42) {
        printf("Titulo: %s\n", titulo);
        printf("Titulo Original: %s\n", titulo_original);
        printf("Diretor: %s\n", diretor);
        printf("Ano de Lancamento: %s\n", ano_str);
        printf("Pais: %s\n", pais);
        printf("Nota: %s\n", nota_str);
        fclose(arquivo_dados);
        return;
    }
    else if(strcmp(codigo, rrn)==0 && *(titulo+0)==42) {
        printf("Filme nao encontrado\n");
        fclose(arquivo_dados);
        return;
    }

    // Fechar o arquivo de dados
    fclose(arquivo_dados);
}

int main() {
    struct indice_primario *indice_primario = NULL;
    struct indice_secundario *indice_secundario = NULL;
    int num_registros_primario = 1;
    int num_registros_secundario = 1;

    num_registros_primario = criar_arquivos();
    num_registros_secundario = num_registros_primario;

    // Carregar indices do disco para a memoria principal
    carregar_indices(&indice_primario, &indice_secundario, &num_registros_primario, &num_registros_secundario);

    int opcao;
    do {
        printf("\nMENU:\n\n");
        printf("1. Inserir Filme\n");
        printf("2. Remover Filme\n");
        printf("3. Buscar por RRN\n");
        printf("4. Alterar nota\n");
        printf("5. Buscar por nome (pt)\n");
        printf("6. Listar Filmes\n");
        printf("7. Sair\n\n");
        printf("Escolha uma Opcao: ");
        scanf("%d", &opcao);

        printf("\n\n");

        switch (opcao) {
            case 1:
                // Inserir Filme
                struct registro novo_filme;
                printf("Titulo: ");
                scanf(" %[^\n]", novo_filme.titulo); // Permite a leitura de uma linha completa
                printf("Titulo Original: ");
                scanf(" %[^\n]", novo_filme.titulo_original);
                printf("Diretor: ");
                scanf(" %[^\n]", novo_filme.diretor);
                printf("Ano de Lancamento: ");
                scanf("%d", &novo_filme.ano);
                printf("Pais: ");
                scanf(" %[^\n]", novo_filme.pais);
                printf("Nota: ");
                scanf("%d", &novo_filme.nota);

                inserir_registro(&novo_filme, &indice_primario, &indice_secundario, &num_registros_primario, &num_registros_secundario);
                salvar_indices(indice_primario, indice_secundario, num_registros_primario, num_registros_secundario);
                break;
            case 2:
                // Remover Filme
                char rrn_remover[5];
                printf("RRN do Filme a ser Removido: ");
                scanf("%s", rrn_remover);
                remover_registro(&rrn_remover[0], &indice_primario, &num_registros_primario);
                break;
            case 3:
                // Buscar Filme por RRN
                char rrn_buscar[5];
                printf("RRN do Filme a ser Buscado: ");
                scanf("%s", rrn_buscar);
                buscar_registro(rrn_buscar);
                break;
            case 4:
                // Modificar nota do filme
                char rrn_modificar[5];
                printf("RRN do Filme a ser modificado: ");
                scanf("%s", rrn_modificar);
                modificar_nota(rrn_modificar);
                break;
            case 5:
                // Buscar filme por titulo
                char titulo[50];
                printf("Nome do Filme a ser Buscado: ");
                setbuf(stdin, NULL);
                scanf("%[^\n]", titulo);
                buscar_titulo(titulo);
                break;
            case 6:
                // Listar todos os filmes
                listar_filmes();
                break;
            default:
                printf("Invalido!\n");
                break;
        }
    } while (opcao != 7);

    // Liberar memoria alocada
    free(indice_primario);
    free(indice_secundario);

    return 0;
}