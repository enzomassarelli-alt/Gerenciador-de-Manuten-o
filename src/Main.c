#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include "lista.h"

void esvaziar_buffer()
{
    fflush(stdin);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar()
{
    printf("\n\n\tAperte ENTER para continuar...");
    fflush(stdin);
    getchar();
}

void interface_cadastrar(Lista *lista)
{
    int codSol, prioridade, periodo;
    char codEq[20], nomeEq[50];
    int codigoEqValido, nomeEqValido;

    printf("\n\t---------------------------------------------------------");
    printf("\n\t                  CADASTRO DE SOLICITACAO                ");
    printf("\n\t---------------------------------------------------------\n");

    //Leitura e validação do CODIGO DA SOLICITACAO (4 dígitos, sem duplicidade)
    do
    {
        printf("\n\t[?] Codigo da Solicicitacao (1000 - 9999): ");
        if (scanf("%d", &codSol) != 1)
        {
            printf("\t    -> [ERRO] Digite apenas numeros inteiros!\n");
            codSol = 0;
            esvaziar_buffer();
        }
        else
        {
            esvaziar_buffer();
            if (codSol < 1000 || codSol > 9999)
            {
                printf("\t    -> [ERRO] O codigo precisa estar entre 1000 e 9999!\n");
            }
            else
            {
                if (buscaPorCodigo(lista, codSol) != NULL)
                {
                printf("\t    -> [ERRO] Este codigo de solicitacao ja esta cadastrado!\n");
                }
            }
        }
    } while (codSol < 1000 || codSol > 9999 || buscaPorCodigo(lista, codSol) != NULL);

    //Leitura e validação do CODIGO DO EQUIPAMENT0 (3 numeros e 3 letras)
    do
    {
        codigoEqValido = 0;
        printf("\n\t[?] Codigo do Equipamento (Ex: OSC023) : ");

        if (fgets(codEq, sizeof(codEq), stdin) != NULL)
        {
            size_t len = strlen(codEq);
            if (len > 0 && codEq[len - 1] == '\n')
            {
                codEq[len - 1] = '\0'; // Remove a quebra de linha do final
            }
            else
            {
                esvaziar_buffer(); // Limpa o excesso do buffer caso ultrapasse o limite
            }
        }

        if (!codigoEquipamentoValido(codEq))
        {
            printf("\t    -> [ERRO] Formato invalido! Use exatamente 3 letras e 3 numeros.\n");
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                codEq[i] = (char)toupper((unsigned char)codEq[i]);
            }

            if (buscaPorCodigoEquipamento(lista, codEq) != NULL)
            {
                printf("\t    -> [ERRO] O equipamento com codigo '%s' ja esta em manutencao!\n", codEq);
            }
            else
            {
                codigoEqValido = 1;
            }
        }
    } while (!codigoEqValido);
    //Leitura e validação do NOME do Equipamento

    do
    {
        nomeEqValido = 0;
        printf("\n\t[?] Nome do Equipamento (max. 20 car.) : ");

        if (fgets(nomeEq, sizeof(nomeEq), stdin) != NULL)
        {
            size_t len = strlen(nomeEq);
            if (len > 0 && nomeEq[len - 1] == '\n')
            {
                nomeEq[len - 1] = '\0'; // Remove a quebra de linha do final
            }
            else
            {
                esvaziar_buffer(); // Limpa o excesso do buffer caso ultrapasse 50 chars
            }
        }

        if (strlen(nomeEq) == 0)
        {
            printf("\t    -> [ERRO] O nome nao pode ficar vazio!\n");
        }
        else
        {
            if (strlen(nomeEq) > 20)
            {
                printf("\t    -> [ERRO] O nome ultrapassou o limite de 20 caracteres!\n");
            }
            else
            {
                nomeEqValido = 1;
            }
        }
    } while (!nomeEqValido);

    //Leitura e validação da PRIORIDADE
    do
    {
        printf("\n\t[?] Prioridade (1-Alta|2-Media|3-Baixa) : ");
        if (scanf("%d", &prioridade) != 1)
        {
            printf("\t    -> [ERRO] Valor invalido! Digite 1, 2 ou 3.\n");
            prioridade = 0;
            esvaziar_buffer();
        }
        else
        {
            esvaziar_buffer();
            if (prioridade < 1 || prioridade > 3)
            {
                printf("\t    -> [ERRO] Opcao desconhecida. Escolha 1, 2 ou 3!\n");
            }
        }
    } while (prioridade < 1 || prioridade > 3);

    int limite;
    if (prioridade == 1)
    {
        limite = 7;
    }
    else
    {
        if (prioridade == 2)
        {
            limite = 15;
        }
            else
            {
                limite = 20;
            }
    }
    //Leitura e validação do PERIODO
    do
    {
        printf("\n\t[?] Periodo de espera (1 a %2d dias)     : ", limite);
        if (scanf("%d", &periodo) != 1)
        {
            printf("\t    -> [ERRO] Digite um numero inteiro valido!\n");
            periodo = 0;
            esvaziar_buffer();
        }
        else
        {
            esvaziar_buffer();
            if (!periodoValido(prioridade, periodo))
            {
                printf("\t    -> [ERRO] Periodo incompativel com a prioridade escolhida (%d)!\n", prioridade);
            }
        }
    } while (!periodoValido(prioridade, periodo));

    printf("\n\t---------------------------------------------------------");

    if (insereOrdenado(lista, codSol, codEq, nomeEq, prioridade, periodo))
    {
        printf("\n\t[SUCESSO] Solicitacao cadastrada com sucesso!\n");
    }
    else
    {
        printf("\n\t[ERRO] Falha critica ao inserir a solicitacao na lista!\n");
    }
    printf("\t---------------------------------------------------------");
}

void interface_remover(Lista *lista)
{
    printf("\n\t---------------------------------------------------------");
    printf("\n\t                REMOVER SOLICITACAO                      ");
    printf("\n\t---------------------------------------------------------\n");

    if (listaVazia(lista))
    {
        printf("\n\t[AVISO] Nao ha solicitacoes cadastradas no sistema.\n");
    }
    else
    {
        int codSol;
        printf("\n\t[?] Informe o Codigo da Solicicitacao (1000 - 9999): ");

        if (scanf("%d", &codSol) != 1)
        {
            printf("\n\t    -> [ERRO] Entrada invalida! Digite apenas numeros.\n");
            esvaziar_buffer();
        }
        else
        {
            esvaziar_buffer();

            if (codSol < 1000 || codSol > 9999)
            {
                printf("\n\t    -> [ERRO] O codigo deve conter exatamente 4 digitos!\n");
            }
            else
            {
                if (retiraPorCodigo(lista, codSol))
                {
                    printf("\n\t[SUCESSO] Solicitacao %04d removida com sucesso!\n", codSol);
                }
                else
                {
                    printf("\n\t[ERRO] Solicitacao %04d nao foi encontrada na base de dados.\n", codSol);
                }
            }
        }
    }
    printf("\n\t---------------------------------------------------------");
}

void interface_consultar(Lista *lista)
{
    printf("\n\t---------------------------------------------------------");
    printf("\n\t                CONSULTAR SOLICITACAO                    ");
    printf("\n\t---------------------------------------------------------\n");

    if (listaVazia(lista))
    {
        printf("\n\t[AVISO] Nao ha solicitacoes cadastradas no sistema.\n");
    }
    else
    {
        int codSol;
        printf("\n\t[?] Informe o Codigo da Solicicitacao (1000 - 9999): ");

        if (scanf("%d", &codSol) != 1)
        {
            printf("\n\t    -> [ERRO] Entrada invalida! Digite apenas numeros.\n");
            esvaziar_buffer();
        }
        else
        {
            esvaziar_buffer();

            if (codSol < 1000 || codSol > 9999)
            {
                printf("\n\t    -> [ERRO] O codigo deve conter exatamente 4 digitos!\n");
            }
            else
            {
                No *no = buscaPorCodigo(lista, codSol);
                if (no != NULL)
                {
                    printf("\n\t[SUCESSO] Registro localizado com sucesso:\n");
                    imprimeNo(no);
                }
                else
                {
                    printf("\n\t[ERRO] Solicitacao %04d nao foi encontrada.\n", codSol);
                }
            }
        }
    }
    printf("\n\t---------------------------------------------------------");
}

void interface_alterar(Lista *lista)
{
    printf("\n\t---------------------------------------------------------");
    printf("\n\t               ALTERAR PRIORIDADE/PERIODO                ");
    printf("\n\t---------------------------------------------------------\n");

    if (listaVazia(lista))
    {
        printf("\n\t[AVISO] Nao ha solicitacoes cadastradas no sistema.\n");
    }
    else
    {
        int codSol;
        printf("\n\t[?] Informe o Codigo da Solicicitacao (1000 - 9999): ");

        if (scanf("%d", &codSol) != 1)
        {
            printf("\n\t    -> [ERRO] Entrada invalida! Digite apenas numeros.\n");
            esvaziar_buffer();
        }
        else
        {
            esvaziar_buffer();

            if (codSol < 1000 || codSol > 9999)
            {
                printf("\n\t    -> [ERRO] O codigo deve conter exatamente 4 digitos!\n");
            }
            else
            {
                No *no = buscaPorCodigo(lista, codSol);
                if (no == NULL)
                {
                    printf("\n\t[ERRO] Solicitacao %04d nao encontrada!\n", codSol);
                }
                else
                {
                    printf("\n\t[INFO] Dados atuais do registo:\n");
                    imprimeNo(no);
                    printf("\n\t---------------------------------------------------------");

                    int novaPrioridade, novoPeriodo;
                    do
                    {
                        printf("\n\t[?] Nova Prioridade (1-Alta | 2-Media | 3-Baixa): ");
                        if (scanf("%d", &novaPrioridade) != 1)
                        {
                            printf("\t    -> [ERRO] Valor invalido!\n");
                            novaPrioridade = 0;
                            esvaziar_buffer();
                        }
                        else
                        {
                            esvaziar_buffer();
                            if (novaPrioridade < 1 || novaPrioridade > 3)
                            {
                                printf("\t    -> [ERRO] Escolha uma prioridade entre 1 e 3!\n");
                            }
                        }
                    } while (novaPrioridade < 1 || novaPrioridade > 3);

                    int limite;
                    if (novaPrioridade == 1)
                    {
                        limite = 7;
                    }
                    else
                    {
                        if (novaPrioridade == 2)
                        {
                            limite = 15;
                        }
                        else
                        {
                            limite = 20;
                        }
                    }
                    do
                    {
                        printf("\n\t[?] Novo Periodo (1 a %d dias)                  : ", limite);
                        if (scanf("%d", &novoPeriodo) != 1)
                        {
                            printf("\t    -> [ERRO] Digite um numero valido!\n");
                            novoPeriodo = 0;
                            esvaziar_buffer();
                        }
                        else
                        {
                            esvaziar_buffer();
                            if (!periodoValido(novaPrioridade, novoPeriodo))
                            {
                                printf("\t    -> [ERRO] Periodo incompativel com a nova prioridade!\n");
                            }
                        }
                    } while (!periodoValido(novaPrioridade, novoPeriodo));

                    printf("\n\t---------------------------------------------------------");
                    if (alteraPrioridadePeriodo(lista, codSol, novaPrioridade, novoPeriodo))
                    {
                        printf("\n\t[SUCESSO] Parametros alterados com sucesso!\n");
                    }
                    else
                    {
                        printf("\n\t[ERRO] Falha ao atualizar a solicitacao.\n");
                    }
                }
            }
        }
    }
    printf("\n\t---------------------------------------------------------");
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese");

    Lista *listaPrincipal = criaLista();
    int opcao;

    do
    {
        system("cls");
        printf("\n\t=========================================================");
        printf("\n\t GERENCIADOR DE MANUTENCAO DE EQUIPAMENTOS - LABORATORIO");
        printf("\n\t=========================================================\n");
        printf("\t| 1 - Inserir Solicitacao de Manutencao                 |\n");
        printf("\t| 2 - Remover Solicitacao                               |\n");
        printf("\t| 3 - Consultar Solicitacao                             |\n");
        printf("\t| 4 - Alterar Prioridade e/ou Periodo                   |\n");
        printf("\t| 5 - Exibir Ordem de Realizacao da Manutencao          |\n");
        printf("\t| 6 - Exibir Todas as Solicitacoes                      |\n");
        printf("\t| 0 - Finalizar Sistema                                 |\n");
        printf("\t=========================================================\n");
        printf("\n\t\tSelecione uma OPCAO: ");

        if (scanf("%d", &opcao) != 1)
        {
            opcao = -1;
        }
        esvaziar_buffer();

        switch (opcao)
        {
        case 0:
            system("cls");
            listaPrincipal = liberaLista(listaPrincipal);
            printf("\n\tEncerrando o programa... Obrigado por utiliza-lo!\n\n");
            exit(0);
            break;

        case 1:
            system("cls");
            printf("\n\t--- INSERIR SOLICITACAO ---\n");
            interface_cadastrar(listaPrincipal);
            pausar();
            break;

        case 2:
            system("cls");
            printf("\n\t--- REMOVER SOLICITACAO ---\n");
            interface_remover(listaPrincipal);
            pausar();
            break;

        case 3:
            system("cls");
            printf("\n\t--- CONSULTAR SOLICITACAO ---\n");
            interface_consultar(listaPrincipal);
            pausar();
            break;

        case 4:
            system("cls");
            printf("\n\t--- ALTERAR SOLICITACAO ---\n");
            interface_alterar(listaPrincipal);
            pausar();
            break;

        case 5:
            system("cls");
            printf("\n\t--- ORDEM DE REALIZACAO DA MANUTENCAO ---\n");
            if (listaVazia(listaPrincipal))
            {
                printf("\n\tNao ha solicitacoes cadastradas.\n");
            }
            else
            {
                Lista *listaOrdem = geraOrdemManutencao(listaPrincipal);
                imprimeLista(listaOrdem);
                listaOrdem = liberaLista(listaOrdem);
            }
            pausar();
            break;

        case 6:
            system("cls");
            printf("\n\t--- TODAS AS SOLICITACOES CADASTRADAS ---\n");
            imprimeLista(listaPrincipal);
            pausar();
            break;

        default:
            system("cls");
            printf("\n\t[ERRO] Opcao Invalida! Tente novamente.\n");
            pausar();
            break;
        }

    } while (opcao != 0);

    return 0;
}
