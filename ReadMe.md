# AnalogClock

A sleek, 24-hour transparent analog clock built with **Qt6** and customized for professional workflows. This project is a modern evolution of the classic Qt "analogclock" example, developed in collaboration with Gemini.

## Key Features

* **Unique 24-Hour Main Dial**: Designed with "00" at the bottom (traditional 6 o'clock position) for a distinct, instrument-like feel.
* **High-Precision Stopwatch**: Features a "smooth-glide" seconds hand with a 40ms update interval for seamless, sweeping movement.
* **Dual-Time Subdial**: A 12-hour subdial ensures you never lose track of real-time, even when the stopwatch mode is active.
* **Desktop-Friendly UI**: Frameless, transparent, and configured to "always on top" — perfectly optimized to live elegantly on your Linux desktop.
* **Dynamic Resizing**: Easily scale the clock's size up or down simply by scrolling the mouse wheel.
* **Component-Level Visibility Toggles**: Customize your view by toggling the rendering of the outer border, markers, numbers, subdial, and more.
* **Color Inversion**: Easily invert the UI drawing colors between white and black to maintain high contrast and visibility against any desktop background.

## Usage

* **Move**: Left-click and drag anywhere on the clock to reposition it on your screen.
* **Resize**: Scroll the mouse wheel forward or backward to scale the UI.
* **Stopwatch**: Right-click to access the context menu for stopwatch controls (Start / Stop / Reset).
* **Toggle Components**: Right-click to access customization options (Toggle Minute Ticks / Hour Markers / Subdial, etc.).

## Functional Description

### Subdial #1

The subdial located in the upper right functions as a standard 12-hour clock. When the stopwatch is active, the subdial's hands continue tracking the actual current time independently, ensuring you stay anchored to real-time.

### Main Dial

The main dial serves a dual purpose as both a 24-hour clock and a high-precision stopwatch. The accent-colored (Cyan/Red) seconds hand sweeps smoothly at a 40ms granularity, delivering the premium aesthetic and tactile feel of a luxury mechanical timepiece.

## Technical Details

* **Framework**: Qt 6.11 (Linux), Qt 6.11 (Windows)
* **Graphics**: Anti-aliased rendering utilizing `QPainter` coupled with `WA_TranslucentBackground`.
* **Timing**: Precision tracking powered by `QElapsedTimer` to drive the stopwatch logic.

## ScreenShots v0.3.0

| Clock Mode (All Components On) | Stopwatch Mode | Color Inversion |
| :---------------------------: | :------------: | :-------------: |
| ![ScreenShot01](doc/ScreenShot_D01.png) | ![ScreenShot02](doc/ScreenShot_D02.png) | ![ScreenShot12](doc/ScreenShot_D12.png) |

| Hour Hand Only | No Hour Markers | No Minute Ticks |
| :------------: | :------------: | :-------------: |
| ![ScreenShot03](doc/ScreenShot_D03.png) | ![ScreenShot04](doc/ScreenShot_D04.png) | ![ScreenShot05](doc/ScreenShot_D05.png) |

| No Subdial | No Digital Panel | No Date Display |
| :--------: | :--------------: | :-------------: |
| ![ScreenShot06](doc/ScreenShot_D06.png) | ![ScreenShot07](doc/ScreenShot_D07.png) | ![ScreenShot08](doc/ScreenShot_D08.png) |

| No Numbers | No Outer Border | Minimalist (Hour Hand Only!) |
| :--------: | :------------: | :--------------------------: |
| ![ScreenShot09](doc/ScreenShot_D09.png) | ![ScreenShot10](doc/ScreenShot_D10.png) | ![ScreenShot11](doc/ScreenShot_D11.png) |