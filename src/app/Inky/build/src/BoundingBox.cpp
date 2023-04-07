#include "BoundingBox.hpp"
#include <cmath>
#include <algorithm>

void BoundingBox::clipTo(const BoundingBox& other)
{
  if (other.x > x)
  {
    width -= (other.x-x);
    x = other.x;
  }
  if (other.y > y)
  {
    height -= (other.y-y);
    y = other.y;
  }
  if (x+width > other.x+other.width)
  {
    width -= ((x + width) - (other.x + other.width));
  }
  if (y+height > other.y+other.height)
  {
    height -= ((y + height) - (other.y + other.height));
  }
  width = std::max(0, width);
  height = std::max(0, height);
}