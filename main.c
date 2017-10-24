#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 10

//============================================= Structs ===============================================//
typedef struct Series
{
    char nome[50];
    int quantCurtidasX;
    double fSuporte, fConfianca[TAM];
}series;

typedef struct Pessoas
{
    char nome[50];
}pessoas;

//======================================== Assinatura das Funçoes =====================================//
void lerSeries(series *listaSeries);
void lerPessoas(pessoas *listaPessoas);
void lerGosta(int **matriz);
void suporte(int **matriz, series *listaSeries);
void confianca (int **matriz, series *listaSeries);
void imprimeSeries (series *listaSeries);
void calculo(series *listaSeries);

//======================================== Funçao Principal ===========================================//
int main()
{
    pessoas *listaPessoas = (pessoas*) malloc (TAM * sizeof(pessoas));
    series *listaSeries = (series*) malloc (TAM * sizeof(series));
    
    int **matriz;
    matriz = (int**) malloc (TAM * sizeof(int*));
    for (int i = 0; i < TAM; i ++)
    {
        matriz[i] = (int*) malloc(TAM * sizeof(int));
    }
    for (int lin = 0; lin < TAM; lin++)
    {
        for (int col = 0; col < TAM; col++)
        {
            matriz[lin][col] = 0;
        }
    }
    
    lerSeries(listaSeries);
    lerPessoas(listaPessoas);
    lerGosta(matriz);
    suporte(matriz, listaSeries);
    confianca (matriz, listaSeries);
    //imprimeSeries (listaSeries);
    calculo(listaSeries);
    /*
    for (int lin = 0; lin < TAM; lin++)
    {
        printf("%s\n", listaPessoas[lin].nome);
        for (int col = 0; col < TAM; col++)
        {
            if (matriz[lin][col] == 1)
            {
                printf("%s ", listaSeries[col].nome);
            }
        }
        printf("\n");
    }
    
 */
    return 0;
}

//============================================= Funções ===============================================//
void lerSeries(series *listaSeries)
{
    FILE *arquivo = fopen("Series.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    else
    {
        char buffer[50];
        int i = 0;
        while (!feof(arquivo))
        {
            fgets (buffer, sizeof (buffer), arquivo);
            int cont = strlen(buffer);
            buffer[cont - 1] = '\0'; // Feito para eliminar o '\n' do final de cada nome das series

            strcpy(listaSeries[i].nome, buffer);
            listaSeries[i].quantCurtidasX = 0;
            i++;
        }
    }

    fclose(arquivo);
    /*
    for (int i = 0; i < TAM; i++)
    {
        printf("%s", listaSeries[i].nome);
    }
    printf("\n");
*/
}

void lerPessoas(pessoas *listaPessoas)
{
    FILE *arquivo = fopen("Pessoas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    else
    {
        char buffer[50];
        int i = 0;
        while (!feof(arquivo)) // feito para ler a linha inteira do arquivo
        {
            fgets (buffer, sizeof (buffer), arquivo);
            strcpy(listaPessoas[i].nome, buffer);
            i++;
        }
    }
    fclose(arquivo);
    /*
      for(int i = 0; i < TAM; i++)
      {
          printf("%s", listaPessoas[i].nome);
      }
      printf("\n");
*/
}

void lerGosta(int **matriz)
{
    int linh = 0;
    int col = 0;
    FILE *arquivo = fopen("Gosta.txt", "r");
    rewind(arquivo); // Volta o indicador de posição do fluxo para a posição inicial. Isto é, o começo do arquivo.
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    else
    {
        while ((fscanf(arquivo, "%d%d", &linh, &col))!= EOF)
        {
            matriz[linh][col] = 1;
        }
        fclose(arquivo);
    }
/*
    for (linh = 0; linh < TAM; linh++)
    {
        for (col = 0; col < TAM; col++)
        {
            printf("%d\t", matriz[linh][col]);
        }
        printf("\n");
    }
*/
}

void suporte(int **matriz, series *listaSeries) // 30%
{
    int auxGosta, auxSuporte = 0;
    for (int lin = 0; lin < TAM; lin++)
    {
        auxGosta = 0;
        for (int col = 0; col < TAM; col++)
        {
            if (matriz[lin][col] == 1)
            {
                listaSeries[col].quantCurtidasX++;
                auxGosta = 1;
            }
        }
        if (auxGosta == 0) auxSuporte++;
    }
    if ((TAM - auxSuporte) == 0)
    {
        printf("\n\nNenhum usuario assistiu filme\n\n");
        exit(EXIT_FAILURE); // Para o programa todo.
    }
    double aux = 0;
    for (int i = 0; i < TAM; i++)
    {
        aux = (double) listaSeries[i].quantCurtidasX / (double) (TAM - auxSuporte);
        listaSeries[i].fSuporte = aux * 100;
    }
}

void confianca (int **matriz, series *listaSeries) // 70%
{
    int contLike1 = 0;
    int col1, col2, lin;

    for ( col1 = 0; col1 < TAM; col1++ )
    {
        for ( col2 = 0; col2 < TAM; col2++ )
        {
            if ( col1 == col2)
            {
                listaSeries[col1].fConfianca[col1] = -1;
            }
            else if ( listaSeries[col1].quantCurtidasX > 0 )
            {
                for ( lin = 0; lin < TAM; lin++ )
                {
                    if (( matriz[lin][col1] == 1) && (matriz[lin][col2] == 1)) contLike1++;
                }
                listaSeries[col1].fConfianca[col2] =
                        ((double)contLike1 / (double)listaSeries[col1].quantCurtidasX) * 100;
                contLike1 = 0;
            }
        }
    }
}

void imprimeSeries (series *listaSeries)
{
    for ( int i = 0; i < TAM; i++ )
    {
        printf("%d %s\nSup: %.2lf\n", i + 1, listaSeries[i].nome, listaSeries[i].fSuporte);
        for ( int j = 0; j < TAM; j++ )
        {
            printf("Conf: %.2lf \n", listaSeries[i].fConfianca[j]);
        }
    }
}

void calculo(series *listaSeries)
{
    for ( int i = 0; i < TAM; i++)
    {
        if (listaSeries[i].fSuporte >= 30.00)
        {
            for ( int j = 0; j < TAM; j++)
            {
                if (listaSeries[i].fConfianca[j] >= 70.00)
                {
                    printf("Quem assistiu a serie %s talvez gostaria de assistir a serie %s\n", listaSeries[i].nome, listaSeries[j].nome);

                }
            }
        }
    }
}

