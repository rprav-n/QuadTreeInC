// MARK: - Includes
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


typedef struct
{
    void** items;
    int size;
    int capacity;
    int item_size;
}Array;



typedef struct QuadTree
{
    bool divided;
    Rectangle boundary;
    Array* points;
    int capacity;

    struct QuadTree* top_left;
    struct QuadTree* top_right;
    struct QuadTree* bottom_left;
    struct QuadTree* bottom_right;
} QuadTree;


void array_init(Array* array, int item_size, int capacity);
void array_append(Array* array, void* item);
void array_free(Array* array);


QuadTree* quadtree_create(Rectangle boundary);
void quadtree_insert(QuadTree* quad_tree, Vector2* point);
void quadtree_subdivide(QuadTree* quad_tree);
bool quadtree_boundary_contains(Rectangle rect, Vector2 point);
void draw_quad_tree(QuadTree quad_tree);
void draw_quad_points(QuadTree quad_tree);