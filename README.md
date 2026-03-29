# 5-Minute Vikram and Betal Animation Engine

This project uses OpenGL and FreeGLUT in C++ to generate a 5-minute continuous, visual animation of the classic folklore: King Vikramaditya catching ghost Betal, and Betal telling him a story. 

The application is purely visual (no subtitles or text overlays), which stretches runtime pacing accurately across 300 seconds and perfectly loops on completion.

## System Prerequisites
- Windows OS
- MSYS2 environment configured with GCC (specifically `C:\msys64\ucrt64\bin\g++.exe`)
- OpenGL Development libraries: `libfreeglut`, `libglu32`, `libopengl32`

## How To Execute

### Method 1: Visual Studio Code (`tasks.json`)
The workspace includes a `.vscode/tasks.json` configuration file, making it easy to compile locally.
1. Open this `opengl` directory within Visual Studio Code.
2. Press **`Ctrl + Shift + B`**. This will execute the default `Build OpenGL Project` task. 
3. If there are no issues, a new executable called `vikram_betal.exe` will be generated in your project folder.
4. Open the integrated terminal (`Ctrl + ~`) and type:
   ```powershell
   .\vikram_betal.exe
   ```

### Method 2: Manual Terminal Compilation
If you prefer not to rely on the VS Code shortcut, you can execute the raw GCC compile command manually from your PowerShell or Command Prompt running inside this folder:

**1. Compile the code:**
```powershell
C:\msys64\ucrt64\bin\g++.exe -g vikram_betal.cpp -o vikram_betal.exe -lfreeglut -lglu32 -lopengl32
```
**2. Run the code:**
```powershell
.\vikram_betal.exe
```

## How To Record the 5-Minute Video
Since this directly launches an OpenGL graphical window:
1. Launch the `vikram_betal.exe` application.
2. Press the **`Win(❖) + G`** keys to open the built-in Windows Xbox Game Bar, or launch any standard screen recorder like **OBS Studio**.
3. Hit the Record button securely over the window region. Wait until the 5 minutes have completely elapsed, and then hit Stop.
