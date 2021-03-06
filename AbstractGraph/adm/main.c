#include <stdio.h>
#include <stdlib.h>
#include "ADM.h"
//#include "ADL.h"

int main(){
    graph *g = graph_create();
    printf("Empty: %d\n", graph_empty(g));
    graph_add_node(g, "5");
    graph_add_node(g, "11");
    graph_add_node(g, "2");
    graph_add_node(g, "7");
    graph_add_node(g, "8");
    graph_add_node(g, "9");
    graph_add_node(g, "3");
    graph_add_node(g, "10");
    graph_add_edge(g, "5", "11");
    graph_add_edge(g, "11", "2");
    graph_add_edge(g, "7", "11");
    graph_add_edge(g, "7", "8");
    graph_add_edge(g, "11", "10");
    graph_add_edge(g, "11", "9");
    graph_add_edge(g, "8", "9");
    graph_add_edge(g, "3", "8");
    graph_add_edge(g, "3", "10");
    graph_print(g);
    graph_clear(g);
    printf("Empty: %d\n", graph_empty(g));

    printf("\n\n");

    graph *d = graph_create();
    printf("Empty: %d\n", graph_empty(g));
    graph_add_node(d, "Unterhose");
    graph_add_node(d, "Hose");
    graph_add_node(d, "Mantel");
    graph_add_node(d, "Pullover");
    graph_add_node(d, "Unterhemd");
    graph_add_node(d, "Schuhe");
    graph_add_node(d, "Socken");
    graph_add_edge(d, "Unterhose", "Hose");
    graph_add_edge(d, "Hose", "Mantel");
    graph_add_edge(d, "Pullover", "Mantel");
    graph_add_edge(d, "Unterhemd", "Pullover");
    graph_add_edge(d, "Hose", "Schuhe");
    graph_add_edge(d, "Socken", "Schuhe");
    graph_print(d);
    graph_clear(d);
    printf("Empty: %d\n", graph_empty(d));

    printf("\n\n");

    graph *a = graph_create();
    printf("Empty: %d\n", graph_empty(a));
    graph_add_node(a, "A");
    graph_add_node(a, "B");
    graph_add_node(a, "C");
    graph_add_node(a, "D");
    graph_add_node(a, "E");
    graph_add_edge(a, "A", "B");
    graph_add_edge(a, "A", "C");
    graph_add_edge(a, "A", "D");
    graph_add_edge(a, "A", "E");
    graph_add_edge(a, "B", "D");
    graph_add_edge(a, "C", "B");
    graph_add_edge(a, "C", "E");
    graph_delete_node(a, "A");
    graph_print(a);
    graph_clear(a);
    printf("Empty: %d\n", graph_empty(a));


    graph_destroy(d);
    graph_destroy(a);
    graph_destroy(g);
}