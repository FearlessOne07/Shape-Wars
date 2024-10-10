#pragma once

#include "raylib.h"
struct RenderContext {
  float gameWidth;
  float gameHeight;
  float marginX;
  float marginY;
  float scale;
  mutable Camera2D camera;
};
