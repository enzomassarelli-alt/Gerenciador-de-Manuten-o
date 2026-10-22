#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* =========================================================================
 * FUNÇÕES DE MANIPULAÇÃO DE LISTA
 * =========================================================================

Lista* criaLista(void) CRIA E INICIALIZA A ESTRUTURA DA LISTA ENCADEADA

int listaVazia(Lista *l) VERIFICA SE A LISTA ESTÁ VAZIA (RETORNA 1 SE VAZIA, 0 CASO CONTRÁRIO)

int codigoEquipamentoValido(char *codEq) VALIDA SE O CÓDIGO DO EQUIPAMENTO SEGUE O PADRÃO (3 LETRAS E 3 DÍGITOS)

int periodoValido(int prioridade, int periodo) VERIFICA SE O PERÍODO RESPEITA O LIMITE DE DIAS DE CADA PRIORIDADE

int nomesIguais(char *n1, char *n2) COMPARA DOIS NOMES IGNORANDO MAIÚSCULAS E MINÚSCULAS

No* criaNo(int codSol, char *codEq, char *nomeEq, int prioridade, int periodo) CRIA DINAMICAMENTE UM NÓ ISOLADO NA MEMÓRIA

No* buscaPorCodigo(Lista *l, int codSol) BUSCA E RETORNA UM NÓ PELO CÓDIGO DE SOLICITAÇÃO (4 DÍGITOS)

No* buscaPorCodigoEquipamento(Lista *l, char *codEq) BUSCA E RETORNA UM NÓ PELO CÓDIGO DO EQUIPAMENTO (EX: ABC123)

No* buscaPorNomeEquipamento(Lista *l, char *nomeEq) BUSCA E RETORNA UM NÓ PELO NOME DO EQUIPAMENTO

int insereFim(Lista *l, int codSol, char *codEq, char *nomeEq, int prioridade, int periodo) INSERE UM NOVO NÓ NO FINAL DA LISTA (ORDEM DE INSERÇÃO) E IMPEDE DUPLICIDADES

int alteraPrioridadePeriodo(Lista *l, int codSol, int novaPrioridade, int novoPeriodo) ATUALIZA A PRIORIDADE E O PERÍODO DE ESPERA DE UMA SOLICITAÇÃO EXISTENTE

int retiraPorCodigo(Lista *l, int codSol) REMOVE UM NÓ ESPECÍFICO PELO CÓDIGO DA SOLICITAÇÃO E LIBERA A MEMÓRIA

void insereOrdemManutencao(Lista *ordem, No *origem) INSERE UM NÓ NUMA LISTA ORDENADA PELO GRAU DE URGÊNCIA (PRIORIDADE -> PERÍODO -> CÓDIGO)

Lista* geraOrdemManutencao(Lista *l) GERA E RETORNA UMA NOVA LISTA ORDENADA POR URGÊNCIA, SEM MODIFICAR A LISTA ORIGINAL

void imprimeNo(No *no) EXIBE OS DADOS FORMATADOS DE APENAS UM NÓ ESPECÍFICO

void imprimeLista(Lista *l) PERCORRE E IMPRIME TODOS OS REGISTOS CADASTRADOS NA LISTA

Lista* liberaLista(Lista *l) LIBERA TODA A MEMÓRIA ALOCADA PELOS NÓS E PELA ESTRUTURA DA LISTA

*/

/* =========================================================================
 * DEFINIÇÃO DAS ESTRUTURAS
 * ========================================================================= */

typedef struct no
{
    int        codigoSolicitacao;
    char       codigoEquipamento[7];  // 3 letras + 3 dígitos + '\0'
    char       nomeEquipamento[21];   // Até 20 caracteres úteis + '\0'
    int        prioridade;            // 1 - Alta, 2 - Média, 3 - Baixa
    int        periodo;               // Dias tolerados
    struct no *prox;
} No;

typedef struct lista
{
    No *inicio;
} Lista;

/* =========================================================================
 * 1. INICIALIZAÇÃO E VERIFICAÇÃO DE ESTADO
 * ========================================================================= */

// Cria e inicializa a lista encadeada
Lista* criaLista(void)
{
    Lista *l = (Lista*) malloc(sizeof(Lista));
    if (l != NULL)
    {
        l->inicio = NULL;
    }
    return l;
}

// Retorna 1 se a lista for nula ou não tiver nós, 0 caso contrário
int listaVazia(Lista *l)
{
    if (l == NULL || l->inicio == NULL)
    {
        return 1;
    }
    return 0;
}

/* =========================================================================
 * 2. FUNÇÕES DE VALIDAÇÃO DE REGRAS DE NEGÓCIO
 * ========================================================================= */

// Valida se o código do equipamento tem 3 letras seguidas de 3 dígitos
int codigoEquipamentoValido(char *codEq)
{
    int i;

    if (codEq == NULL || strlen(codEq) != 6)
    {
        return 0;
    }

    for (i = 0; i < 3; i++)
    {
        if (!isalpha((unsigned char)codEq[i]))
        {
            return 0;
        }
    }

    for (i = 3; i < 6; i++)
    {
        if (!isdigit((unsigned char)codEq[i]))
        {
            return 0;
        }
    }

    return 1;
}

// Verifica se o período respeita o limite de cada prioridade
int periodoValido(int prioridade, int periodo)
{
    if (prioridade == 1)
    {
        return (periodo >= 1 && periodo <= 7);
    }
    if (prioridade == 2)
    {
        return (periodo >= 1 && periodo <= 15);
    }
    if (prioridade == 3)
    {
        return (periodo >= 1 && periodo <= 20);
    }
    return 0;
}

// Compara duas strings ignorando diferenças entre maiúsculas e minúsculas
int nomesIguais(char *n1, char *n2)
{
    int i = 0;

    if (n1 == NULL || n2 == NULL)
    {
        return 0;
    }

    while (n1[i] != '\0' && n2[i] != '\0')
    {
        if (tolower((unsigned char)n1[i]) != tolower((unsigned char)n2[i]))
        {
            return 0;
        }
        i++;
    }

    return (n1[i] == '\0' && n2[i] == '\0');
}

/* =========================================================================
 * 3. ALOCAÇÃO DE NÓ E BUSCAS
 * ========================================================================= */

// Cria dinamicamente um nó isolado
No* criaNo(int codSol, char *codEq, char *nomeEq, int prioridade, int periodo)
{
    No *p = (No*) malloc(sizeof(No));
    if (p == NULL)
    {
        return NULL;
    }

    p->codigoSolicitacao = codSol;
    p->prioridade = prioridade;
    p->periodo = periodo;

    strcpy(p->codigoEquipamento, codEq);
    strcpy(p->nomeEquipamento, nomeEq);

    p->prox = NULL;

    return p;
}

// Busca um nó por código de solicitação (4 dígitos)
No* buscaPorCodigo(Lista *l, int codSol)
{
    if (l == NULL)
    {
        return NULL;
    }

    No *aux = l->inicio;
    while (aux != NULL && aux->codigoSolicitacao != codSol)
    {
        aux = aux->prox;
    }
    return aux;
}

// Busca um nó pelo código do equipamento (Ex: ABC123)
No* buscaPorCodigoEquipamento(Lista *l, char *codEq)
{
    if (l == NULL || codEq == NULL)
    {
        return NULL;
    }

    No *aux = l->inicio;
    while (aux != NULL)
    {
        if (nomesIguais(aux->codigoEquipamento, codEq))
        {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

// Busca um nó pelo nome do equipamento (Ex: cadeira)
No* buscaPorNomeEquipamento(Lista *l, char *nomeEq)
{
    if (l == NULL || nomeEq == NULL)
    {
        return NULL;
    }

    No *aux = l->inicio;
    while (aux != NULL)
    {
        if (nomesIguais(aux->nomeEquipamento, nomeEq))
        {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

/* =========================================================================
 * 4. OPERAÇÕES PRINCIPAIS (INSERÇÃO, ALTERAÇÃO E REMOÇÃO)
 * ========================================================================= */

// Insere sempre no final da lista, preservando a ordem cronológica de inserção e sem duplicidades
int insereFim(Lista *l, int codSol, char *codEq, char *nomeEq, int prioridade, int periodo)
{
    No *novo, *aux;

    if (l == NULL)
    {
        return 0;
    }

    // Verifica duplicidade de Código de Solicitação, Código do Equipamento e Nome
    if (buscaPorCodigo(l, codSol) != NULL ||
        buscaPorCodigoEquipamento(l, codEq) != NULL ||
        buscaPorNomeEquipamento(l, nomeEq) != NULL)
    {
        return 0;
    }

    novo = criaNo(codSol, codEq, nomeEq, prioridade, periodo);
    if (novo == NULL)
    {
        return 0;
    }

    if (l->inicio == NULL)
    {
        l->inicio = novo;
    }
    else
    {
        aux = l->inicio;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }

    return 1;
}

// Atualiza prioridade e período de um nó existente
int alteraPrioridadePeriodo(Lista *l, int codSol, int novaPrioridade, int novoPeriodo)
{
    No *no = buscaPorCodigo(l, codSol);

    if (no == NULL)
    {
        return 0;
    }

    if (!periodoValido(novaPrioridade, novoPeriodo))
    {
        return 0;
    }

    no->prioridade = novaPrioridade;
    no->periodo = novoPeriodo;

    return 1;
}

// Remove nó pelo código de solicitação e libera a memória
int retiraPorCodigo(Lista *l, int codSol)
{
    No *ant, *aux;

    if (listaVazia(l))
    {
        return 0;
    }

    // Remoção do primeiro elemento
    if (l->inicio->codigoSolicitacao == codSol)
    {
        aux = l->inicio;
        l->inicio = l->inicio->prox;
        free(aux);
        return 1;
    }

    ant = l->inicio;
    aux = l->inicio->prox;

    while (aux != NULL && aux->codigoSolicitacao != codSol)
    {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == NULL)
    {
        return 0;
    }

    ant->prox = aux->prox;
    free(aux);

    return 1;
}

/* =========================================================================
 * 5. GERAÇÃO DA ORDEM DE MANUTENÇÃO (ORDENAÇÃO POR URGÊNCIA)
 * ========================================================================= */

// Inserção ordenada por urgência (Prioridade -> Período -> Código)
void insereOrdemManutencao(Lista *ordem, No *origem)
{
    No *novo, *ant, *aux;

    novo = criaNo(origem->codigoSolicitacao, origem->codigoEquipamento,
                  origem->nomeEquipamento, origem->prioridade, origem->periodo);
    if (novo == NULL)
    {
        return;
    }

    if (ordem->inicio == NULL)
    {
        ordem->inicio = novo;
        return;
    }

    aux = ordem->inicio;
    ant = NULL;

    while (aux != NULL &&
           (aux->prioridade < novo->prioridade ||
            (aux->prioridade == novo->prioridade && aux->periodo < novo->periodo) ||
            (aux->prioridade == novo->prioridade && aux->periodo == novo->periodo &&
             aux->codigoSolicitacao < novo->codigoSolicitacao)))
    {
        ant = aux;
        aux = aux->prox;
    }

    if (ant == NULL)
    {
        novo->prox = ordem->inicio;
        ordem->inicio = novo;
    }
    else
    {
        novo->prox = aux;
        ant->prox = novo;
    }
}

// Cria uma nova lista ordenada por urgência sem modificar a lista original
Lista* geraOrdemManutencao(Lista *l)
{
    if (l == NULL)
    {
        return NULL;
    }

    Lista *ordem = criaLista();
    if (ordem == NULL)
    {
        return NULL;
    }

    No *aux = l->inicio;
    while (aux != NULL)
    {
        insereOrdemManutencao(ordem, aux);
        aux = aux->prox;
    }

    return ordem;
}

/* =========================================================================
 * 6. EXIBIÇÃO DE RELATÓRIOS E LIBERAÇÃO DE MEMÓRIA
 * ========================================================================= */

// Exibe os dados formatados de um único nó
void imprimeNo(No *no)
{
    if (no == NULL)
    {
        return;
    }

    char *txtPrioridade;
    if (no->prioridade == 1)
    {
        txtPrioridade = "Alta";
    }
    else if (no->prioridade == 2)
    {
        txtPrioridade = "Media";
    }
    else
    {
        txtPrioridade = "Baixa";
    }

    printf("\t| Solicitacao: %04d | Codigo: %-6s | Equip: %-20s | Prioridade: %d-%-5s | Periodo: %2d dia(s) |\n",
           no->codigoSolicitacao, no->codigoEquipamento, no->nomeEquipamento,
           no->prioridade, txtPrioridade, no->periodo);
}

// Percorre e imprime toda a lista
void imprimeLista(Lista *l)
{
    if (listaVazia(l))
    {
        printf("\n\tNao ha solicitacoes cadastradas no momento.\n");
        return;
    }

    printf("\n\t========================================== RELATORIO DE SOLICITACOES ==========================================\n");
    No *aux = l->inicio;
    while (aux != NULL)
    {
        imprimeNo(aux);
        aux = aux->prox;
    }
    printf("\t===============================================================================================================\n");
}

// Libera todos os nós e a estrutura da lista
Lista* liberaLista(Lista *l)
{
    if (l == NULL)
    {
        return NULL;
    }

    No *q = l->inicio;
    while (q != NULL)
    {
        No *t = q->prox;
        free(q);
        q = t;
    }
    free(l);
    return NULL;
}

#endif /* LISTA_H_INCLUDED */
