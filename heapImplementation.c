#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define name_size 10
#define SIZE 2000
typedef struct free_list
{
    int start;
    int size;
    struct free_list *next;
    struct free_list *prev;
} free_Node;
typedef struct alloc_blocks
{
    char name[name_size];
    int start;
    int size;
    struct alloc_blocks *next;
    struct alloc_blocks *prev;
} alloc_list;
free_Node *fptr;
alloc_list *alptr;
void allocate(char *, int);
void print_BothLists();
void deallocate(alloc_list *, char *);
alloc_list *allocatethenode(alloc_list *, alloc_list *);
void deallocate(alloc_list *ptr, char *name)
{
    while (ptr)
    {
        if (strcmp(ptr->name, name) == 0)
            break;
        ptr = ptr->next;
    }
    if (ptr == NULL)
    {
        printf("Not found\n");
    }
    else
    {
        free_Node *pptr = (free_Node *)malloc(sizeof(free_Node));
        pptr->next = pptr->prev = NULL;
        pptr->size = ptr->size;
        pptr->start = ptr->start;
        if (ptr == alptr)
            alptr = ptr->next;
        else
        {
            ptr->prev->next = ptr->next;
            if (ptr->next)
                ptr->next->prev = ptr->prev;
        }
        free_Node *t = fptr;
        if (fptr == NULL)
            fptr = pptr;
        else
        {
            while (t && t->next && t->start < pptr->start)
            {
                t = t->next;
            }

            if (t && t->next == NULL && t->start < pptr->start)
            {
                t->next = pptr;
                pptr->prev = t;
            }
            else
            {
                if (t && t == fptr)
                {
                    pptr->next = t;
                    t->prev = pptr;
                    fptr = pptr;
                }
                else if (t != NULL)
                {
                    pptr->prev = t->prev;
                    t->prev->next = pptr;
                    t->prev = pptr;
                    pptr->next = t;
                }
            }
        }
        free_Node *ptr = pptr;
        if (ptr->prev != NULL)
        {
            if (ptr->prev->start == ptr->start - ptr->prev->size)
            {
                ptr->prev->size = ptr->prev->size + ptr->size;
                ptr->prev->next = ptr->next;
                if (ptr->next != NULL)
                    ptr->next->prev = ptr->prev;
                free_Node *temp = ptr;
                ptr = ptr->prev;
                free(temp);
            }
        }
        if (ptr->next != NULL)
        {
            if (ptr->start == ptr->next->start - ptr->size)
            {
                ptr->size = ptr->size + ptr->next->size;
                ptr->next = ptr->next->next;
                if (ptr->next != NULL)
                    ptr->next->prev = ptr;
            }
        }
    }
}
alloc_list *allocatethenode(alloc_list *hptr, alloc_list *pptr)
{
    alloc_list *ptr, *pn;
    pptr->next = pptr->prev = NULL;
    if (hptr == NULL)
        hptr = pptr;
    else
    {
        ptr = hptr;
        while (ptr->next != NULL)
        {
            if (pptr->start > ptr->start)
                ptr = ptr->next;
        }

        if (ptr->next == NULL)
        {
            if (pptr->start > ptr->start)
            {
                ptr->next = pptr;
                pptr->prev = ptr;
            }
        }
        else
        {
            if (ptr == hptr)
                hptr = pptr;
            pn = ptr->prev;
            if (pn != NULL)
                pn->next = pptr;
            pptr->prev = pn;
            pptr->next = ptr;
            ptr->prev = pptr;
        }
    }
    return hptr;
}
void allocate(char *name, int n)
{
    free_Node *ptr = fptr;
    int status = 0, temp = 0;
    alloc_list *check = alptr;
    while (check != NULL)
    {
        if (strcmp(check->name, name) == 0)
        {
            temp = 1;
            break;
        }
        check = check->next;
    }
    if (temp == 1)
        printf("same block plz reenter\n");
    else
    {
        while (!status && ptr != NULL)
        {
            if (ptr->size < n)
                ptr = ptr->next;
            else
                status = 1;
        }
        if (status == 0)
            printf("\nnot enough space\n\n");
        else
        {
            alloc_list *sptr = (alloc_list *)malloc(sizeof(alloc_list));
            sptr->start = ptr->start;
            sptr->size = n;
            strcpy(sptr->name, name);
            sptr->next = NULL;
            sptr->prev = NULL;
            alloc_list *optr = alptr;
            alptr = allocatethenode(alptr, sptr);
            ptr->start = ptr->start + n;
            ptr->size = ptr->size - n;
            int s = ptr->size;
            if (s == 0)
            {
                if (ptr != NULL)
                {
                    free_Node *pn, *nn;
                    pn = ptr->prev;
                    nn = ptr->next;
                    if (pn != NULL)
                        pn->next = nn;
                    else
                        fptr = nn;
                    if (nn != NULL)
                        nn->prev = pn;
                    free(ptr);
                }
            }
        }
    }
}
void print_BothLists()
{
    alloc_list *aptr = alptr;
    while (aptr != NULL)
    {
        printf("ALLOCATED BLOCKS\n");
        int x = aptr->start;
        int y = aptr->start + aptr->size;
        printf("%d to %d\n", x, y);
        aptr = aptr->next;
    }
    free_Node *yptr = fptr;
    while (yptr != NULL)
    {

        printf("FREE BLOCKS\n");
        int x = yptr->start;
        int y = yptr->start + yptr->size;
        printf("%d to %d\n", x, y);
        yptr = yptr->next;
    }
}
int main()
{
    int choice;
    fptr = (free_Node *)malloc(sizeof(free_Node));
    fptr->start = 0;
    fptr->size = SIZE;
    fptr->prev = NULL;
    fptr->next = NULL;
    printf("Memory available 2000\n\n");
    choice = 1;
    while (choice != 0)
    {
        printf("1.Allocate \n2.Deallocate\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            int n;
            scanf("%d", &n);
            char name[name_size];
            scanf("%s", name);
            allocate(name, n);
            printf("HEAP\n");
            print_BothLists();
        }
        else if (choice == 2)
        {
            char name[name_size];
            printf("Enter tag of block you want to delete\n");
            scanf("%s", name);
            deallocate(alptr, name);
            printf("HEAP\n");
            print_BothLists();
        }
        else
            printf("enter either 2 or 1\n");
        printf("If you want to continue enter '1' else '0'\n");
    }
}
