# MD3 Model & Animation Viewer
This program can open **[.md3](https://en.wikipedia.org/wiki/MD3_(file_format))** model *(from [Quake 3 Arena](https://en.wikipedia.org/wiki/Quake_III_Arena))* files with **.cfg** animations.
It using **SDL2** and **OpenGL** for rendering and drawing.

This is my big homework for my university.

## Usage
* I. **Drop 'n View**
  * Drop the .md3 file into the MD3Viewer.exe
  * It can load only model, not animation!
* II. **Console**
  * Start from *console* or *.bat* file
  * Usage: ./MD3Viewer.exe [OPTIONS] -f model.md3
  * Options: 
    * -f file.md3   | Set model's file
    * -a file.cfg    | Set model's animation file
    * -W width      | Set screen's width
    * -H <height     | Set screen's height
    * -F              | Fullscreen
    * -s scale      | Set model's scale
    * -fps fps      | Set FPS (animation may change it)
    * -r              | Redirect standard outputs, errors to file
    * -d              | Enable debug mode
    * -i              | Enable info mode
    * -h              | Print usage / this
