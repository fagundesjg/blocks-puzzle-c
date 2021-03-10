#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------
// DEFINIÇÕES DE CONSTANTES

#define TRUE 1
#define FALSE 0

//--------------------------------------------------
// ESTRUTURAS DE DADOS

typedef struct node
{
    struct node *next;
    struct node *prev;
    int value;
} NODE;

typedef struct
{
    NODE *head;
    NODE *tail;
    int size;
} LIST;

typedef struct
{
    LIST **lists;
    int size;
} TBLOCOS;

//--------------------------------------------------
// CABEÇALHO DAS FUNÇÕES

/**
 * Função que recebe uma string e retorna TRUE caso seja um número ou FALSE caso contrário. 
 * */
int is_num(const char *str);

/**
 * Função que recebe uma lista e retorna TRUE caso ela esteja vazia ou FALSE caso contrário.
 * */
int list_empty(LIST list);

/**
 * Função que recebe um número e retorna um NODE alocado com seus respectivos apontamentos iniciais e o valor inserido.
 * */
NODE *node_create(int value);

/**
 * Função que recebe uma lista e um valor como parâmetro. O valor é inserido ao final da lista.
 * */
void list_add(LIST *list, int value);

/**
 * Função que recebe uma lista e remove o último elemento retornando o seu valor.
 * */
int list_pop(LIST *list);

/**
 * Função que cria uma lista vazia com uma célula cabeça/sentinela e que guarda a referência do primeiro e último elemento,
 * além do tamanho da lista.
 * Obs: A célula cabeça serve para facilitar algumas operações de apontamentos.
 * */
LIST *list_create();

/**
 * Função que libera todos os nós da lista da memória.
 * */
void list_free(LIST *list);

/**
 * Função responsável por criar uma estrutura de TBlocos do tamanho recebido por parâmetro.
 * */
TBLOCOS tblocos_create(int size);

/**
 * Função que libera da memória toda a estrutura de tblocos e reinicia seus valores.
 * */
void tblocos_free(TBLOCOS *tblocos);

/**
 * Função que imprime uma lista na tela de forma tradicional (horizontal).
 * */
void list_print(LIST *list);

/**
 * Função que imprime uma estrutura de tblocos na tela na forma vertical.
 * */
void tblocos_print(TBLOCOS tblocos);

/**
 * Função que recebe uma lista e um valor e retorna TRUE caso o valor esteja na lista ou FALSE caso contrário.
 * */
int list_has_value(LIST list, int value);

/**
 * Função que recebe uma estrutura de tblocos e um valor e retorna qual o índice do monte onde o mesmo se encontra ou -1.
 * */
int tblocos_index_of(TBLOCOS tblocos, int value);

/**
 * Função que recebe uma estrutura de tblocos e um índice e retorna TRUE se aquele índice é um índice existente dentro da estrutura ou
 * FALSE caso contrário.
 * */
int tblocos_is_valid_index(TBLOCOS tblocos, int index);

/**
 * Função que recebe a estrutura tblocos e um índice de monte A e um índice de monte B.
 * A função move o elemento do topo do monte A para o topo do monte B.
 * */
void tblocos_move_top(TBLOCOS *tblocos, int index_a, int index_b);

/**
 * Função que recebe uma estrutura de tblocos e o índice de um monte e um valor.
 * Responsável por ir removendo os elementos do topo do monte do índice recebido e ir retornando-os aos seus montes originais
 * até o monte ficar vazio ou encontrar o elemento de valor igual ao recebido no terceiro parâmetro.
 * */
void tblocos_back_above_elements_to_original_position(TBLOCOS *tblocos, int list_index, int value);

/**
 *  Move o bloco a para cima do bloco b retornando eventuais blocos que já estiverem sobre a ou b para as suas posições originais.
 * */
int move_onto(TBLOCOS *tblocos, int a, int b);

/**
 * Move o bloco a no topo do monte onde está o bloco b retornando eventuais blocos que já estiverem sobre a às suas posições originais.
 * */
int move_over(TBLOCOS *tblocos, int a, int b);

/**
 * Coloca o bloco a juntamente com todos os blocos que estiverem sobre ele sobre o monte que contém o bloco b.
 * */
int pile(TBLOCOS *tblocos, int a, int b);

/**
 * Move o bloco a juntamente com todos os blocos que estiverem sobre ele em cima do bloco b, retornando eventuais blocos que já estiverem sobre b às suas posições originais.
 * */
int pile_onto(TBLOCOS *tblocos, int a, int b);

/**
 * Coloca o bloco a juntamente com todos os blocos que estiverem sobre ele sobre o monte que contém o bloco b.
 * */
int pile_over(TBLOCOS *tblocos, int a, int b);

//--------------------------------------------------
// IMPLEMENTAÇÕES DAS FUNÇÕES

int is_num(const char *str)
{
    while (*str != '\0')
    {
        if (*str < '0' || *str > '9')
            return FALSE;
        str++;
    }
    return TRUE;
}

int list_empty(LIST list)
{
    return list.size == 0;
}

NODE *node_create(int value)
{
    NODE *node = (NODE *)malloc(sizeof(NODE));
    node->value = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void list_add(LIST *list, int value)
{
    NODE *node = node_create(value);
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
    list->size++;
}

int list_pop(LIST *list)
{
    int value = list->tail->value;
    NODE *tmp = list->tail;

    list->tail = tmp->prev;
    list->tail->next = NULL;
    list->size--;
    free(tmp);

    return value;
}

LIST *list_create()
{
    LIST *list = (LIST *)malloc(sizeof(LIST));
    list->head = node_create(-1);
    list->tail = list->head;
    list->size = 0;
    return list;
}

void list_free(LIST *list)
{
    NODE *aux = list->head;
    NODE *tmp;

    while (aux != NULL)
    {
        tmp = aux;
        aux = aux->next;
        free(tmp);
        list->size--;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

TBLOCOS tblocos_create(int size)
{
    TBLOCOS tblocos;
    int i;

    tblocos.lists = (LIST **)malloc(sizeof(LIST *) * size);
    tblocos.size = size;

    for (i = 0; i < size; i++)
    {
        tblocos.lists[i] = list_create();
        list_add(tblocos.lists[i], i);
    }

    return tblocos;
}

void tblocos_free(TBLOCOS *tblocos)
{
    int i;

    for (i = 0; i < tblocos->size; i++)
    {
        list_free(tblocos->lists[i]);
    }
    free(tblocos->lists);
    tblocos->lists = NULL;
    tblocos->size = 0;
}

void list_print(LIST *list)
{
    NODE *aux = list->head->next;
    while (aux != NULL)
    {
        printf("%2d ", aux->value);
        aux = aux->next;
    }
    printf("\n");
}

void tblocos_print(TBLOCOS tblocos)
{
    int i;
    NODE *aux[tblocos.size];
    int sizes[tblocos.size];
    int offset = tblocos.size;
    char line[tblocos.size * 3 + 1];
    int any_block = FALSE;
    strcpy(line, "");

    printf("\n");

    for (i = 0; i < tblocos.size; i++)
    {
        aux[i] = tblocos.lists[i]->tail;
        sizes[i] = tblocos.lists[i]->size;
    }

    while (offset > 0)
    {
        for (i = 0; i < tblocos.size; i++)
        {
            if (sizes[i] >= offset)
            {
                any_block = TRUE;
                sprintf(line, "%s%2d ", line, aux[i]->value);
                aux[i] = aux[i]->prev;
            }
            else
            {
                strcat(line, "   ");
            }
        }
        if (any_block)
        {
            printf("%s\n", line);
        }
        strcpy(line, "");
        offset--;
        any_block = FALSE;
    }
}

int list_has_value(LIST list, int value)
{
    NODE *aux = list.head->next;

    while (aux != NULL)
    {
        if (aux->value == value)
            return TRUE;
        aux = aux->next;
    }

    return FALSE;
}

int tblocos_index_of(TBLOCOS tblocos, int value)
{
    int i;

    for (i = 0; i < tblocos.size; i++)
    {
        if (list_has_value(*tblocos.lists[i], value))
        {
            return i;
        }
    }

    return -1;
}

int tblocos_is_valid_index(TBLOCOS tblocos, int index)
{
    return index >= 0 && index < tblocos.size;
}

void tblocos_move_top(TBLOCOS *tblocos, int index_a, int index_b)
{
    if (tblocos_is_valid_index(*tblocos, index_a) && tblocos_is_valid_index(*tblocos, index_b) && !list_empty(*tblocos->lists[index_a]))
    {
        int top_element = list_pop(tblocos->lists[index_a]);
        list_add(tblocos->lists[index_b], top_element);
    }
}

void tblocos_back_above_elements_to_original_position(TBLOCOS *tblocos, int list_index, int value)
{
    if (tblocos_is_valid_index(*tblocos, list_index))
    {
        LIST *list_aux = tblocos->lists[list_index];
        int top_value;
        while (!list_empty(*list_aux) && list_aux->tail->value != value)
        {
            top_value = list_pop(list_aux);
            list_add(tblocos->lists[top_value], top_value);
        }
    }
}

int move_onto(TBLOCOS *tblocos, int a, int b)
{
    int index_list_a = tblocos_index_of(*tblocos, a);
    int index_list_b = tblocos_index_of(*tblocos, b);

    if (index_list_a != -1 && index_list_b != -1 && index_list_a != index_list_b)
    {
        tblocos_back_above_elements_to_original_position(tblocos, index_list_a, a);
        tblocos_back_above_elements_to_original_position(tblocos, index_list_b, b);
        tblocos_move_top(tblocos, index_list_a, index_list_b);
        return TRUE;
    }
    return FALSE;
}

int move_over(TBLOCOS *tblocos, int a, int b)
{
    int index_list_a = tblocos_index_of(*tblocos, a);
    int index_list_b = tblocos_index_of(*tblocos, b);

    if (index_list_a != -1 && index_list_b != -1 && index_list_a != index_list_b)
    {
        tblocos_back_above_elements_to_original_position(tblocos, index_list_a, a);
        tblocos_move_top(tblocos, index_list_a, index_list_b);
        return TRUE;
    }
    return FALSE;
}

int pile(TBLOCOS *tblocos, int a, int b)
{
    int index_list_a = tblocos_index_of(*tblocos, a);
    int index_list_b = tblocos_index_of(*tblocos, b);

    if (index_list_a != -1 && index_list_b != -1 && index_list_a != index_list_b)
    {
        LIST *list_a = tblocos->lists[index_list_a];
        LIST *list_b = tblocos->lists[index_list_b];
        NODE *p = list_a->head->next;
        int count = 0;

        while (p != NULL)
        {
            if (p->value == a)
            {
                NODE *aux = list_b->tail;
                list_b->tail->next = p;
                list_b->tail = list_a->tail;
                list_a->tail = p->prev;
                list_a->tail->next = NULL;
                p->prev = aux;
                while (p != NULL)
                {
                    count++;
                    p = p->next;
                }
                list_a->size -= count;
                list_b->size += count;
            }
            else
            {
                p = p->next;
            }
        }
        return TRUE;
    }
    return FALSE;
}

int pile_onto(TBLOCOS *tblocos, int a, int b)
{
    int index_list_a = tblocos_index_of(*tblocos, a);
    int index_list_b = tblocos_index_of(*tblocos, b);

    if (index_list_a != -1 && index_list_b != -1 && index_list_a != index_list_b)
    {
        tblocos_back_above_elements_to_original_position(tblocos, index_list_b, b);
        pile(tblocos, a, b);
        return TRUE;
    }
    return FALSE;
}

int pile_over(TBLOCOS *tblocos, int a, int b)
{
    int index_list_a = tblocos_index_of(*tblocos, a);
    int index_list_b = tblocos_index_of(*tblocos, b);

    if (index_list_a != -1 && index_list_b != -1 && index_list_a != index_list_b)
    {
        pile(tblocos, a, b);
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char *argv[])
{
    char input[30], cmds[4][30], *token;
    int i, a, b, error = FALSE, running = TRUE;
    TBLOCOS tblocos = tblocos_create(5);

    while (running)
    {
        tblocos_print(tblocos);
        fflush(stdin);
        printf("\n>> ");
        scanf("%[^\n]s", input);

        if (strcmp(input, "quit") == 0)
        {
            printf("\nPrograma finalizado.\n");
            running = FALSE;
        }
        else
        {
            i = 0;
            token = strtok(input, " ");

            if (token != NULL)
                strcpy(cmds[i], token);

            while (token != NULL)
            {
                i++;
                token = strtok(NULL, " ");
                if (token != NULL)
                    strcpy(cmds[i], token);
            }

            if (i != 4)
            {
                printf("\nNumero incorreto de parametros. Tente novamente.\n");
            }
            else
            {
                if (is_num(cmds[1]) && is_num(cmds[3]))
                {
                    a = atoi(cmds[1]);
                    b = atoi(cmds[3]);
                    if (!(a >= 0 && a < tblocos.size && b >= 0 && b < tblocos.size))
                    {
                        error = TRUE;
                    }
                }
                else
                {
                    printf("\nO segundo e o quarto argumento deve ser um numero! Tente novamente.\n");
                }

                if (error)
                {
                    printf("\nO segundo e o quarto argumento deve ser um numero >= 0 e < que %d. Tente novamente.\n", tblocos.size);
                }
                else
                {
                    if (strcmp(cmds[0], "move") == 0 && strcmp(cmds[2], "onto") == 0)
                    {
                        move_onto(&tblocos, a, b);
                    }
                    else if (strcmp(cmds[0], "move") == 0 && strcmp(cmds[2], "over") == 0)
                    {
                        move_over(&tblocos, a, b);
                    }
                    else if (strcmp(cmds[0], "pile") == 0 && strcmp(cmds[2], "onto") == 0)
                    {
                        pile_onto(&tblocos, a, b);
                    }
                    else if (strcmp(cmds[0], "pile") == 0 && strcmp(cmds[2], "over") == 0)
                    {
                        pile_over(&tblocos, a, b);
                    }

                    else
                    {
                        printf("\nParametro incorreto. Tente novamente\n");
                    }
                }
            }
        }
    }

    tblocos_free(&tblocos);
    return 0;
}