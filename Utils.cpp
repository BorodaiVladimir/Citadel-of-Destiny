#include "Utils.h"
#include <cstdlib>

namespace MyGame {

    int GetRandomNumber(int min, int max) {
        return min + rand() % (max - min + 1);
    }

} // namespace MyGame