#pragma once

struct BoundingBox
{
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;

  void clipTo(const BoundingBox& other);
};
