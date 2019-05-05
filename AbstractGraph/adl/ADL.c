#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ADL.h"

struct adjacency_node {
    node_value value;
    struct adjacency_node * next;
};

struct node_list_node {
    struct node_list_node * next;
    node_value value;
    struct adjacency_node * head;
};

struct graph { 
    struct node_list_node * head;
    size_t count;
};


graph  *    graph_create(void){
    graph * self = (graph *)malloc(sizeof(graph));
    assert(self);
    self->head = NULL;
    self->count = 0;
    return self;
}
void        graph_destroy(graph * self){
    graph_clear(self);
    free(self);
}

bool        graph_empty(const graph * self){
    //assert(self);
    return self == NULL || self->count == 0;
}
size_t      graph_size(const graph * self){
    assert(self);
    return self->count;
}

static struct adjacency_node * new_adj_node(node_value adjacency_value, struct adjacency_node * next){
    struct adjacency_node * ptr = (struct adjacency_node *)malloc(sizeof(struct adjacency_node));
    assert(ptr);
    ptr->value = (char *)malloc(sizeof(char) * strlen(adjacency_value));
    assert(ptr->value);
    strcpy(ptr->value, adjacency_value);
    ptr->next = next;
    return ptr;
}

static bool check_node_exists(graph * self, node_value value){
    struct node_list_node * ptr = self->head;
    while (strcmp(ptr->value, value) != 0 && ptr->next != NULL){
        ptr = ptr->next;
    }
    return strcmp(ptr->value, value) == 0;
}

static struct node_list_node * new_node(node_value value, struct node_list_node * next){
    struct node_list_node * ptr = (struct node_list_node *)malloc(sizeof(struct node_list_node));
    assert(ptr);
    ptr->next = next;
    ptr->value = (char *)malloc(sizeof(char) * strlen(value));
    assert(ptr->value);
    strcpy(ptr->value, value);
    ptr->head = NULL;
    return ptr;
}

void        graph_add_node(graph * self, node_value value){
    assert(self);
    self->head = new_node(value, self->head);
    self->count++;
}

void        graph_delete_edge(graph * self, node_value value1, node_value value2){
    if(check_node_exists(self, value1) && check_node_exists(self,value2)){
        struct node_list_node * ptr = self->head;
        while (strcmp(ptr->value, value1) != 0 && ptr->next != NULL){
            ptr = ptr->next;
        }
        if (strcmp(ptr->value, value1) == 0){
            struct adjacency_node * adj_ptr = ptr->head;
            struct adjacency_node * prev_ptr = NULL;
            while (strcmp(adj_ptr->value, value2) == 0 && adj_ptr->next != NULL){
                prev_ptr = adj_ptr;
                adj_ptr = adj_ptr->next;
            }
        
            if(strcmp(adj_ptr->value, value2) == 0){
                if(prev_ptr == NULL){
                    ptr->head = adj_ptr->next;
                    free(adj_ptr);
                } else {
                    prev_ptr->next = adj_ptr->next;
                    free(adj_ptr);
                }
            } else {
                printf("input not valid!\n");
            }

        } else {
            printf("value1 not found!\n");
        }
    } else {
        printf("input not valid!\n");
    }
}

void        graph_delete_node(graph * self, node_value value){
    assert(self);
    assert(!graph_empty(self));

    struct node_list_node * ptr = self->head;
    struct node_list_node * prev_ptr = NULL;
    while (strcmp(ptr->value, value) != 0 && ptr->next != NULL){
        prev_ptr = ptr;
        ptr = ptr->next;
    }

    if(strcmp(ptr->value, value) == 0){
        struct node_list_node * adj_del_ptr = self->head;
        for (; adj_del_ptr; adj_del_ptr = adj_del_ptr->next) {
            struct adjacency_node *a_ptr_prev = NULL;
            for (struct adjacency_node *a_ptr = adj_del_ptr->head; a_ptr; a_ptr = a_ptr->next){
                if(strcmp(value, a_ptr->value) == 0){
                    if (a_ptr_prev == NULL)
                    {
                        adj_del_ptr->head = a_ptr->next;
                    } else {
                        a_ptr_prev->next = a_ptr->next;
                    }
                }
                a_ptr_prev = a_ptr;
            }
        }

        if(prev_ptr == NULL){
            self->head = ptr->next;
            free(ptr);
        } else {
            prev_ptr->next = ptr->next;
            free(ptr);
        }
        self->count--;
    } else {
        printf("no node with this value saved!\n");
    }
}

void        graph_add_edge(graph * self, node_value value1, node_value value2){
    assert(self);
    assert(!graph_empty(self));
    if(check_node_exists(self, value1) && check_node_exists(self,value2)){

        struct node_list_node * ptr = self->head;
        while (strcmp(ptr->value, value1) != 0 && ptr->next != NULL){
            ptr = ptr->next;
        }

        if (strcmp(ptr->value, value1) == 0){
            ptr->head = new_adj_node(value2, ptr->head);
        } else {
            printf("value1 not found!\n");
        }   
    } else {
        printf("input not vaild!\n");
    }
}


void        graph_clear(graph * self){
    struct node_list_node * ptr = self->head;
    struct node_list_node * next_ptr = NULL;
    struct adjacency_node * adj_ptr = NULL;
    struct adjacency_node * next_adj_ptr = NULL;
    while(ptr != NULL){
        next_ptr = ptr->next;
        //free adjacency list
        adj_ptr = ptr->head;
        while(adj_ptr != NULL) {
            next_adj_ptr = adj_ptr->next;
            free(adj_ptr);
            adj_ptr = next_adj_ptr;
        }
        free(ptr);
        ptr = next_ptr;
        self->count--;
    }
}

void        graph_print(graph * self){
    assert(self);
    for(const struct node_list_node * ptr = self->head; ptr; ptr = ptr->next){
        printf("\nNODE: %s\n", ptr->value);
    }
    printf("\n");
    for(const struct node_list_node * ptr = self->head; ptr; ptr = ptr->next){
        for(const struct adjacency_node * a_ptr = ptr->head; a_ptr; a_ptr = a_ptr->next){
            printf("i:%s    j:%s\n", ptr->value, a_ptr->value);
        }
    }
}