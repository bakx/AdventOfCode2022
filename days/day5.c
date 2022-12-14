#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Stack
{
    struct Node * next;
    struct Stack ** stack;
    int position;
    size_t size;
} Stack;

typedef struct Node 
{
    char * key;
    struct Node * next;
} Node;


Node * pop(Stack * stack)
{
    if (!stack->next)
    {
        printf("Unable to pop, no items");
        return NULL;
    }

    Node * prev = NULL;
    Node * next = stack->next;
    
    while (1)
    {
        if (!next->next)
            break;
        
        prev = next;
        next = next->next;
    }

    if (prev)
        prev->next = NULL;
    
    if (stack->next == next)
        stack->next = NULL;

    stack->size -= 1;

    return next;
}

void push(Stack * stack, Node * n, bool inverse)
{
    if (!stack->next)
    {
        stack->next = n;
        stack->size += 1;
        return;
    }

    if (!inverse)
    {
        Node * prev = NULL;
        Node * next = stack->next;

        while (1)
        {
            if (!next->next)
                break;

            next = next->next;
        }

        next->next = n;
    }
    else
    {
        Node * prev = NULL;
        Node * next = stack->next;

        while (1)
        {
            if (!next->next)
                break;

            next = next->next;
        }

        n->next = stack->next;
        stack->next = n;
    }

    stack->size += 1;
}

void move(Stack * s, Stack * d, int count)
{
    Node * prev = NULL;
    Node * next = s->next;

    if (s->size - count > 0)
    {
        for (int i = 0; i < s->size - count; i++)
        {
            prev = next;
            next = next->next;        
        }
    }

    if (prev)
        prev->next = NULL;

    Node * dest = d->next;

    if (dest)
    {
        while(1)
        {
            if (!dest->next)
                break;
            
            dest = dest->next;
        }

        dest->next = next;
    }
    else
        d->next = next;

    s->size -= count;

    if (s->size == 0)
        s->next = NULL;

    d->size += count;
}

void printStack(Stack * stack)
{
    for (int i = 0; i < stack->size; i++)   // DEBUGGING 
    {
        Node * next = stack->stack[i]->next;
        printf("Stack %d has keys [", i + 1);

        if (!next)
        {
            printf("]\n");
            continue;
        }

        while (1)
        {
            printf(" %c ", next->key[0]);

            if (!next->next)
                break;

            next = next->next;
        }
        printf("]\n");
    }
    printf("\n");
}

void cleanStack(Stack * stack)
{
    Node * prev = NULL;
    Node * next = stack->next;

    while(1)
    {
        if (!next || !next->next)
            break;
        
        prev = next;
        next = next->next;

        free(prev);
    }
}

char findTop(Stack * stack)
{
    Node * next = stack->next;
    if (!next)
        return ' ';
    
    while (1)
    {
        if (!next->next)
            break;

        next = next->next;
    }

    return next->key[0];
}

Node * createNode(char * key)
{
    Node * n = malloc(sizeof(Node));
    n->key = strdup(key);
    n->next = NULL;
    return n;
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day5.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    bool loadInstructions = false;
    bool init = false;

    Stack * stack = malloc(sizeof(Stack));
    stack->size = 0;
    Stack * stack2 = malloc(sizeof(Stack));
    stack->size = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (!init) // Must be first line, do some init stuff
        {
            stack->size = read/4;
            stack->stack = malloc(sizeof(Stack*) * stack->size);

            for (int i=0; i < stack->size; i++)
            {
                stack->stack[i] = malloc(sizeof(Stack));
                stack->stack[i]->size = 0;
            }

            stack2->size = read/4;
            stack2->stack = malloc(sizeof(Stack*) * stack2->size);

            for (int i=0; i < stack2->size; i++)
            {
                stack2->stack[i] = malloc(sizeof(Stack));
                stack2->stack[i]->size = 0;
            }

            init = true;
        }

        if (read == 1)
        {
            printStack(stack);
            loadInstructions = true;
            continue;
        }

        if (loadInstructions)
        {
            bool p1, p2, p3;
            char m1[5], m2[5], m3[5] = {0};
            memset(m1, 0, sizeof(m1));
            memset(m2, 0, sizeof(m2));
            memset(m3, 0, sizeof(m3));
            
            p1 = true;
            p2 = false;
            p3 = false;
            bool hasStarted = false;
            
            for(int i = 5; i < strlen(line); i++)
            {
                char c = line[i];

                if ((int) c == 32 && hasStarted)
                {
                    if (p1 && !p2)
                        p2 = true;
                    else if (p1 && p2)
                        p3 = true;

                    hasStarted = false;                    
                    continue;
                }

                 if ((int) c < 48 || (int) c > 57)
                    continue;

                char * dest = (p1 && !p2 && !p3 ? m1 : p1 && p2 && !p3 ? m2 : m3);
                hasStarted = true;

                strncat(dest, &c, 1);
            }

            int i1, i2, i3;
            char * ptr;
            i1 = strtol(m1, &ptr, 10);
            i2 = strtol(m2, &ptr, 10);
            i3 = strtol(m3, &ptr, 10);

            // Part 1
            Stack * s = stack->stack[i2 - 1];
            Stack * d = stack->stack[i3 - 1];

            for (int j=0; j< i1; j++) 
            {
                Node * n = pop(s);
                push(d, n, false);
            }

            // Part 2
            Stack * s2 = stack2->stack[i2 - 1];
            Stack * d2 = stack2->stack[i3 - 1];

            move(s2, d2, i1);
            continue;
        }

        for(int i = 0; i < strlen(line); i++)
        {
            char c = line[i];

            if ( (int) c < 65 || (int) c > 90)
                continue;

            // Get position
            int position = i / 4;
            if (i == 1)
                position = 0;
            if (read - 4 == i)
                position -= 1; // final object has no space

            Node * node = createNode(&c);
            push(stack->stack[position], node, true);

            Node * node2 = createNode(&c);
            push(stack2->stack[position], node2, true);
        }
    }

    printf("Crate values #1 [");
    for (int i = 0; i < stack->size; i++)
    {
        printf( "%c", findTop(stack->stack[i]));
        cleanStack(stack->stack[i]);
    }
    printf("]\n");

    printf("Crate values #2 [");
    for (int i = 0; i < stack2->size; i++)
    {
        printf( "%c", findTop(stack2->stack[i]));
        cleanStack(stack2->stack[i]);
    }
    printf("]\n");

    free(stack);
    free(stack2);
    
    free(line);
    fclose(fp);
}