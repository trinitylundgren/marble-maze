
#include "level-loader.hpp"

LevelData::LevelData(std::string bmpPath) {
    int i;
    FILE* f = fopen(bmpPath.c_str(), "rb");
    uint8_t info[54];

    // read the 54-byte header
    fread(info, sizeof(uint8_t), 54, f);

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    // allocate 3 bytes per pixel
    int size = 3 * width * height;
    uint8_t* data = new uint8_t[size];

    // read the rest of the data at once
    fread(data, sizeof(uint8_t), size, f);
    fclose(f);

    for(i = 0; i < size; i += 3) {
            // flip the order of every 3 bytes
            uint8_t tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

    struct pixel* pixels = (struct pixel*)data;

    // Iterate over pixels; write into level features
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            struct pixel currentPixel = pixels[i * WIDTH + j];
            if (pixelsToFeatures.find(currentPixel) != pixelsToFeatures.end()) {
                FEATURE currentFeature = pixelsToFeatures[currentPixel];
                levelFeatures[HEIGHT - i - 1][WIDTH - j - 1] = currentFeature;
            }
            else {
                std::cerr << "Unrecognized pixel in bmp_path: \n";
                std::cerr << "i = " << i << ", j = " << j << "\n";
                std::cerr << "red = " << currentPixel.red << ", green = " << currentPixel.green;
                std::cerr << "blue = " << currentPixel.blue << "\n";
                exit(1);
            }
        }
    }

    delete [] data;
}

FEATURE LevelData::get(int i, int j) {
    return levelFeatures[i][j];
}

std::ostream& operator<<(std::ostream &strm, const LevelData &ld) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            FEATURE feature = ld.levelFeatures[i][j];
            if (featuresToChars.find(feature) != featuresToChars.end()) {
                strm << featuresToChars[feature];
            }
            else {
                strm << "?";
            }
        }
        strm << "\n";
    }
    return strm;
}

bool operator< (struct pixel a, struct pixel b) {
    return a.red * 0x10000 + a.green * 0x100 + a.blue <
           b.red * 0x10000 + a.green * 0x100 + a.blue;
}

/*int main() {
    // TODO: No segfault with bad filenames
    std::string filename = "level-data/level-0.bmp";
    LevelData levelData = LevelData(filename);
    std::cout << levelData;
}
*/
