#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ADM.h"

struct node {
    node_value value;
};

struct matrix_graph {
    bool * matrix;
    struct node * payload;
};

struct graph { 
    struct matrix_graph * head;
    size_t count;
};


graph  *    graph_create(void){
    graph * self = (graph *)malloc(sizeof(graph));
    assert(self);
    self->head = (struct matrix_graph *)malloc(sizeof(struct matrix_graph));
    assert(self->head);
    self->count = 0;
    return self;
}

void        graph_destroy(graph * self){
    graph_clear(self);
    free(self);
}

bool        graph_empty(const graph * self){
    return self == NULL || self->count == 0;
}

size_t      graph_size(const graph * self){
    assert(self);
    return self->count;
}

static bool check_node_exists(graph * self, node_value value){
    struct matrix_graph * ptr = self->head;
    for(size_t i = 0; i < self->count; i++){
        if(strcmp(ptr->payload[i].value, value) == 0){
            return true;
        }
    }   
    return false;
}

void        graph_add_node(graph * self, node_value value){
    assert(self);
    if(check_node_exists(self, value) == false){
        self->count++;
        //payload array
        struct matrix_graph * ptr = self->head;
        struct node * temp_payload = (struct node *)malloc(sizeof(struct node) * (self->count));
        assert(temp_payload);
        for(size_t i = 0; i < (self->count-1); i++){
            temp_payload[i] = ptr->payload[i];
        }
        temp_payload[self->count-1].value = (node_value)malloc(sizeof(char) * (size_t)strlen(value));
        assert(temp_payload[self->count-1].value);
        strcpy(temp_payload[self->count-1].value, value);
        ptr->payload = temp_payload;

        //matrix array
        bool * temp_matrix = (bool *)malloc(sizeof(bool) * ((self->count) * (self->count)));
        assert(temp_matrix);
            for (size_t i = 0; i < (self->count-1); i++)
            {
                for (size_t j = 0; j < (self->count-1);j++){
                    //row * N + col
                    temp_matrix[i * (self->count) + j] = ptr->matrix[i * (self->count - 1) + j];
                }
            }
            for(size_t i = 0; i < (self->count); i++){
                temp_matrix[(self->count-1) * (self->count) + i] = false;
            }
            for(size_t i = 0; i < (self->count); i++){
                temp_matrix[i * (self->count) + (self->count-1)] = false;
            }
        ptr->matrix = temp_matrix;
    } else {
        printf("Exists already!");
    }
}

static size_t find_value_pos(graph * self, node_value value){
    struct matrix_graph * ptr = self->head;
    for(size_t i = 0; i < self->count; i++){
        if(strcmp(ptr->payload[i].value, value) == 0){
            return i;
        }
    }   
    return 0;
}

void        graph_delete_node(graph * self, node_value value){
    assert(self);
    assert(!graph_empty(self));

    struct matrix_graph * ptr = self->head;
    struct node * temp_payload = (struct node *)malloc(sizeof(struct node) * (self->count-1));//new temp with size-1
    assert(temp_payload);
    bool * temp_matrix = (bool *)malloc(sizeof(bool) * ((self->count-1) * (self->count-1)));
    assert(temp_matrix);
    
    
    if(check_node_exists(self, value)){
        //payload array
        size_t delete_node_pos = find_value_pos(self, value);
        size_t bigger_counter = 0;
        for(size_t i = 0; i < (self->count)-1; i++){
            if(strcmp(ptr->payload[bigger_counter].value, value)== 0){
                bigger_counter++;
            }
            temp_payload[i].value = ptr->payload[bigger_counter].value;
            bigger_counter++;
        }
        ptr->payload = temp_payload;

        //matrix array
        size_t bigger_j = 0;
        size_t bigger_i = 0;
        for(size_t i = 0; i < self->count; i++){
            if(delete_node_pos == i){
                continue;
            }
            for(size_t j = 0; j < self->count; j++){
                if(delete_node_pos == j){
                    continue;
                }
                temp_matrix[bigger_i * (self->count-1) + bigger_j] = ptr->matrix[i * (self->count) + j];
                bigger_j++;
            }
            bigger_i++;
        }
        ptr->matrix = temp_matrix;

        self->count--;
    } else {
        printf("can not delete\n");
    }


}

void        graph_add_edge(graph * self, node_value value1, node_value value2){
    assert(self);
    struct matrix_graph * ptr = self->head;
    if(check_node_exists(self, value1) && check_node_exists(self, value2)){
        size_t value1_pos = find_value_pos(self, value1);
        size_t value2_pos = find_value_pos(self, value2);
        ptr->matrix[value1_pos * (self->count) + value2_pos] = true;
    } else {
        printf("can not add edge\n");
    }
}

void        graph_delete_edge(graph * self, node_value value1, node_value value2){
    assert(self);
    struct matrix_graph * ptr = self->head;
    if(check_node_exists(self, value1) && check_node_exists(self, value2)){
        size_t value1_pos = find_value_pos(self, value1);
        size_t value2_pos = find_value_pos(self, value2);
        ptr->matrix[value1_pos * (self->count) + value2_pos] = false;
    } else {
        printf("can not add edge\n");
    }
}

void        graph_clear(graph * self){
    assert(self);
    free(self->head->payload);
    free(self->head->matrix);
    free(self->head);
    self->count = 0;
}

void        graph_print(graph * self){
    assert(self);
    struct matrix_graph * ptr = self->head; 
    for(size_t i = 0; i < self->count; i++){
        printf("\nNODE: %s\n", ptr->payload[i].value);
    }
    printf("\n");
    for (size_t i = 0; i < (self->count); i++){
        for (size_t j = 0; j < (self->count); j++){
            //row * N + col
            if (ptr->matrix[i * (self->count) + j] == true){
                printf("i:%s    j:%s\n", ptr->payload[i].value, ptr->payload[j].value);
            }
        }
    }
}