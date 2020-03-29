#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h>
#include <time.h>

#include "gamelib.h"

using namespace std;

void draw_player(SDL_Renderer *p_renderer, coord_t position) {
  draw_rectangle(p_renderer, position, {.r=255,.g=0,.b=0});
}
void move_missile(missile_t *missile, int amount) {
  missile->coord.y += amount;
}
void draw_missile(SDL_Renderer *p_renderer, coord_t position) {
  draw_rectangle(p_renderer, position, {.r=0,.g=0,.b=255});
}
void render(void *data) {
  game_t *game = (game_t*)data;
  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
  SDL_RenderClear(game->renderer);
  draw_player(game->renderer, game->player.position);
  for(int i=0;i<N_MISSILES;i++) {
    if(game->missiles[i].valid) {
      draw_missile(game->renderer,game->missiles[i].coord);
    }
  }
  if(game->lives>0) {
    char buffer[256] = {0};
    sprintf(buffer, "score: %d", game->score);
    draw_text(
              game->renderer,
              {.x=10,.y=10,.w=100,.h=50},
              {.r=255,.g=255,.b=255},
              buffer);
    sprintf(buffer, "lives: %d", game->lives);
    draw_text(
              game->renderer,
              {.x=10,.y=60,.w=100,.h=50},
              {.r=255,.g=255,.b=255},
              buffer);
  } else {
    char buffer[256] = {0};
    sprintf(buffer, "score: %d", game->score);
    draw_text(
              game->renderer,
              {.x=SCREEN_WIDTH/2,.y=SCREEN_HEIGHT/2+60,.w=100,.h=50},
              {.r=255,.g=255,.b=255},
              buffer);
    draw_text(
              game->renderer,
              {.x=SCREEN_WIDTH/2,.y=SCREEN_HEIGHT/2,.w=100,.h=50},
              {.r=255,.g=255,.b=255},
              "Game Over");
    draw_text(
              game->renderer,
              {.x=SCREEN_WIDTH/2,.y=SCREEN_HEIGHT/2+120,.w=100,.h=50},
              {.r=255,.g=255,.b=255},
              "Press Esc to Quit");
  }
  SDL_RenderPresent(game->renderer);
}
int collide(coord_t *a, coord_t *b) {
  int left_side = a->x < b->x;
  int right_side = a->x > b->x;
  int top = a->y < b->y;
  if(top) {
    if(left_side) {
      if(a->x+a->w > b->x)
        return 1;
    }
    else if(right_side) {
      if(a->x < b->x+b->w)
        return 1;
    }
  }
  return 0;
}

int missile_timer = 1;
unsigned int logic(unsigned int t, void *data) {
  game_t *game = (game_t*)data;
  if(game->tick_counter%15==0 && game->lives>0) {
    // move stuff
    switch(game->player.control.direction) {
    case CONTROL_DIR_LEFT:
      game->player.position.x-=10;
      break;
    case CONTROL_DIR_RIGHT:
      game->player.position.x+=10;
      break;
    }

    for(int i=0;i<N_MISSILES;i++) {
      if(game->missiles[i].valid) {
        move_missile(&game->missiles[i], game->score/10+2);
      }
    }
    // collision check
    for(int i=0;i<N_MISSILES;i++) {
      missile_t *missile = &game->missiles[i];
      if(missile->valid && collide(&game->player.position,&missile->coord)) {
        missile->valid = 0;
        game->score++;
      }
      if(missile->valid && missile->coord.y > 500) {
        missile->valid = 0;
        game->lives --;
      }
    }
    missile_timer++;

    if((missile_timer%100)==0) {
      for(int i=0;i<N_MISSILES;i++) {
        missile_t *missile = &game->missiles[i];
        if(!missile->valid) {
          missile->valid = 1;
          missile->coord = {.x=rand()%(SCREEN_WIDTH-20)+10,.y=10,.w=10,.h=10};
          break;
        }
      }
    }
  }
  // queue render event
  if(game->tick_counter%15==0) {
    SDL_UserEvent p_userevent;
    p_userevent.type = SDL_USEREVENT;
    p_userevent.code = (game->lives>0?0:GAME_END);
    p_userevent.data1 = (void*)render;
    p_userevent.data2 = data;

    SDL_Event p_event;
    p_event.type = SDL_USEREVENT;
    p_event.user = p_userevent;

    SDL_PushEvent(&p_event);
  }
  game->tick_counter ++;
  return t;
}
int main(int argc, char* args[]) {
  game_main();
  return 0;
}
