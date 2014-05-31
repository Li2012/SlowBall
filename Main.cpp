#include "config.h"
#include "game.h"
#include "map.h"

#include <iostream>

int main() {
  Game g;
  Map m(15, 10);
  g.RenderMap(m);
  g.RunGame();
  return 0;
}
