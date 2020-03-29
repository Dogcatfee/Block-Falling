#ifndef GAMELIB_H
#define GAMELIB_H
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define N_MISSILES 32
#define GAME_END 1

typedef SDL_Rect coord_t;

typedef struct color_s {
  uint8_t r,g,b;
} color_t;

typedef struct control_s {
  uint8_t direction;
} control_t;

typedef struct player_s {
  coord_t position;
  control_s control;
} player_t;

typedef struct missile_s {
  coord_t coord;
  int valid;
} missile_t;

typedef struct game_s {
  SDL_Renderer *renderer;
  player_t player;
  missile_t missiles[N_MISSILES];
  int lives = 10;
  int score = 0;
  int tick_counter = 0;
} game_t;

enum CONTROL_DIRECTIONS {
                         CONTROL_DIR_NONE,
                         CONTROL_DIR_LEFT,
                         CONTROL_DIR_RIGHT,
                         CONTROL_DIR_UP,
                         CONTROL_DIR_DOWN,
};


void draw_rectangle(SDL_Renderer *p_renderer, SDL_Rect rect, color_t color);
void render(void *data);
void game_main();
unsigned int logic(unsigned int t, void *data);
void draw_text(SDL_Renderer *p_renderer, SDL_Rect rect, color_t color, char*text);
#endif
