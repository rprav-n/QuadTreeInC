rm -rf ./build/main.exe
gcc -o build/main src/*.c -Wall -Werror -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -g -O1 C:/raylib/raylib/src/raylib.rc.data -I. -IC:/raylib/raylib/src -IC:/raylib/raylib/src/external -L. -LC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP
./build/main.exe