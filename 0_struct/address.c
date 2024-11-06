#include <stdio.h>
#include <stdlib.h>

struct list_head
{
    struct list_head *prev, *next;
};

void init_head(struct list_head *head)
{
    head->prev = head;
    head->next = head;
}

struct MyNode
{
    int data;
    struct list_head node;
    struct list_head node1;
};

void __list_add(struct list_head *new,
                struct list_head *prev,
                struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

int main()
{
    struct list_head listhead;

    init_head(&listhead);

    struct MyNode node1;
    
    node1.data = 1;

    init_head(&node1.node);

    list_add(&node1.node, &listhead);

    // node1 address
    printf("struct MyNode address is %ld\n", &node1);
    
    // node1.node address
    printf("node1.node address is %ld\n", &node1.node);

    // node1.node address + 1 address
    printf("node1.node address + 1 is %ld\n", &node1.node + 1);

    // node1.node offset size
    printf("node offset address is %ld\n", &((struct MyNode *)0)->node);

    // node1.node offset size
    printf("node offset number is %ld\n", (unsigned long)&((struct MyNode *)0)->node);

    // node1.node address - node1.node offset size ==>  node1 adress
    printf("struct MyNode address is %ld\n", (char *)&node1.node - (unsigned long)&((struct MyNode *)0)->node );

    return 0;
}
