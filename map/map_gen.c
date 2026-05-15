#include "map.h"
#include <stdlib.h>
#include <string.h>

void Map_generate(Map *m, int seed) {
    srand(seed);
    m->lin = MAP_LINHAS;
    m->col = MAP_COLUNAS;

    memset(m->grid, 0, sizeof(m->grid));
    // Paredes Solidas 
    for(int x = 0; x< m->col; x++){
        m->grid[0][x] = 1;
        m->grid[m->lin -1][x] = 1;
    }
    for(int y = 0; y< m->lin; y++){
        m->grid[y][0] = 1;
        m->grid[y][m->col -1] = 1;
    }
    // Gerar paredes internas aleatórias 20%
     for (int y = 2; y < MAP_LINHAS-2; y++){
        for (int x = 2; x < MAP_COLUNAS-2; x++){{
            if (rand() % 5 == 0)
                m->grid[y][x] = 1 + rand() % 3; // tipo 1, 2 ou 3
        }
    }
    m->grid[1][1]= 0; // Garantir posição inicial livre
    m->grid[2][1]= 0;
    m->grid[1][2]= 0;
}