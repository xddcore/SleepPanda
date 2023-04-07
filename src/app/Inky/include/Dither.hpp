#pragma once

#include "Color.hpp"
#include "Image.hpp"

// These 
typedef IndexedColor indexedColorFromRGBA(const RGBAColor&);

void patternDither(const Image& sourceImage, Image& destImage);
void diffusionDither(const Image& sourceImage, Image& destImage, float ditherAccuracy = 1.0f);