@echo Delete MD3Viewer.exe if exists
@if exist MD3Viewer.exe del MD3Viewer.exe

@echo Compile MD3Viewer.exe
@gcc src/*.c -o MD3Viewer.exe -O3 -Wall -lmingw32 -Llibs -lSDL2main -lSDL2 -lSDL2_image -lOpenGL32 -lglu32 -lOpenGLUT

@echo Compilled...
@pause