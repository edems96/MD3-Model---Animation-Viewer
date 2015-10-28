del MD3Viewer.exe
gcc src/*.c -o MD3Viewer.exe -Wall -lmingw32 -L. -lSDL2main -lSDL2 -lSDL2_image -lOpenGL32 -lglu32 -lOpenGLUT
pause