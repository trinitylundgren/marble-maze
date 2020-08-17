/*!
 * @file demo2-hourglass.cpp
 *
 * Slightly more complex example for Adafruit_PixelDust on Raspberry Pi.
 * Has obstacles for particles to move around.
 * REQUIRES rpi-rgb-led-matrix LIBRARY!
 * I2C MUST BE ENABLED using raspi-config!
 *
 */

#ifndef ARDUINO // Arduino IDE sometimes aggressively builds subfolders

#include "Adafruit_PixelDust.h"
#include "led-matrix-c.h"
#include "lis3dh.h"
#include "level-loader.hpp"
#include <signal.h>

#define N_GRAINS 1 ///< Number of sand grains on 64x64 matrix

#define LEVEL_PATH "level-data/level-3.bmp"

struct RGBLedMatrix *matrix;
Adafruit_LIS3DH lis3dh;
volatile bool running = true;
int nGrains = N_GRAINS; // Runtime grain count (adapts to res)


// Signal handler allows matrix to be properly deinitialized.
int sig[] = {SIGHUP,  SIGINT, SIGQUIT, SIGABRT,
             SIGKILL, SIGBUS, SIGSEGV, SIGTERM};
#define N_SIGNALS (int)(sizeof sig / sizeof sig[0])

void irqHandler(int dummy) {
  if (matrix) {
    led_matrix_delete(matrix);
    matrix = NULL;
  }
  for (int i = 0; i < N_SIGNALS; i++)
    signal(sig[i], NULL);
  running = false;
}

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  struct LedCanvas *canvas;
  int width, height, i, xx, yy, zz;
  Adafruit_PixelDust *sand = NULL;
  dimension_t x, y;

  // Make LevelData object from bitmap filepath
  LevelData levelData = LevelData(LEVEL_PATH);

  for (i = 0; i < N_SIGNALS; i++)
    signal(sig[i], irqHandler); // ASAP!

  // Initialize LED matrix defaults
  memset(&options, 0, sizeof(options));
  options.rows = 64;
  options.cols = 64;
  options.chain_length = 1;

  // Parse command line input.  --led-help lists options!
  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  // Create offscreen canvas for double-buffered animation
  canvas = led_matrix_create_offscreen_canvas(matrix);
  led_canvas_get_size(canvas, &width, &height);
  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n", width, height,
          options.hardware_mapping);

  if (lis3dh.begin()) {
    puts("LIS3DH init failed");
    return 2;
  }

  sand = new Adafruit_PixelDust(width, height, nGrains, 1, 64, true);
  if (!sand->begin()) {
    puts("PixelDust init failed");
    return 3;
  }


  // Loop over every pixel in levelData, insert obstacles
  for (int i = 0; i < 64; ++i) {
      for (int j = 0; j < 64; ++j) {
         FEATURE feature = levelData.get(i, j);
         x = (dimension_t)j;
         y = (dimension_t)i;
         switch(feature) {
             case WALL:
                 sand->setPixel(x, y);
                 break;
             case SAND:
                 sand->setPosition(0, x, y);
                 break;
             case EMPTY:
                 break;
         }
      }
  }

  while (running) {
    // Read accelerometer...
    lis3dh.accelRead(&xx, &yy, &zz);

    // Run one frame of the simulation.  Axis flip here
    // depends how the accelerometer is mounted relative
    // to the LED matrix.
    sand->iterate(-xx, -yy, zz);

    // Canvas is cleared and both the hourglass and sand
    // grains are re-drawn every frame.  It's easier than
    // trying to erase-and-redraw when animation is
    // double-buffered.
    led_canvas_clear(canvas);
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
           FEATURE feature = levelData.get(i, j);
           x = (dimension_t)j;
           y = (dimension_t)i;
           if (feature == WALL) {
               led_canvas_set_pixel(canvas, x, y, 32, 32, 96);
           }
        }
    }

    sand->getPosition(0, &x, &y);
    led_canvas_set_pixel(canvas, x, y, 200, 200, 100);

    // Update matrix contents on next vertical sync
    // and provide a new canvas for the next frame.
    canvas = led_matrix_swap_on_vsync(matrix, canvas);
  }

  return 0;
}

#endif // !ARDUINO
