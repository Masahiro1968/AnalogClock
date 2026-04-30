# DEPLOY FOR LINUX

This document outlines the procedure for creating an AppImage, developed in collaboration with Gemini.

**Directory and Tool Layout:**

```
Deploy/
  - linuxdeploy-x86_64.AppImage
  - linuxdeploy-plugin-qt-x86_64.AppImage
  - DeployAnalogClock.sh
  - AnalogClock/ (Sub-directory)
      - AnalogClock (Binary)
      - AnalogClock.png (Icon)
```

## 1. Creating the Working Directory and Placing Modules

1. Create a "Deploy" directory as the base for the deployment environment.
2. Create an "AnalogClock" deployment directory within it.
3. Place the compiled module and the icon PNG file into the "AnalogClock" directory.

## 2. Installing Required Tools

Prepare the tools in the "Deploy" directory. Execute the following commands:

- linuxdeploy: A tool to bundle required libraries.
- linuxdeploy-plugin-qt: A plugin to resolve Qt dependencies.

```bash
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x *.AppImage
```

## 3. Executing Deployment

Create a script named "DeployAnalogClock.sh" in the Deploy directory.

Note: You must update the qmake path. Currently, Windows uses 6.8.3 while Ubuntu uses 6.11.0.

```bash
#!/bin/bash

export PATH=/home/<your name>/Qt/6.x.y/gcc_64/bin:$PATH
export QMAKE=/home/<your name>/Qt/6.x.y/gcc_64/bin/qmake

../linuxdeploy-x86_64.AppImage \
    --executable AnalogClock \
    --icon-file AnalogClock.png \
    --create-desktop-file \
    --appdir AppDir \
    --plugin qt \
    --output appimage
```

Navigate to the AnalogClock directory and execute the script from the parent folder.

## Tips: Addressing FUSE Issues

If the AppImage fails to start with a "FUSE" error in the console:
Newer Ubuntu environments may require libfuse2.

```bash
sudo apt install libfuse2t64
```

## Tips: "libOpenGL.so.0 not found" Error (e.g., Ubuntu 26.04)

On newer Ubuntu environments, OpenGL-related libraries may not be pre-installed. If you encounter the error `error while loading shared libraries: libOpenGL.so.0`, please install the following package:

```bash
sudo apt install libopengl0
```

## Tips: Background Transparency Issues (e.g., Lubuntu)

If the background is black in lightweight environments like Lubuntu:
Check if a compositor like "picom" is running. Some environments may not support transparency due to OS design.


# DEPLOY FOR WINDOWS

Windows deployment uses "windeployqt.exe" included in the Qt package.

**Directory Layout:**

```
Deploy/
  - AnalogClock/
      - AnalogClock.exe
```

## Execution Steps:

1. Navigate to the AnalogClock folder.
2. Open the Qt console (e.g., "Qt 6.x.y MSVC 2022 64-bit") from the Start menu.
3. Run the following command:
   windeployqt -release AnalogClock.exe
4. Compress the folder into a ZIP file.

## Tips: "vcruntime140.dll not found" Error

If this error appears on fresh Windows 11 installations:
Install the "Microsoft Visual C++ Redistributable (64-bit)".
Link: https://aka.ms/vc14/vc_redist.x64.exe
After installation, restart your PC.
