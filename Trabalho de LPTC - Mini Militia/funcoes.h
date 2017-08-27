#ifndef FUNCOES
#define FUNCOES

// Limpa a tela
void cls();

// Apresenta o cabecalho do jogo
void cabecalho();

// Apresenta a tela inicial do programa
int telaInicial();

// Ler as configuracoes do jogo
int *lerConfiguracao();

// Apresenta a tabela de jogos (fase de grupos)
void jogosGrupo(int *tabela, int numJogos);

// Apresenta a tabela da fase de grupos
void tabelaGrupos(int **equipes, int numEquipes);

// Realiza a fese de grupos
void faseGrupos(int **equipes, int *tabela, int *pontos, int numJogos, int numEquipes);

// Realiza as partidas
void partida(int *jogadores, int *pontos, int **equipes,int desempate);

// Apresenta o vencedor
void vencedor(int vencedor, int pVencedor, int perdedor, int pPerdedor);

// Realiza o relatorio do jogos em um arquivo .txt
void relatorioPartida(int *jogadores, int *somaPontos, int (*numAcertos)[3], int (*numRepescagem)[2], int *somaAcertosRep, int *numKill, int *somaBonus, int *somaRepescagem, int vencedor);

// Adiciona a tabela da fase de grupos em um arquivo .txt
void relatorioGrupos(int **equipes, int numEquipes);

// Apresenta a tabela de jogos das eliminatorias
void jogosEliminatoria(int **equipes);

// Realiza as eliminatorias
void faseEliminatoria(int **equipes, int *pontos, int *finalistas);

// Apresenta os finalistas
void jogosFinal(int *finalistas);

// Realiza a final e disuta 3 lugar
void faseFinal(int **equipes, int *pontos, int *finalistas);

// Apresenta a tabela final do campeonato
void tabelaFinal(int **equipes, int *finalistas, int numEquipes);

// Troca duas posicoes da matriz
void trocaPosicao(int **equipes, int i, int j);

// Escreve a tabela final do campeonato em um arquivo .txt
void relatorioFinal(int **equipes, int *finalistas, int numEquipes);

// Apresenta a tela de configuracao
int telaConfiguracao();

// Apresenta as configuracoes atuais
void confEscreve(int *conf);

// Reescreve as configuracoes do programa
void confPontuacao(FILE *file, int *conf);

// Reescreve as configuracoes de grupos e tabela de equipes
void confEquipes(FILE *file, int *conf);

// Escreve as configuracoes atual no arquivo .txt
void confAtual(FILE *file, int *conf);

// Le as configuracoes padrao
void confPadrao(FILE *file);

#endif