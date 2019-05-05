#ifndef ADL_H
#define ADL_H

#include <stdbool.h>
#include <stddef.h>

typedef char * node_value;
typedef struct graph graph;

graph  *    graph_create(void);
void        graph_destroy(graph * self);

bool        graph_empty(const graph * self);
size_t      graph_size(const graph * self);

void        graph_add_node(graph * self, node_value value);
void        graph_delete_node(graph * self, node_value value);

void        graph_add_edge(graph * self, node_value value1, node_value value2);
void        graph_delete_edge(graph * self, node_value value1, node_value value2);

void        graph_clear(graph * self);

void        graph_print(graph * self);

#endif