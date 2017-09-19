#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

void cls()
{
    #ifdef WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int *lerConfiguracao()
{
    int i, n;

    // Abre o arquivo de configuracoes
    FILE *file;
    file = fopen("conf.txt", "r");

    // Verifica se foi aberto
    if(file == NULL)
    {
        setbuf(stdin, NULL);
        printf("Nao foi possivel acessar as configuracoes.\n");
        getchar();
        exit(0);
    }

    // Le do arquivo o numero de dados
    fscanf(file,"%d",&n);

    // Aloca o vetor e armazena as informacoes
    int *dados = (int*) malloc(n*sizeof(int));
    for(i=1;i<n;i++)
        fscanf(file,"%d", &dados[i]);

    // Coloco o tamanho do vetor na posicao zero
    dados[0] = n;

    // Fecha o arquivo
    fclose(file);

    return dados;
}

void cabecalho()
{
    printf("\n\n\n");
    printf("********************************************************************\n");
    printf("**                                                                **\n");
    printf("**                 CAMPEONATO DE MINI MILITIA                     **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n\n");
}

int telaInicial()
{
    int opcao;

    cls();
    cabecalho();
    printf("\n                            MENU\n\n");
    printf("                   1- Iniciar Campeonato\n");
    printf("                   2- Configuracoes\n");
    printf("                   3- Instrucoes do Campeonato OFF\n");
    printf("                   4- Sair do Campeonato\n\n\n");
    printf("Opcao: ");
    scanf("%d",&opcao);

    // verifica se a opcao e valida
    while(opcao < 1 && opcao > 4)
    {
        printf("\nOpcao invalida informe outra: ");
        scanf("%d",&opcao);
    }

    return opcao;
}

void jogosGrupo(int *tabela, int numJogos)
{
    int i;

    cls();
    printf("\n\n\n");
    printf("********************************************************************\n");
    printf("**                                                                **\n");
    printf("**                   TABELA DA FASE DE GRUPOS                     **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n\n");

    for(i=0;i<numJogos;i+=2)
        printf("\t\t   | Equipe %d | vs | Equipe %d |\n\n", tabela[i], tabela[i+1]);


    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void faseGrupos(int **equipes, int *tabela, int *pontos, int numJogos, int numEquipes)
{
    int i, jogadores[2];
    FILE *file;

    // Reseta o arquivo
    file = fopen("relatorio.txt", "w");
    fclose(file);

    for(i=0;i<numJogos;i+=2)
    {
        cls();
        printf("\n********************************************************************\n");
        printf("**                                                                **\n");
        printf("**              CALCULANDO PONTUACAO FASE DE GRUPOS               **\n");
        printf("**                                                                **\n");
        printf("********************************************************************\n\n");

        jogadores[0] = tabela[i];
        jogadores[1] = tabela[i+1];

        file = fopen("relatorio.txt", "a");
        fprintf(file, "             JOGO DA FASE DE GRUPOS\n\n");
        fclose(file);

        partida(jogadores, pontos, equipes, 0);
    }
}

void tabelaGrupos(int **equipes, int numEquipes)
{
    int i, j;

    // Ordena as posicoes em ordem crescente
    for(i=0;i<numEquipes;i++)
    {
        for(j=0;j<numEquipes-1;j++)
        {
            if(equipes[j][1] < equipes[j+1][1])
                trocaPosicao(equipes, j, j+1);
        }
    }

    cls();
    printf("\n********************************************************************\n");
    printf("**                                                                **\n");
    printf("**           TABELA GERAL DO CAMPEONATO MINI MILITIA              **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n");

    printf("\t     Equipes   Pontos   Kills   Acertos");
    for(i=0;i<numEquipes;i++)
    {
        printf("\n\t    -------------------------------------\n");
        printf("\t    |   %d   |   %3d   |   %2d   |   %2d   |", equipes[i][0], equipes[i][1], equipes[i][2], equipes[i][3]);
    }
    printf("\n\t    -------------------------------------\n");
    printf("\n\n");

    relatorioGrupos(equipes, numEquipes);

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void partida(int *jogadores, int *pontos, int **equipes, int desempate)
{
    int i, k, flag, aux, numAcertos[2][3], somaBonus[2], numQuestoes[3], numKill[2], somaAcertos[2], somaPontos[2];
    int numRepescagem[2][2], somaRepescagem[2], somaAcertosRep[2], numAcertosRep[2];

    // Inicia os vetores com zero
     for(i=0;i<2;i++)
     {
         somaPontos[i] = 0;
         somaBonus[i] = 0;
         somaRepescagem[i] = 0;
         somaAcertos[i] = 0;
         somaAcertosRep[i] = 0;
     }

    // Informa as equipes da partida
    printf("\t\t      Equipe %d  vs  Equipe %d\n", jogadores[0], jogadores[1]);

    // Recebe as pontuacoes dos times
    for(i=0;i<2;i++)
    {
        do
        {
            // Inicia com zero os vetores
            flag = 0;
            for(k=0;k<2;k++)
            {
                numRepescagem[i][k] = 0;
                numAcertosRep[k] = 0;
            }
            for(k=0;k<3;k++)
            {
                numAcertos[i][k] = 0;
                numQuestoes[k] = 0;
            }

            if(i == 0)
                k = 1;
            else
                k = 0;

            if(i==1)
                printf("\n\n");

            // Recebe o numero de kills sofridos
            printf("EQUIPE %d\n", jogadores[i]);
            printf("Numero de kills sofridos: ");
            scanf("%d", &numKill[k]);
            while(numKill[k] < 0 || numKill[k] > 5)
            {
                printf("Valor invalido informe outro: ");
                scanf("%d", &numKill[k]);
            }

            // Verifica se e necessario receber as perguntas
            if(numKill[k] > 0)
            {
                // Recebe as questoes do tipo verdadeiro/falso
                printf("\nNumero de Questoes de Verdadeiro ou Falso: ");
                scanf("%d", &numQuestoes[0]);
                while(numQuestoes[0] < 0 || numQuestoes[0] > numKill[k])
                {
                    printf("Valor invalido informe outro: ");
                    scanf("%d", &numQuestoes[0]);
                }
                if(numQuestoes[0] > 0)
                {
                    printf("Acertos: ");
                    scanf("%d", &numAcertos[i][0]);
                    while(numAcertos[i][0] > numQuestoes[0])
                    {
                        printf("Valor invalido informe outro: ");
                        scanf("%d", &numAcertos[i][0]);
                    }
                }

                // Recebe as questoes do tipo alternativa
                if(numQuestoes[0] < numKill[k])
                {
                    printf("\nNumero de Questoes de Alternativa: ");
                    scanf("%d", &numQuestoes[1]);
                    while(numQuestoes[1] < 0 || numQuestoes[1] > numKill[k])
                    {
                        printf("Valor invalido informe outro: ");
                        scanf("%d", &numQuestoes[1]);
                    }
                    if(numQuestoes[1] > 0)
                    {
                        printf("Acertos: ");
                        scanf("%d", &numAcertos[i][1]);
                        while(numAcertos[i][1] > numQuestoes[1])
                        {
                            printf("Valor invalido informe outro: ");
                            scanf("%d", &numAcertos[i][1]);
                        }
                        if(numAcertos[i][1] < numQuestoes[1])
                        {
                            printf("Repescagem: ");
                            scanf("%d", &numRepescagem[i][0]);
                            while((numAcertos[i][1] + numRepescagem[i][0]) > numQuestoes[1])
                            {
                                printf("Valor invalido informe outro: ");
                                scanf("%d", &numRepescagem[i][0]);
                            }
                            if(numRepescagem[i][0] > 0)
                            {
                                printf("Acertos: ");
                                scanf("%d", &numAcertosRep[0]);
                                while(numAcertosRep[0] > numRepescagem[i][0] || numAcertosRep[0] < 0)
                                {
                                    printf("Valor invalido informe outro: ");
                                    scanf("%d", &numAcertosRep[0]);
                                }
                            }
                        }
                    }
                }

                // Recebe as questoes do tipo aberto
                if((numQuestoes[0]+numQuestoes[1]) < numKill[k])
                {
                    printf("\nNumero de Questoes do tipo Aberto: ");
                    scanf("%d", &numQuestoes[2]);
                    while(numQuestoes[2] < 0 || numQuestoes[2] > numKill[k])
                    {
                        printf("Valor invalido informe outro: ");
                        scanf("%d", &numQuestoes[2]);
                    }
                    if(numQuestoes[2] > 0)
                    {
                        printf("Acertos: ");
                        scanf("%d", &numAcertos[i][2]);
                        while(numAcertos[i][2] > numQuestoes[2])
                        {
                            printf("Valor invalido informe outro: ");
                            scanf("%d", &numAcertos[i][2]);
                        }
                        if(numAcertos[i][2] < numQuestoes[2])
                        {
                            printf("Repescagem: ");
                            scanf("%d", &numRepescagem[i][1]);
                            while((numAcertos[i][2] + numRepescagem[i][1]) > numQuestoes[2])
                            {
                                printf("Valor invalido informe outro: ");
                                scanf("%d", &numRepescagem[i][1]);
                            }
                            if(numRepescagem[i][1] > 0)
                            {
                                printf("Acertos: ");
                                scanf("%d", &numAcertosRep[1]);
                                while(numAcertosRep[1] > numRepescagem[i][1] || numAcertosRep[1] < 0)
                                {
                                    printf("Valor invalido informe outro: ");
                                    scanf("%d", &numAcertosRep[1]);
                                }
                            }
                        }
                    }
                }

                // Verifica  se os dados estao corretos
                if((numQuestoes[0] + numQuestoes[1] + numQuestoes[2]) != numKill[k])
                {
                    printf("\n____________________________________________________________________\n");
                    printf("O numero de questoes devem iguais ao numero de kill sofridos\n");
                    printf("Informe os dados da Equipe %d novamente.\n", jogadores[i]);
                    flag = 1;
                }
                else
                {
                    somaPontos[k] += numKill[k] * pontos[3];

                    // Pontos da questao de verdadeiro/falso
                    somaAcertos[i] += numAcertos[i][0];
                    somaPontos[i] += numAcertos[i][0] * pontos[0];
                    somaBonus[k] += (numQuestoes[0] -  numAcertos[i][0]) * pontos[2];

                    // Repescagem Alternativa
                    somaRepescagem[i] += numAcertosRep[0] * (pontos[1]/2);
                    somaRepescagem[k] += (numRepescagem[i][0] - numAcertosRep[0]) * (pontos[1]/2);
                    somaAcertosRep[i] += numAcertosRep[0];
                    // Pontos Alternativa
                    somaPontos[i] += numAcertos[i][1] * pontos[1];
                    numAcertos[i][1] += numAcertosRep[0];
                    somaAcertos[i] += numAcertos[i][1];
                    somaBonus[k] += (numQuestoes[1] - numAcertos[i][1]) * pontos[1];

                    // Repescagem Aberto
                    somaRepescagem[i] += numAcertosRep[1] * (pontos[2]/2);
                    somaRepescagem[k] += (numRepescagem[i][1] - numAcertosRep[1]) * (pontos[2]/2);
                    somaAcertosRep[i] += numAcertosRep[1];
                    // Pontos Aberto
                    somaPontos[i] += numAcertos[i][2] * pontos[2];
                    numAcertos[i][2] += numAcertosRep[1];
                    somaAcertos[i] += numAcertos[i][2];
                    somaBonus[k] += (numQuestoes[2] - numAcertos[i][2]) * pontos[0];
                }
            }
        }while(flag == 1);
    }

    // Atribui o resultado a tabela de equipes
    equipes[jogadores[0]-1][3] += somaAcertos[0];
    equipes[jogadores[0]-1][2] += numKill[0];
    equipes[jogadores[1]-1][3] += somaAcertos[1];
    equipes[jogadores[1]-1][2] += numKill[1];

    // Calcula o total de pontos
    somaPontos[0] += somaBonus[0];
    somaPontos[0] += somaRepescagem[0];
    somaPontos[1] += somaBonus[1];
    somaPontos[1] += somaRepescagem[1];

    // Verifica o resultado da partida
    if(somaPontos[0] == somaPontos[1])
    {
        if(desempate == 1)
        {
            // Criterio de desempate: Numero de Acertos e Numero de Kill
            if(somaAcertos[1] > somaAcertos[0] || ( numKill[1] > numKill[0] && somaAcertos[0] == somaAcertos[1]))
            {
                aux = jogadores[1];
                jogadores[1] = jogadores[0];
                jogadores[0] = aux;
            }
        }
        else
        {
            printf("\n\n");
            printf("********************************************************************\n");
            printf("**                                                                **\n");
            printf("**                           EMPATE                               **\n");
            printf("**                    EQUIPE %d E EQUIPE %d                         **\n", jogadores[0], jogadores[1]);
            printf("**                       com %3d pontos                           **\n", somaPontos[0]);
            printf("**                                                                **\n");
            printf("********************************************************************\n");

            setbuf(stdin, NULL);
            printf("\nDigite alguma tecla para continuar. ");
            getchar();

            relatorioPartida(jogadores, somaPontos, numAcertos, numRepescagem, somaAcertosRep, numKill, somaBonus, somaRepescagem, -1);
        }
    }
    else
    {
        // Apresenta o vencedor e adiciona o pontos do vencedor na tabela de equipes
        if(somaPontos[0] > somaPontos[1])
        {
            vencedor(jogadores[0], somaPontos[0], jogadores[1], somaPontos[1]);
            equipes[jogadores[0]-1][1] += somaPontos[0] - somaPontos[1];
        }
        else
        {
            vencedor(jogadores[1], somaPontos[1], jogadores[0], somaPontos[0]);
            equipes[jogadores[1]-1][1] += somaPontos[1] - somaPontos[0];

            // Organiza o vencedor na posicao zero do vetor
            aux = jogadores[1];
            jogadores[1] = jogadores[0];
            jogadores[0] = aux;
        }
        relatorioPartida(jogadores, somaPontos, numAcertos, numRepescagem, somaAcertosRep, numKill, somaBonus, somaRepescagem, jogadores[0]);
    }
}

void vencedor(int vencedor, int pVencedor, int perdedor, int pPerdedor)
{
    printf("\n\n");
    printf("********************************************************************\n");
    printf("**                                                                **\n");
    printf("**                          VENCEDOR                              **\n");
    printf("**                          EQUIPE %d                              **\n", vencedor);
    printf("**                       com %3d pontos                           **\n", pVencedor);
    printf("**                                                                **\n");
    printf("********************************************************************\n");
    printf("\nPerdedor:\nGrupo %d com %d pontos\n\n", perdedor, pPerdedor);

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void relatorioPartida(int *jogadores, int *somaPontos, int (*numAcertos)[3], int (*numRepescagem)[2], int *somaAcertosRep, int *numKill, int *somaBonus, int *somaRepescagem, int vencedor)
{
    FILE *file;
    file = fopen("relatorio.txt", "a"); // Aberto em modo de append {apenco}

    if(file == NULL)
        printf("\n\nErro ao fazer o relatorio.\n\n");
    else
    {
        fprintf(file, "\t\t Equipe %d  vs  Equipe %d \n", jogadores[0], jogadores[1]);

        fprintf(file, "\nEQUIPE %d\n", jogadores[0]);
        fprintf(file, "Numero de Kills: %d\n", numKill[0]);
        fprintf(file, "Verdadeiro/Falso: %d\n", numAcertos[0][0]);
        fprintf(file, "Alternativa: %d\n", numAcertos[0][1]);
        fprintf(file, "Aberto: %d\n", numAcertos[0][2]);
        fprintf(file, "Repescagem: %d\n", numRepescagem[0][0]+numRepescagem[0][1]);
        fprintf(file, "Acertos: %d\n", somaAcertosRep[0]);
        fprintf(file, "Bonus adquirido: %d\n", somaBonus[0]);
        fprintf(file, "Pontos de repescagem: %d\n", somaRepescagem[0]);
        fprintf(file, "Total de pontos na partida: %d\n", somaPontos[0]);

        fprintf(file, "\nEQUIPE %d\n", jogadores[1]);
        fprintf(file, "Numero de Kills: %d\n", numKill[1]);
        fprintf(file, "Verdadeiro/Falso: %d\n", numAcertos[1][0]);
        fprintf(file, "Alternativa: %d\n", numAcertos[1][1]);
        fprintf(file, "Aberto: %d\n", numAcertos[1][2]);
        fprintf(file, "Repescagem: %d\n", numRepescagem[1][0]+numRepescagem[1][1]);
        fprintf(file, "Acertos: %d\n", somaAcertosRep[1]);
        fprintf(file, "Bonus adquirido: %d\n", somaBonus[1]);
        fprintf(file, "Pontos de repescagem: %d\n", somaRepescagem[1]);
        fprintf(file, "Total de pontos na partida: %d\n", somaPontos[1]);

        if(vencedor == -1)
            fprintf(file, "\nVENCEDOR DA PARTIDA: EMPATE\n");
        else
            fprintf(file, "\nVENCEDOR DA PARTIDA: EQUIPE %d", vencedor);

        fprintf(file, "\n_____________________________________________________\n");
    }

    fclose(file);
}

void relatorioGrupos(int **equipes, int numEquipes)
{
    int i;
    FILE *file;
    file = fopen("relatorio.txt", "a"); // Aberto em modo de append {apenco}

    fprintf(file, "\n********************************************************************\n");
    fprintf(file, "**                                                                **\n");
    fprintf(file, "**             TABELA DA FASE DE GRUPOS DO CAMPEONATO             **\n");
    fprintf(file, "**                                                                **\n");
    fprintf(file, "********************************************************************\n\n");

    fprintf(file, "\t\t Equipes   Pontos   Kills   Acertos");
    for(i=0;i<numEquipes;i++)
    {
        fprintf(file, "\n\t\t-------------------------------------\n");
        fprintf(file, "\t\t|   %d   |   %3d   |   %2d   |   %2d   |",
                 equipes[i][0], equipes[i][1], equipes[i][2], equipes[i][3]);
    }
    fprintf(file, "\n\t\t-------------------------------------\n");
    fprintf(file, "\n_____________________________________________________\n");
    fclose(file);
}

void jogosEliminatoria(int **equipes)
{
    int i;

    cls();
    printf("\n\n\n");
    printf("********************************************************************\n");
    printf("**                                                                **\n");
    printf("**                TABELA DA FASE ELIMINATORIA                     **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n\n");

    for(i=0;i<4;i+=2)
    {
        printf("\t    ----------------          ----------------\n");
        printf("\t   |    Equipe %d   |   vs   |    Equipe %d   |\n", equipes[i][0], equipes[i+1][0]);
        printf("\t    ----------------          ----------------\n");
    }

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void faseEliminatoria(int **equipes, int *pontos, int *finalistas)
{
    int i, jogadores[2], aux;
    FILE *file;

    for(i=0;i<4;i+=2)
    {
        cls();
        printf("\n********************************************************************\n");
        printf("**                                                                **\n");
        printf("**             CALCULANDO PONTUACAO FASE ELIMINATORIA             **\n");
        printf("**                                                                **\n");
        printf("********************************************************************\n\n");

        jogadores[0] = equipes[i][0];
        jogadores[1] = equipes[i+1][0];

        file = fopen("relatorio.txt", "a");
        fprintf(file, "             JOGO DA FASE ELIMINATORIA\n\n");
        fclose(file);
        partida(jogadores, pontos, equipes, 1);

        finalistas[i] = jogadores[0];
        finalistas[i+1] = jogadores[1];
    }

    // Ordena o vetor na ordem correta
    aux = finalistas[1];
    finalistas[1] = finalistas[2];
    finalistas[2] = aux;
}

void jogosFinal(int *finalistas)
{
    int i;

    cls();
    printf("\n\n\n");
    printf("********************************************************************\n");
    printf("**                                                                **\n");
    printf("**                    TABELA DA FASE FINAL                        **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n\n");

    for(i=0;i<4;i+=2)
    {
        if(i==0)
            printf("\t\t\t Disputa 1º Lugar\n");
        else
            printf("\t\t\t Disputa 3º lugar\n");
        printf("\t    -------------          -------------\n");
        printf("\t   |  Equipe %d   |   vs   |  Equipe %d   |\n", finalistas[i], finalistas[i+1]);
        printf("\t    -------------          -------------\n\n");
    }

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void faseFinal(int **equipes, int *pontos, int *finalistas)
{
    int jogadores[2];
    FILE *file;

    cls();
    printf("\n********************************************************************\n");
    printf("**                                                                **\n");
    printf("**             CALCULANDO PONTUACAO DISPUTA 3 LUGAR               **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n");

    jogadores[0] = finalistas[2];
    jogadores[1] = finalistas[3];

    file = fopen("relatorio.txt", "a");
    fprintf(file, "             JOGO DA FASE FINAL (DISPUTA DO 3 LUGAR)\n\n");
    fclose(file);
    partida(jogadores, pontos, equipes, 1);

    finalistas[2] = jogadores[0];
    finalistas[3] = jogadores[1];

    cls();
    printf("\n********************************************************************\n");
    printf("**                                                                **\n");
    printf("**             CALCULANDO PONTUACAO DISPUTA 1 LUGAR               **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n");

    jogadores[0] = finalistas[0];
    jogadores[1] = finalistas[1];

    file = fopen("relatorio.txt", "a");
    fprintf(file, "             JOGO DA FASE FINAL (DISPUTA DO 1 LUGAR)\n\n");
    fclose(file);
    partida(jogadores, pontos, equipes, 1);

    finalistas[0] = jogadores[0];
    finalistas[1] = jogadores[1];
}

void trocaPosicao(int **equipes, int i, int j)
{
    int aux1, aux2, aux3, aux4;
    aux1 = equipes[i][0];
    aux2 = equipes[i][1];
    aux3 = equipes[i][2];
    aux4 = equipes[i][3];
    equipes[i][0] = equipes[j][0];
    equipes[i][1] = equipes[j][1];
    equipes[i][2] = equipes[j][2];
    equipes[i][3] = equipes[j][3];
    equipes[j][0] = aux1;
    equipes[j][1] = aux2;
    equipes[j][2] = aux3;
    equipes[j][3] = aux4;
}

void tabelaFinal(int **equipes, int *finalistas, int numEquipes)
{
    int i, k;

    cls();
    printf("\n********************************************************************\n");
    printf("**                                                                **\n");
    printf("**           TABELA FINAL DO CAMPEONATO MINI MILITIA              **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n");

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
        {
            if(finalistas[i] == equipes[k][0])
                trocaPosicao(equipes, i, k);
        }
    }

    printf("\t\t Equipes   Pontos   Kills   Acertos");
    for(i=0;i<numEquipes;i++)
    {
        printf("\n\t\t-------------------------------------\n");
        printf("\t\t|   %d   |   %3d   |   %2d   |   %2d   |", equipes[i][0], equipes[i][1], equipes[i][2], equipes[i][3]);
    }
    printf("\n\t\t-------------------------------------\n");

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void relatorioFinal(int **equipes, int *finalistas, int numEquipes)
{
    int i;
    FILE *file;
    file = fopen("relatorio.txt", "a"); // Aberto em modo de append {apenco}

    cls();
    fprintf(file, "\n********************************************************************\n");
    fprintf(file, "**                                                                **\n");
    fprintf(file, "**           TABELA FINAL DO CAMPEONATO MINI MILITIA              **\n");
    fprintf(file, "**                                                                **\n");
    fprintf(file, "********************************************************************\n\n");

    fprintf(file, "         Equipes   Pontos   Kills   Acertos");
    for(i=0;i<numEquipes;i++)
    {
        fprintf(file, "\n        -------------------------------------\n");
        fprintf(file, "        |   %d   |   %3d   |   %2d   |   %2d   |", equipes[i][0], equipes[i][1], equipes[i][2], equipes[i][3]);
    }
    fprintf(file, "\n        -------------------------------------\n");
    fclose(file);
}

int telaConfiguracao()
{
    int opcao;

    cls();
    cabecalho();
    printf("\n                          CONFIGURACOES\n\n");
    printf("                   1- Ver as configuracoes atuais\n");
    printf("                   2- Alterar as pontuacoes\n");
    printf("                   3- Alterar numero de times e tabela de jogos\n");
    printf("                   4- Ulilizar as configuracoes padrao\n");
    printf("                   5- Voltar ao menu inicial\n\n\n");
    printf("Opcao: ");
    scanf("%d",&opcao);
    setbuf(stdin,NULL);

    // verifica se a opcao e valida
    while(opcao < 1 && opcao > 5)
    {
        printf("\nOpcao invalida informe outra: ");
        scanf("%d",&opcao);
        setbuf(stdin,NULL);
    }

    return opcao;
}

void confPontuacao(FILE *file, int *conf)
{
    int i, kill, alternativa, verdadeiro, aberto;
    char confirmacao;

    printf("Infome a pontuacao dos item abaixo:\nKill: ");
    scanf("%d", &kill);
    printf("Alternativa: ");
    scanf("%d", &alternativa);
    printf("Verdadeiro/Falso: ");
    scanf("%d", &verdadeiro);
    printf("Aberto: ");
    scanf("%d", &aberto);

    setbuf(stdin, NULL);
    printf("\nDeseja salvar as configuracoes? [s/n] ");
    scanf("%c", &confirmacao);
    setbuf(stdin, NULL);

    if(confirmacao == 's')
    {
        fprintf(file, "%d\n%d\n", conf[0], conf[1]);
        fprintf(file, "%d\n%d\n%d\n%d\n\n", verdadeiro, alternativa, aberto, kill);

        for(i=6;i<conf[0];i+=2)
            fprintf(file, "%d %d\n", conf[i], conf[i+1]);

        printf("As configuracoes foram salvas.\n");

        setbuf(stdin, NULL);
        printf("\nDigite alguma tecla para continuar. ");
        getchar();
    }
    else
        confAtual(file, conf);
}

void confEquipes(FILE *file, int *conf)
{
    int i=0, k=1, numEquipes, aux1, aux2;
    int *jogos = (int*) malloc((i+2)*sizeof(int));
    char opcao;

    printf("\nInforme o numero de equipes: ");
    scanf("%d", &numEquipes);
    while(numEquipes < 5 || numEquipes < 1)
    {
        setbuf(stdin, NULL);
        printf("Valor invalido. Deseja informar outro? [s/n] ");
        scanf("%c", &opcao);
        setbuf(stdin, NULL);
        if(opcao == 's')
        {
            printf("Informe o numero de equipes: ");
            scanf("%d", &numEquipes);
        }
        else
        {
            setbuf(stdin, NULL);
            printf("\nDigite alguma tecla para continuar. ");
            getchar();
            return;
        }
    }

    printf("\nDigite [0 0] para finalizar a tabela de jogos.");
    printf("\nInforme a nova tabela de jogos\n");
    do
    {
        printf("Jogo %d: ", k);
        scanf("%d %d", &aux1, &aux2);

        if(aux1 != 0 || aux2 != 0)
        {
            if(i>=2)
                jogos = (int*) realloc(jogos, (i+2)*sizeof(int));

            jogos[i] = aux1;
            jogos[i+1] = aux2;
            i+=2;
        }

        k++;
    }while(aux1 != 0 || aux2 != 0);

    setbuf(stdin,NULL);
    printf("\n\nDeseja salvar as alteracoes no jogo? [s/n] ");
    scanf("%c", &opcao);
    setbuf(stdin,NULL);

    if(opcao == 's')
    {
        fprintf(file,"%d\n%d\n", (i+6), numEquipes);
        for(k=2;k<6;k++)
            fprintf(file, "%d\n", conf[k]);

        fprintf(file, "\n");
        for(k=0;k<i;k+=2)
            fprintf(file, "%d %d\n", jogos[k], jogos[k+1]);

        printf("As configuracoes foram salvas.\n");

        setbuf(stdin, NULL);
        printf("\nDigite alguma tecla para continuar. ");
        getchar();
    }
    else
        confAtual(file, conf);

    free(jogos);

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void confEscreve(int *conf)
{
    int i, k = 1;

    cls();
    printf("\n\n\n");
    printf("********************************************************************\n");
    printf("**                                                                **\n");
    printf("**                 CONFIGURACOES DO CAMPEONATO                    **\n");
    printf("**                                                                **\n");
    printf("********************************************************************\n\n\n");

    printf("PONTUACOES\nVerdadeiro/Falso: %d\n", conf[2]);
    printf("Alternativa: %d\nAberto: %d\nKill: %d\n", conf[3], conf[4], conf[5]);

    printf("\nEQUIPES\nNumero de Equipes: %d\n", conf[1]);

    printf("\nTABELA DE JOGOS\n");
    for(i=6;i<conf[0];i+=2)
    {
        printf("Jogo %2d: Equipe %d  vs  Equipe %d\n", k, conf[i], conf[i+1]);
        k++;
    }

    setbuf(stdin, NULL);
    printf("\nDigite alguma tecla para continuar. ");
    getchar();
}

void confAtual(FILE *file, int *conf)
{
    int i;
    for(i=0;i<conf[0];i++)
    {
        if(i < 6)
            fprintf(file, "%d\n", conf[i]);
        else
        {
            if(i == 6)
                fprintf(file, "\n%d %d\n", conf[i],conf[i+1]);
            else
                fprintf(file, "%d %d\n", conf[i],conf[i+1]);
            i++;
        }
    }
    printf("As configuracoes nao foram salvas.\n");
}

void confPadrao(FILE *file)
{
    int i, n;
    char opcao;

    // Verifica se foi aberto
    if(file == NULL)
    {
        setbuf(stdin, NULL);
        printf("Nao foi possivel acessar as configuracoes padrao.\n");
        getchar();
        exit(0);
    }

    // Le do arquivo o numero de dados
    fscanf(file,"%d",&n);

    // Aloca o vetor e armazena as informacoes
    int *padrao = (int*) malloc(n*sizeof(int));
    for(i=1;i<n;i++)
        fscanf(file,"%d", &padrao[i]);

    // Coloco o tamanho do vetor na posicao zero
    padrao[0] = n;

    setbuf(stdin,NULL);
    printf("Deseja ulilizar as configuracoes padrao? [s/n] ");
    opcao = getchar();

    if(opcao == 'n')
        printf("As configuracoes nao foram alteradas.\n\n");
    else
        printf("As configuracoes foram alteradas com sucesso.\n\n");

    getchar();
    printf("\nDigite alguma tecla para continuar. ");
    getchar();

    fclose(file);
    file = fopen("conf.txt", "w");

    for(i=0;i<6;i++)
        fprintf(file, "%d\n", padrao[i]);
    fprintf(file, "\n");
    for(i=6;i<n;i+=2)
        fprintf(file, "%d %d\n", padrao[i], padrao[i+1]);
}
