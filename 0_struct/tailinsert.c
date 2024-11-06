#include <stdio.h>
#include <stdlib.h>

struct list_head listhead;
struct MyNode node1;
struct MyNode node2;
struct MyNode node3;

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
    // printf("new add is %p, prev add is %p, next add is %p\n", new, prev, next);
    // printf("-----------------------------------------------------------------------------------------------------------------\n");
    // printf("head add is %p, head->prev add is %p, head->next add is %p\n", &listhead, listhead.prev, listhead.next);
    // printf("node1 add is %p, node1->prev add is %p, node1->next add is %p\n", &(node1.node), node1.node.prev, node1.node.next);
    // printf("new add is %p, prev add is %p, next add is %p\n", new, prev, next);
    // printf("-----------------------------------------------------------------------------------------------------------------\n");
    next->prev = new;
    // printf("head add is %p, head->prev add is %p, head->next add is %p\n", &listhead, listhead.prev, listhead.next);
    // printf("node1 add is %p, node1->prev add is %p, node1->next add is %p\n", &(node1.node), node1.node.prev, node1.node.next);
    // printf("new add is %p, prev add is %p, next add is %p\n", new, prev, next);
    new->next = next;
    // printf("-----------------------------------------------------------------------------------------------------------------\n");
    // printf("head add is %p, head->prev add is %p, head->next add is %p\n", &listhead, listhead.prev, listhead.next);
    // printf("node1 add is %p, node1->prev add is %p, node1->next add is %p\n", &(node1.node), node1.node.prev, node1.node.next);
    // printf("new add is %p, prev add is %p, next add is %p\n", new, prev, next);
    new->prev = prev;
    // printf("-----------------------------------------------------------------------------------------------------------------\n");
    // printf("head add is %p, head->prev add is %p, head->next add is %p\n", &listhead, listhead.prev, listhead.next);
    // printf("node1 add is %p, node1->prev add is %p, node1->next add is %p\n", &(node1.node), node1.node.prev, node1.node.next);
    // printf("new add is %p, prev add is %p, next add is %p\n", new, prev, next);
    prev->next = new;
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define foreach_list(pos, head, member)                          \
    for (pos = container_of((head)->next, typeof(*pos), member); \
         &pos->member != head;                                   \
         pos = container_of(pos->member.next, typeof(*pos), member)) \
         {printf("data = %d\n", pos->data);}

int main()
{

    init_head(&listhead);

    node1.data = 1;
    init_head(&node1.node);
    list_add_tail(&node1.node, &listhead);

    node2.data = 2;
    init_head(&node2.node);
    list_add_tail(&node2.node, &listhead);

    node3.data = 3;
    init_head(&node3.node);
    list_add_tail(&node3.node, &listhead);

    struct MyNode *pos;
    foreach_list(pos, &listhead, node);
    {
        printf("data = %d\n", pos->data);
    }

    return 0;
}
