#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

int main()
{
    int opcao, *conf, pontos[4], numEquipes, i, j, flag = 0;
    char sair;
    FILE *file;

    // Imprime a tela inicial e le as configuracoes
    opcao = telaInicial();
    conf = lerConfiguracao();
    int numJogos = conf[0]-6, tabela[numJogos];
    numEquipes = conf[1];

    // Aloca a memoria e armazena na tabela de equipes
    int **equipes = (int**) malloc(numEquipes*sizeof(int*));
    for(i=0;i<numEquipes;i++)
        equipes[i] = (int*) malloc(4*sizeof(int));

    for(i=0;i<numEquipes;i++)
    {
        for(j=0;j<4;j++)
        {
            if(j==0)
                equipes[i][j] = i+1;
            else
                equipes[i][j] = 0;
        }
    }

    // Aloca a memoria para o vetor de finalistas
    int *finalistas = (int*) malloc(4*sizeof(int));
    for(i=0;i<4;i++)
        finalistas[i] = 0;

    // Armazena as pontuacoes
    for(i=2;i<6;i++)
        pontos[i-2] = conf[i];

    // Armazena a tabela de jogos
    for(i=6;i<conf[0];i++)
        tabela[i-6] = conf[i];

    // Inicia o programa conforme a opcao selecionada
    switch(opcao)
    {
        case 1:
        jogosGrupo(tabela, numJogos);
        faseGrupos(equipes, tabela, pontos, numJogos, numEquipes);
        tabelaGrupos(equipes, numEquipes);
        jogosEliminatoria(equipes);
        faseEliminatoria(equipes, pontos, finalistas);
        jogosFinal(finalistas);
        faseFinal(equipes, pontos, finalistas);
        tabelaFinal(equipes, finalistas, numEquipes);
        relatorioFinal(equipes, finalistas, numEquipes);
        flag = 1;
        break;

        case 2:
        do
        {
            // Mostra o menu de configuracoes e realiza as operacoes
            opcao = telaConfiguracao();
            switch(opcao)
            {
                case 1:
                conf = lerConfiguracao();
                confEscreve(conf);
                break;

                case 2:
                file = fopen("conf.txt", "w");
                confPontuacao(file, conf);
                fclose(file);
                break;

                case 3:
                file = fopen("conf.txt", "w");
                confEquipes(file, conf);
                fclose(file);
                break;

                case 4:
                file = fopen("default.txt", "r");
                confPadrao(file);
                fclose(file);
                break;

                case 5:
                break;
            }
        }while(opcao != 5);
        flag = 1;
        break;

        case 3:
        flag = 1;
        break;

        case 4:
        setbuf(stdin, NULL);
        printf("\n\nTem certeza de deseja sair do programa? [s/n] ");
        scanf("%c", &sair);
        setbuf(stdin, NULL);
        if(sair == 'n')
            flag = 1;
        break;
    }

    // Libera a memoria alocada
    free(equipes);
    free(finalistas);
    free(conf);

    // Reinicia o programa
    if(flag == 1)
        return main();

    return 0;
}

