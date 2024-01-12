#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "triangle.h"

// Array of triangles that should be rendere frame by frame
triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};
mat4_t proj_matrix;

bool is_running = false;

bool CULL_BACKFACE = false;
bool RENDER_WIREFRAME = false;
bool RENDER_FILL = false;
bool RENDER_VERTICES = false;
bool RENDER_TEXTURED = true;

int previous_frame_time = 0;

void setup(void){
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer = (u_int32_t*) malloc(sizeof(u_int32_t) *  window_width * window_height);

  // Creating an SDL texture that is used to display the color_buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  // Initialize the perspective projection matrix
  float fov = M_PI / 3.0; // the same as 180/3, or 60deg
  float aspect = (float)window_width / (float)window_height;
  float znear = 0.1;
  float zfar = 10.0;
  proj_matrix = mat4_make_projection(fov, aspect, znear, zfar);

  // Manually load the hardcoded texture data from the static array
  mesh_texture = (uint32_t*)REDBRICK_TEXTURE;
  texture_width = 64;
  texture_height = 64;

  // Load the cube values in the mesh data structure
  load_cube_mesh_data();
  //load_obj_file_data("./assets/f22.obj");
}

void handle_key_press(SDL_Keycode keycode){
  switch(keycode){
    case SDLK_ESCAPE:
      is_running = false;
      break;
    case SDLK_1:
      CULL_BACKFACE = !CULL_BACKFACE;
      break;
    case SDLK_2:
      RENDER_WIREFRAME = !RENDER_WIREFRAME;
      break;
    case SDLK_3:
      RENDER_FILL = !RENDER_FILL;
      break;
    case SDLK_4:
      RENDER_VERTICES = !RENDER_VERTICES;
      break;
    case SDLK_5:
      RENDER_TEXTURED = !RENDER_TEXTURED;
      break;
  }
}

void process_input(void){
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      handle_key_press(event.key.keysym.sym);
      break;

  }
}

void update(void) {
  //Locking the execution if not enough time passed
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if(time_to_wait>0 && time_to_wait <= FRAME_TARGET_TIME){
    SDL_Delay(time_to_wait);
  }
  previous_frame_time = SDL_GetTicks();

  // Clear the array of triangles to render each frame
  array_free(triangles_to_render);
  triangles_to_render = NULL;


  // Rotate the cube
  mesh.rotation.x += 0.01;
  // mesh.rotation.y += 0.02;
  mesh.rotation.z += 0.01;

  // mesh.scale.x = 0.5;
  // mesh.scale.y = 0.5;
  // mesh.scale.z = 0.5;
  mesh.translation.z = 5;
  // mesh.translation.y +=0.001;

  // Create matrices that will be used to multiply mesh vertices
  mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

  mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
  mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
  mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

  mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        // Loop all three vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // World Matrix combining scale, rotation and translation matrices
            mat4_t world_matrix = mat4_identity();
            // Order matters. First scale, then rotate, and then translate. [T]*[R]*[S]*v
            world_matrix = mat4_mul_mat4(scale_matrix,world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);
           
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            transformed_vertices[j] = vec3_from_vec4(transformed_vertex);
        }

        // Check backface culling
        vec3_t vector_a = transformed_vertices[0]; /*   A   */
        vec3_t vector_b = transformed_vertices[1]; /*  / \  */
        vec3_t vector_c = transformed_vertices[2]; /* C---B */

        // Get the vector subtraction of B-A and C-A
        vec3_t vector_ab = vec3_sub(vector_b, vector_a);
        vec3_t vector_ac = vec3_sub(vector_c, vector_a);
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ac);

        // Compute the face normal (using cross product to find perpendicular)
        vec3_t normal = vec3_cross(vector_ab, vector_ac);
        vec3_normalize(&normal);

        vec3_t camera_ray = vec3_sub(camera_position, vector_a);

        // How aligned the camera ray is with the face normal
        float dot_normal_camera = vec3_dot(normal, camera_ray);

        if(CULL_BACKFACE){
          // Don't render if not facing camera
          if (dot_normal_camera < 0) {
              continue;
          }
        }

        vec4_t projected_points[3];

        // Loop all three vertices to perform projection
        for (int j = 0; j < 3; j++) {
            // Project current point
            projected_points[j] = mat4_mul_vec4_project(proj_matrix, vec4_from_vec3(transformed_vertices[j]));

            // Scale into the view
            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);

            // Invert y values to account for flipped screen y coordinates
            projected_points[j].y *= -1;
                
            // Center points
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);
        }

        // Calculate avg depth based on the vertices Z value after the transformations
        float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

        // Calculate shade intensity based on how aligned is the face normal and light dir
        float light_intensity_factor = -vec3_dot(normal, light.direction);

        // Calculate triangle final color based on light alignment angle
        uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);
 
        triangle_t projected_triangle = {
          .points = {
            {projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
            {projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
            {projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w},
          },
          .texcoords = {
            { mesh_face.a_uv.u, mesh_face.a_uv.v },
            { mesh_face.b_uv.u, mesh_face.b_uv.v },
            { mesh_face.c_uv.u, mesh_face.c_uv.v }
          },
          .color = triangle_color,
          .avg_depth = avg_depth
        };

        array_push(triangles_to_render, projected_triangle);
    }

    // Bubble sort
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++){
      for(int j = i; j < num_triangles; j++){
        if(triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth){
          // Swap the triangles position in the array
          triangle_t temp = triangles_to_render[i];
          triangles_to_render[i] = triangles_to_render[j];
          triangles_to_render[j] = temp;
        }
      }
    }

    // Sort the triangle to render by their avg_depth
}

void render(void){
  // draw_grid(0xFF0000FF, 15);

  // Fill background with gray color
  clear_color_buffer(0xFF999999);

  // Loop all projected triangles and render them
  int num_triangles = array_length(triangles_to_render);

  for (int i = 0; i< num_triangles; i++){
    triangle_t triangle = triangles_to_render[i];

    if(RENDER_VERTICES){
      float vw = 8.0; // vertex width
      //Draw vertex points
      draw_rect(triangle.points[0].x - vw / 2, triangle.points[0].y - vw / 2, vw, vw, 0xFFFFFF00);
      draw_rect(triangle.points[1].x - vw / 2, triangle.points[1].y - vw / 2, vw, vw, 0xFFFFFF00);
      draw_rect(triangle.points[2].x - vw / 2, triangle.points[2].y - vw / 2, vw, vw, 0xFFFFFF00);
    }

    // Draw textured triangle
    if(RENDER_TEXTURED){
       draw_textured_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v, // vertex A
          triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v, // vertex B
          triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v, // vertex C
          mesh_texture
      );
    }

    if(RENDER_FILL){
      // Draw filled triangle
      draw_filled_triangle(
        triangle.points[0].x, triangle.points[0].y,
        triangle.points[1].x, triangle.points[1].y,
        triangle.points[2].x, triangle.points[2].y,
        triangle.color
      );
    }
    if(RENDER_WIREFRAME){
      // Draw unfilled triangle
      draw_triangle(
        triangle.points[0].x, triangle.points[0].y,
        triangle.points[1].x, triangle.points[1].y,
        triangle.points[2].x, triangle.points[2].y,
        0xFF000000
      );
    }

  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);
  SDL_RenderPresent(renderer);
}

// Free memory that was dynamically allocated by the program
void free_resources(void){
  free(color_buffer);
  array_free(mesh.faces);
  array_free(mesh.vertices);
  array_free(triangles_to_render);
}

int main (void) {
  is_running = initialize_window();


  setup();

 

  while (is_running){
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();

  return 0;
}