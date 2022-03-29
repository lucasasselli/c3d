#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "unistd.h"
#include "string.h"
#include "time.h"

#include "gfx.h"

#define SCREEN_SIZE_X 600
#define SCREEN_SIZE_Y 600

#define BUFFER_SIZE 100000

typedef union
{
    struct
    {
        float x;
        float y;
        float z;
        float w;
    } coord;
    float array[4];
} vertex_t;

typedef unsigned int face_t[3];

vertex_t o_buff[BUFFER_SIZE];
vertex_t v_buff[BUFFER_SIZE];
int v_buff_cnt = 0;

face_t f_buff[BUFFER_SIZE];
int f_buff_cnt = 0;

float c_buff[SCREEN_SIZE_Y][SCREEN_SIZE_X];
float z_buff[SCREEN_SIZE_Y][SCREEN_SIZE_X];


void load_obj_file(char* path)
{
    char a;
    float x, y, z;

    FILE *f = fopen(path, "r");

    // Read object file
    while (fscanf(f, "%c %f %f %f\n", &a, &x, &y, &z) != EOF)
    {
        switch (a)
        {
            case 'v':
                // Vertex
                o_buff[v_buff_cnt] = (vertex_t){x, y, z, 1};
                v_buff_cnt++;
                break;

            case 'f':
                // Face
                f_buff[f_buff_cnt][0] = x;
                f_buff[f_buff_cnt][1] = y;
                f_buff[f_buff_cnt][2] = z;
                f_buff_cnt++;
                break;
        }
    }
}

void convert_to_ndc(float camera_fov, float camera_ratio, float clip_near, float clip_far)
{
    for (int i = 0; i < v_buff_cnt; i++)
    {
        v_buff[i].coord.x /= -v_buff[i].coord.z;
        v_buff[i].coord.y /= -v_buff[i].coord.z;

        v_buff[i].coord.x *= 1.0 / tan(camera_fov / 2);
        v_buff[i].coord.y *= 1.0 / (tan(camera_fov / 2) * camera_ratio);
        v_buff[i].coord.z *= 1.0 / (clip_far - clip_near);
        v_buff[i].coord.z += -(2*clip_near) / (clip_far - clip_near);
    }
}

void convert_to_raster()
{
    for (int i = 0; i < v_buff_cnt; i++)
    {
        v_buff[i].coord.x *= 0.5 * ((float) SCREEN_SIZE_X);
        v_buff[i].coord.x += 0.5 * ((float) SCREEN_SIZE_X);
        v_buff[i].coord.y *= -0.5 * ((float) SCREEN_SIZE_Y);
        v_buff[i].coord.y += 0.5 * ((float) SCREEN_SIZE_Y);
    }
}

void vertex_scale(vertex_t* v, float k)
{
    v->coord.x *= k;
    v->coord.y *= k;
    v->coord.z *= k;
}

void vertex_move(vertex_t* v, float x, float y, float z, float r)
{
    float r_x = v->coord.x * cos(r) - v->coord.z * sin(r);
    float r_z = v->coord.x * sin(r) + v->coord.z * cos(r);

    v->coord.x = r_x;
    v->coord.z = r_z;

    v->coord.x += x;
    v->coord.y += y;
    v->coord.z += z;
}

static inline float edge_funct(vertex_t a, vertex_t b, vertex_t c)
{
    return (c.coord.x - a.coord.x) * (b.coord.y - a.coord.y) - (c.coord.y - a.coord.y) * (b.coord.x - a.coord.x);
}

void dump_vertices()
{
    for (int i = 0; i < v_buff_cnt; i++)
    {
        printf("%f %f %f %f\n", v_buff[i].coord.x, v_buff[i].coord.y, v_buff[i].coord.z, v_buff[i].coord.w);
    }
    printf("----------------------------------------------------------------\n");
}

void raster()
{
    vertex_t v0, v1, v2;
    float w0, w1, w2;

    for (int i = 0; i < SCREEN_SIZE_X; i++)
    {
        for (int j = 0; j < SCREEN_SIZE_Y; j++)
        {
            c_buff[i][j] = 0;
            z_buff[i][j] = -1;
        }
    }

    for (int f = 0; f < f_buff_cnt; f++)
    {
        v0 = v_buff[f_buff[f][0]-1];
        v1 = v_buff[f_buff[f][1]-1];
        v2 = v_buff[f_buff[f][2]-1];

        // Culling
        vertex_t p = {(v0.coord.x + v1.coord.x + v2.coord.x)/3.0, (v0.coord.y + v1.coord.y + v2.coord.y)/3.0, 0, 0};

        w0 = edge_funct(v1, v2, p);
        w1 = edge_funct(v2, v0, p);
        w2 = edge_funct(v0, v1, p);

        if (w0 < 0  || w1 < 0 || w2 < 0)
            continue;

        // Calculate min bounding rectangle
        float mbr_min_x = fmin(v0.coord.x, fmin(v1.coord.x, v2.coord.x));
        float mbr_max_x = fmax(v0.coord.x, fmax(v1.coord.x, v2.coord.x));
        float mbr_min_y = fmin(v0.coord.y, fmin(v1.coord.y, v2.coord.y));
        float mbr_max_y = fmax(v0.coord.y, fmax(v1.coord.y, v2.coord.y));

        mbr_min_x = mbr_min_x > 0 ? mbr_min_x : 0;
        mbr_max_x = mbr_max_x < SCREEN_SIZE_X ? mbr_max_x : SCREEN_SIZE_X;
        mbr_min_y = mbr_min_y > 0 ? mbr_min_y : 0;
        mbr_max_y = mbr_max_y < SCREEN_SIZE_Y ? mbr_max_y : SCREEN_SIZE_X;

        float a = edge_funct(v0, v1, v2);

        for (int i = mbr_min_x; i < mbr_max_x; i++)
        {
            for (int j = mbr_min_y; j < mbr_max_y; j++)
            {
                p = (vertex_t){i, j, 0, 1};

                w0 = edge_funct(v1, v2, p);
                w1 = edge_funct(v2, v0, p);
                w2 = edge_funct(v0, v1, p);

                if (w0 >= 0 && w1 >= 0 && w2 >= 0)
                {
                    // Compute Z
                    w0 /= a;
                    w1 /= a;
                    w2 /= a;

                    float z = 1.0/(w0*(1.0/v0.coord.z) + w1*(1.0/v1.coord.z) + w2*(1.0/v2.coord.z));

                    if (z > z_buff[i][j])
                    {
                        z_buff[i][j] = z;
                        c_buff[i][j] = 255 * (z + 1)/2;
                    }
                }
            }
        }
    }
}

void gfx_draw()
{
    for (int i = 0; i < SCREEN_SIZE_X; i++)
    {
        for (int j = 0; j < SCREEN_SIZE_Y; j++)
        {
            int c = c_buff[i][j];
            gfx_color(c, c, c);
            gfx_point(i, j);
        }
    }
}

int main()
{
    float r = 0;

    // Open a new window for drawing.
    gfx_open(SCREEN_SIZE_X, SCREEN_SIZE_Y, "graphics_pipeline");

    load_obj_file("objects/utah.obj");

    while (1)
    {
        memcpy(v_buff, o_buff, BUFFER_SIZE * sizeof(vertex_t));

        for (int i = 0; i < v_buff_cnt; i++)
        {
            vertex_scale(&v_buff[i], 1);
            vertex_move(&v_buff[i], 0, 0, -9, r);
        }

        convert_to_ndc(M_PI / 3, 1.0, 1, 20);
        // TODO: Clipping
        convert_to_raster();

        raster();

        gfx_draw();


        r += M_PI / 500;

        usleep(10e3);
    }

    return 0;
}