#include "game.h"

// #include <SDL2/SDL_image.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "battle_instance.h"

DEFINE_int32(window_width, 800, "window width");
DEFINE_int32(window_height, 600, "window height");

Game::Game()
    // : map_(0, 0)
{
  CHECK((FLAGS_window_width == 800 && FLAGS_window_height == 600) ||
        (FLAGS_window_width == 1366 && FLAGS_window_height == 768))
      << "wrong window width and height";

  window_ = SDL_CreateWindow("SlowBall",          // titile
                             100,                 // x
                             100,                 // y
                             FLAGS_window_width,  // w
                             FLAGS_window_height, // h
                             SDL_WINDOW_OPENGL);  // flags

  CHECK(window_ != nullptr) << "NULL window_";

  display_.CreateRenderer(window_);

}

Game::~Game() {
  SDL_DestroyWindow(window_);
}

int Game::RunGame() {
  bool done = false;
  SDL_Event event;
  int prev_tick = SDL_GetTicks();
  int current_tick = SDL_GetTicks();
  mouse_x_ = 0;
  mouse_y_ = 0;

  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ) {
        LOG(INFO) << "quit event";
        done = true;
      }

      else if (event.type == SDL_MOUSEMOTION) {
        SDL_MouseMotionEvent mouse_motion_event = event.motion;
        mouse_x_ = mouse_motion_event.x;
        mouse_y_ = mouse_motion_event.y;
      }

      else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        SDL_KeyboardEvent keyboard_event = event.key;
        SDL_Keysym key_symbol = keyboard_event.keysym;

        // can call SDL_GetScancodeName(keyboard_event.keysym.scancode)
        // and SDL_GetKeyName(keyboard_event.keysym.sym) to get symbols.
        switch (key_symbol.sym) {
        case SDLK_ESCAPE:
          LOG(INFO) << "keyboard quit event";
          done = true;
          break;
        default:
           // LOG(INFO) << "scan code: " << SDL_GetScancodeName(keyboard_event.keysym.scancode)
           //           << "key name: " << SDL_GetKeyName(keyboard_event.keysym.sym);
          break;
        }
      }
      else {
        LOG(INFO) << "in default";
      }
    }
    current_tick = SDL_GetTicks();
    if (current_tick - prev_tick > 5) {
       LOG(INFO) << "in while loop before delay and prev tick is "
                 << prev_tick << " and current tick is: " << current_tick
                 << " and diff is " << current_tick - prev_tick;
    }
    int delay = kDelayBase - (current_tick - prev_tick);
    if (delay > 0) {
      SDL_Delay(delay);
    }
    prev_tick = SDL_GetTicks();
    // LOG(INFO) << "in while loop after delay and prev tick is "
    //           << prev_tick << " and current tick is: " << current_tick;

  }
  return 0;
}


void Game::RunBattleInstance(BattleInstance* battle_instance) {
  display_.ClearBattleInstanceOffset();
  mouse_x_ = 0;
  mouse_y_ = 0;

  SDL_Event event;
  int prev_tick = SDL_GetTicks();
  int current_tick = SDL_GetTicks();

  Location src, dest;

  display_.RenderBattleInstance(*battle_instance, &config_);

  while (!battle_instance->battle_done_) {
    display_.RenderBattleInstance(*battle_instance, &config_);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ) {
        LOG(INFO) << "quit event, quiting battle";
        battle_instance->battle_done_ = true;
      }
      else if (event.type == SDL_MOUSEMOTION) {
        SDL_MouseMotionEvent mouse_motion_event = event.motion;
        mouse_x_ = mouse_motion_event.x;
        mouse_y_ = mouse_motion_event.y;
        //Location l = display_.PointToLocation(Point(mouse_x_, mouse_y_));
        //display_.RenderLocationInfo(l);
        display_.UpdateMousePoint(Point(mouse_x_, mouse_y_));
        //VLOG(2) << l;
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_MouseButtonEvent mouse_button_event = event.button;
        if (mouse_button_event.button == SDL_BUTTON_LEFT) {
          mouse_x_ = mouse_button_event.x;
          mouse_y_ = mouse_button_event.y;
          src = display_.PointToLocation(Point(mouse_x_, mouse_y_));
        }
      }
      else if (event.type == SDL_MOUSEBUTTONUP) {
        SDL_MouseButtonEvent mouse_button_event = event.button;
        if (mouse_button_event.button == SDL_BUTTON_LEFT) {
          mouse_x_ = mouse_button_event.x;
          mouse_y_ = mouse_button_event.y;
          dest = display_.PointToLocation(Point(mouse_x_, mouse_y_));
          VLOG(2) << "moving from " << src << " to " << dest;
          battle_instance->MoveUnitGroup(src, dest);
        }
      }
      else {

      }
    }
    display_.UpdateBattleInstanceOffset(*battle_instance, mouse_x_, mouse_y_);
    current_tick = SDL_GetTicks();
    if (current_tick - prev_tick > 5) {
       LOG(INFO) << "in while loop before delay and prev tick is "
                 << prev_tick << " and current tick is: " << current_tick
                 << " and diff is " << current_tick - prev_tick;
    }
    int delay = kDelayBase - (current_tick - prev_tick);
    if (delay > 0) {
      SDL_Delay(delay);
    }
    prev_tick = SDL_GetTicks();
  }

  return;
}

