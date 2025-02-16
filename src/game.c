#include "game.h"


int main()
{
    int screen_width = 500;
    int screen_height = 500;

    InitWindow(screen_width, screen_height, "QuadTree");

    Rectangle boundary = (Rectangle){ 0, 0, screen_width, screen_height };
    QuadTree* quad_tree = quadtree_create(boundary);

    float add_time = 1/20.f;
    float current_add_time = 0.f;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        current_add_time += GetFrameTime();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && current_add_time >= add_time)
        {
            Vector2 mp = GetMousePosition();
            Vector2* pos = (Vector2*)malloc(sizeof(Vector2));
            pos->x = mp.x;
            pos->y = mp.y;
            quadtree_insert(quad_tree, pos);
            current_add_time = 0.f;
        }
        BeginDrawing();
        ClearBackground(BLACK);

        draw_quad_tree(*quad_tree);
        draw_quad_points(*quad_tree);

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


void array_init(Array* array, int item_size, int capacity)
{
    array->items = malloc(capacity * item_size);
    if (array->items == NULL)
    {
        printf("array init items are NULL");
        exit(0);
    }
    array->size = 0;
    array->capacity = capacity;
    array->item_size = item_size;
}


void array_append(Array* array, void* item)
{
    if (array->capacity == 0)
    {
        array->capacity = 256;
        array->items = malloc(array->capacity * array->item_size);
        if (array->items == NULL)
        {
            printf("array items are NULL malloc");
            exit(0);
        }
    }

    if (array->size >= array->capacity)
    {
        array->capacity *= 2;
        array->items = realloc(array->items, array->capacity * array->item_size);
        if (array->items == NULL)
        {
            printf("array items are NULL realloc");
            exit(0);
        }
    }

    array->items[array->size] = item;
    array->size++;
}

void array_free(Array* array)
{
    for (size_t i = 0; i < array->size; i++)
    {
        free(array->items[i]);
    }
    free(array->items);
    array->items = NULL;
    array->size = 0;
    array->capacity = 0;
}

QuadTree* quadtree_create(Rectangle boundary)
{
    QuadTree* quad_tree = (QuadTree*)malloc(sizeof(QuadTree));
    if (quad_tree == NULL)
    {
        exit(0);
    }
    quad_tree->boundary = boundary;
    quad_tree->capacity = 4;
    quad_tree->divided = false;

    quad_tree->points = malloc(sizeof(Array));
    array_init(quad_tree->points, sizeof(Vector2), quad_tree->capacity);

    quad_tree->top_left = NULL;
    quad_tree->top_right = NULL;
    quad_tree->bottom_left = NULL;
    quad_tree->bottom_right = NULL;

    return quad_tree;
}

void quadtree_insert(QuadTree* quad_tree, Vector2* point)
{

    if (!quadtree_boundary_contains(quad_tree->boundary, *point))
    {
        return;
    }

    if (quad_tree->points->size < quad_tree->capacity)
    {
        array_append(quad_tree->points, point);
    }
    else
    {
        if (!quad_tree->divided)
        {
            quadtree_subdivide(quad_tree);
        }

        quadtree_insert(quad_tree->top_left, point);
        quadtree_insert(quad_tree->top_right, point);
        quadtree_insert(quad_tree->bottom_left, point);
        quadtree_insert(quad_tree->bottom_right, point);
    }
}

void quadtree_subdivide(QuadTree* quad_tree)
{
    float x = quad_tree->boundary.x;
    float y = quad_tree->boundary.y;
    float half_width = quad_tree->boundary.width / 2;
    float half_height = quad_tree->boundary.height / 2;

    Rectangle tl_rect = { x, y, half_width, half_height };
    Rectangle tr_rect = { x + half_width, y, half_width, half_height };
    Rectangle bl_rect = { x, y + half_height, half_width, half_height };
    Rectangle br_rect = { x + half_width, y + half_height, half_width, half_height };

    quad_tree->top_left = quadtree_create(tl_rect);
    quad_tree->top_right = quadtree_create(tr_rect);
    quad_tree->bottom_left = quadtree_create(bl_rect);
    quad_tree->bottom_right = quadtree_create(br_rect);

    quad_tree->divided = true;
}

bool quadtree_boundary_contains(Rectangle rect, Vector2 point)
{
    return (
        point.x >= rect.x - rect.width &&
        point.x <= rect.x + rect.width &&
        point.y >= rect.y - rect.height &&
        point.y <= rect.y + rect.height
        );
}

void draw_quad_tree(QuadTree quad_tree)
{
    DrawRectangleLines(quad_tree.boundary.x, quad_tree.boundary.y, quad_tree.boundary.width, quad_tree.boundary.height, WHITE);

    if (quad_tree.divided)
    {
        draw_quad_tree(*quad_tree.top_left);
        draw_quad_tree(*quad_tree.top_right);
        draw_quad_tree(*quad_tree.bottom_left);
        draw_quad_tree(*quad_tree.bottom_right);
    }
}

void draw_quad_points(QuadTree quad_tree)
{

    Array* points = quad_tree.points;

    for (size_t i = 0; i < points->size; i++)
    {
        Vector2 pos = *(Vector2*)points->items[i];
        DrawCircleV(pos, 1.f, RED);
    }

    if (quad_tree.divided)
    {
        draw_quad_points(*quad_tree.top_left);
        draw_quad_points(*quad_tree.top_right);
        draw_quad_points(*quad_tree.bottom_left);
        draw_quad_points(*quad_tree.bottom_right);
    }
}