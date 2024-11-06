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

#define foreach_list(pos, head, member)                          \
    for (pos = container_of((head)->next, typeof(*pos), member); \
         &pos->member != head;                                   \
         pos = container_of(pos->member.next, typeof(*pos), member))

int main()
{
    struct list_head listhead;
    init_head(&listhead);

    struct MyNode node1;
    node1.data = 1;
    init_head(&node1.node);
    list_add(&node1.node, &listhead);

    // add node2
    struct MyNode node2;
    node2.data = 2;
    init_head(&node2.node);
    list_add(&node2.node, &listhead);

    // add node3
    struct MyNode node3;
    node3.data = 3;
    init_head(&node3.node);
    list_add(&node3.node, &listhead);

    struct MyNode *pos;

    foreach_list(pos, &listhead, node)
    {
        printf("data = %d\n", pos->data);
    }

    return 0;
}
