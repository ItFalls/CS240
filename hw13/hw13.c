#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "graphics.h"

object *read_object(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }
  if (feof(file)) {
    fclose(file);
    return NULL;
  }
  char name[1024];
  int scan = fscanf(file, "%s\n", name);
  if (scan != 1) {
    fclose(file);
    return NULL;
  }

  object *obj = malloc(sizeof(object));
  assert(obj != NULL);

  scan = fscanf(file, "%d %d\n", &obj->num_points, &obj->num_polygons);
  if (scan != 2) {
    fclose(file);
    return NULL;
  }

  obj->points = malloc(sizeof(point) * obj->num_points);
  assert(obj->points != NULL);

  obj->polygons = malloc(sizeof(polygon) * obj->num_polygons);
  assert(obj->polygons != NULL);

  int first = 0;
  for (int i = 0; i < obj->num_points; i++) {
    int temp = -1;
    scan = fscanf(file, "%d %lf %lf %lf\n", &temp, &obj->points[i].arr[0],
                  &obj->points[i].arr[1], &obj->points[i].arr[2]);
    if (scan != 4) {
      fclose(file);
      return NULL;
    }
    if (i == 0) {
      first = temp;
    }
  }

  for (int i = 0; i < obj->num_polygons; i++) {
    char poly_name[7];
    char color_name[7];

    scan = fscanf(file, "%[^ ] %[^ ]", poly_name, color_name);
    if (scan != 2) {
      fclose(file);
      return NULL;
    }

    name_to_rgb(color_name, &obj->polygons[i].r, &obj->polygons[i].g,
                &obj->polygons[i].b);

    scan = fscanf(file, " %d", &obj->polygons[i].num_points);
    if (scan != 1) {
      fclose(file);
      return NULL;
    }
    obj->polygons[i].point_arr =
        malloc(sizeof(point *) * obj->polygons[i].num_points);
    assert(obj->polygons[i].point_arr != NULL);

    for (int j = 0; j < obj->polygons[i].num_points; j++) {
      int point_index;
      scan = fscanf(file,  " %d", &point_index);
      if (scan != 1) {
        fclose(file);
        return NULL;
      }
      obj->polygons[i].point_arr[j] = &obj->points[point_index - first];
    }
    fscanf(file, "\n");
  }
  fclose(file);
  return obj;
}

void free_object(object *obj) {
  free(obj->points);
  for (int i = 0; i < obj->num_polygons; i++)
    free(obj->polygons[i].point_arr);
  free(obj->polygons);
  free(obj);
}
