/*
 * Copyright (C) 2015, Leo Ma <begeekmyfriend@gmail.com>
 */

#ifndef _SKIPLIST_H
#define _SKIPLIST_H

struct sk_link {
        struct sk_link *prev, *next;
};

static inline void list_init(struct sk_link *link)
{
        link->prev = link;
        link->next = link;
}

static inline void
__list_add(struct sk_link *link, struct sk_link *prev, struct sk_link *next)
{
        link->next = next;
        link->prev = prev;
        next->prev = link;
        prev->next = link;
}

static inline void __list_del(struct sk_link *prev, struct sk_link *next)
{
        prev->next = next;
        next->prev = prev;
}

static inline void list_add(struct sk_link *link, struct sk_link *prev)
{
        __list_add(link, prev, prev->next);
}

static inline void list_del(struct sk_link *link)
{
        __list_del(link->prev, link->next);
        list_init(link);
}

static inline int list_empty(struct sk_link *link)
{
        return link->next == link;
}

#define list_entry(ptr, type, member) \
        ((type *)((char *)(ptr) - (size_t)(&((type *)0)->member)))

#define skiplist_foreach(pos, end) \
        for (; pos != end; pos = pos->next)

#define skiplist_foreach_safe(pos, n, end) \
        for (n = pos->next; pos != end; pos = n, n = pos->next)

#define MAX_LEVEL 32  /* Should be enough for 2^32 elements */

struct skiplist {
        int level;
        int count;
        struct sk_link head[MAX_LEVEL];
};

struct skipnode {
        int key;
        int value;
        struct sk_link link[0];
};

static struct skipnode *skipnode_new(int level, int key, int value)
{
        struct skipnode *node;
        node = (struct skipnode *)malloc(sizeof(*node) + level * sizeof(struct sk_link));
        if (node != NULL) {
                node->key = key;
                node->value = value;
        }
        return node;
}

static void skipnode_delete(struct skipnode *node)
{
        free(node);
}

static struct skiplist *skiplist_new(void)
{
        int i;
        struct skiplist *list = (struct skiplist *)malloc(sizeof(*list));
        if (list != NULL) {
                list->level = 1;
                list->count = 0;
                for (i = 0; i < sizeof(list->head) / sizeof(list->head[0]); i++) {
                        list_init(&list->head[i]);
                }
        }
        return list;
}

static void skiplist_delete(struct skiplist *list)
{
        struct sk_link *n;
        struct sk_link *pos = list->head[0].next;
        skiplist_foreach_safe(pos, n, &list->head[0]) {
                struct skipnode *node = list_entry(pos, struct skipnode, link[0]);
                skipnode_delete(node);
        }
        free(list);
}

static int random_level(void)
{
        int level = 1;
        const double p = 0.25;
        while ((random() & 0xffff) < 0xffff * p) {
                level++;
        }
        return level > MAX_LEVEL ? MAX_LEVEL : level;
}

static struct skipnode *skiplist_search(struct skiplist *list, int key)
{
        struct skipnode *node;
        int i = list->level - 1;
        struct sk_link *pos = &list->head[i];
        struct sk_link *end = &list->head[i];

        for (; i >= 0; i--) {
                pos = pos->next;
                skiplist_foreach(pos, end) {
                        node = list_entry(pos, struct skipnode, link[i]);
                        if (node->key >= key) {
                                end = &node->link[i];
                                break;
                        }
                }
                if (node->key == key) {
                        return node;
                }
                pos = end->prev;
                pos--;
                end--;
        }

        return NULL;
}

static struct skipnode *
skiplist_insert(struct skiplist *list, int key, int value)
{
        int level = random_level();
        if (level > list->level) {
                list->level = level;
        }

        struct skipnode *node = skipnode_new(level, key, value);
        if (node != NULL) {
                int i = list->level - 1;
                struct sk_link *pos = &list->head[i];
                struct sk_link *end = &list->head[i];

                for (; i >= 0; i--) {
                        pos = pos->next;
                        skiplist_foreach(pos, end) {
                                struct skipnode *nd = list_entry(pos, struct skipnode, link[i]);
                                if (nd->key >= key) {
                                        end = &nd->link[i];
                                        break;
                                }
                        }
                        pos = end->prev;
                        if (i < level) {
                                __list_add(&node->link[i], pos, end);
                        }
                        pos--;
                        end--;
                }

                list->count++;
        }
        return node;
}

static void __remove(struct skiplist *list, struct skipnode *node, int level)
{
        int i;
        for (i = 0; i < level; i++) {
                list_del(&node->link[i]);
                if (list_empty(&list->head[i])) {
                        list->level--;
                }
        }
        skipnode_delete(node);
        list->count--;
}

static void skiplist_remove(struct skiplist *list, int key)
{
        struct sk_link *n;
        struct skipnode *node;
        int i = list->level - 1;
        struct sk_link *pos = &list->head[i];
        struct sk_link *end = &list->head[i];

        for (; i >= 0; i--) {
                pos = pos->next;
                skiplist_foreach_safe(pos, n, end) {
                        node = list_entry(pos, struct skipnode, link[i]);
                        if (node->key > key) {
                                end = &node->link[i];
                                break;
                        } else if (node->key == key) {
                                /* we allow nodes with same key. */
                                __remove(list, node, i + 1);
                        }
                }
                pos = end->prev;
                pos--;
                end--;
        }
}

static void skiplist_dump(struct skiplist *list)
{
        struct skipnode *node;
        int i = list->level - 1;
        struct sk_link *pos = &list->head[i];
        struct sk_link *end = &list->head[i];

        printf("\nTotal %d nodes: \n", list->count);
        for (; i >= 0; i--) {
                pos = pos->next;
                printf("level %d:\n", i + 1);
                skiplist_foreach(pos, end) {
                        node = list_entry(pos, struct skipnode, link[i]);
                        printf("key:0x%08x value:0x%08x\n", node->key, node->value);
                }
                pos = &list->head[i];
                pos--;
                end--;
        }
}

#endif  /* _SKIPLIST_H */
