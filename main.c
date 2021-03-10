#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

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

int main(int argc, char *argv[])
{
    int running = TRUE;
    int i;
    char input[30];
    char cmds[4][30];
    char *token;
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
                int index_a, index_b, error = FALSE;
                if (is_num(cmds[1]) && is_num(cmds[3]))
                {
                    index_a = atoi(cmds[1]);
                    index_b = atoi(cmds[3]);
                    if (!(index_a >= 0 && index_a < tblocos.size && index_b >= 0 && index_b < tblocos.size))
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
                        move_onto(&tblocos, index_a, index_b);
                    }
                    else if (strcmp(cmds[0], "move") == 0 && strcmp(cmds[2], "over") == 0)
                    {
                    }
                    else if (strcmp(cmds[0], "pile") == 0 && strcmp(cmds[2], "onto") == 0)
                    {
                    }
                    else if (strcmp(cmds[0], "pile") == 0 && strcmp(cmds[2], "over") == 0)
                    {
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