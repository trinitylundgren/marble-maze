
#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <ostream>
#include <map>

#define HEIGHT 64
#define WIDTH 64

struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

enum FEATURE {EMPTY, WALL, SAND};
static std::map<struct pixel, FEATURE> pixelsToFeatures {
    {{0, 0, 0}, EMPTY}, {{255, 255, 255}, WALL}, {{127, 0, 0}, SAND}
};

static std::map<FEATURE, std::string> featuresToChars {
    {EMPTY, " "}, {WALL, "*"}, {SAND, "S"}
};

bool operator< (struct pixel a, struct pixel b);

class LevelData {
    public:
      LevelData(std::string);
      FEATURE get(int, int);
    private:
      friend std::ostream& operator<<(std::ostream&, const LevelData&);
      FEATURE levelFeatures[HEIGHT][WIDTH];
};

#endif
