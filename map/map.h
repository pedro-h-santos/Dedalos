#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#define MAP_COLUNAS 24
#define MAP_LINHAS 12
typedef struct {
    int grid[MAP_LINHAS][MAP_COLUNAS];
    int linhas;
    int colunas;
} Map;
int Map_getgrid(Map *m, int x, int y);
bool Map_is_walkable(Map *m, float x, float y);
void Map_init(Map *m);
void Map_generate(Map *m, int seed);

#endif