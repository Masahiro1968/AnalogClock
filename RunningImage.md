# Running Images

These operation checks were performed by executing the **v0.2.0** binary under a `Hyper-V` environment.

Testing was conducted on freshly installed OS environments. Depending on the OS, there may be some stuttering during window movement. The movement of the clock hands appears to be fine, and the stopwatch functionality worked without issues.

The light/dark mode reversal feature also functioned correctly.

> Lubuntu is the only exception where background transparency does not work. Since the direction of Lubuntu's high-speed rendering and the clock's background transparency seem incompatible, I have decided to forgo support for it there.

- Hyper-V Configuration:
  - 4 Cores
  - 4GB Memory
  - 60GB Disk (virtual)

## Debian 13.4.0

Confirmed working on Debian 13.

![Debian13.4.0](doc/ScreenShot_Debian-13.4.0.png)

## Kubuntu 24.04.4

Confirmed working on Kubuntu 24.04.4.

In Kubuntu, it is stylish that the execution image icon is replaced by the bundled image.

![kubuntu 24.04.4](doc/ScreenShot_Kubuntu-24.04.4.png)

## Kubuntu 26.04

Confirmed working on Kubuntu 26.04. The execution icon looks stylish here as well.

![Kubuntu26.04](doc/ScreenShot_Kubuntu-26.04.png)

## Lubuntu 24.04.4

Confirmed working on Lubuntu 24.04.4.

However, the background did not become transparent. This follows the policy of a lightweight OS.

![Lubuntu-24.04.4](doc/ScreenShot_Lubuntu-24.04.4.png)

## Lubuntu 26.04

Confirmed working on Lubuntu 26.04.

As expected, the background transparency did not work.

![Lubuntu26.04](doc/ScreenShot_Lubuntu-26.04.png)

## Ubuntu 24.04.3

Confirmed working on Ubuntu 24.04.3.

I expected FUSE issues, but it worked smoothly right after installation. There might be a possibility of it failing as more applications are installed.

![Ubuntu-24.04.3](doc/ScreenShot_Ubuntu-24.04.3.png)

## Ubuntu 26.04

Confirmed working on Ubuntu 26.04.

> As seen in the capture image, it does not run immediately because `libOpenGL.so` is missing. It works once this library is added.

![Ubuntu-26.04](doc/ScreenShot_Ubuntu-26.04.png)

## Windows 11

Confirmed working on Windows 11.

The source code itself is almost entirely shared with the Linux version (thanks to Qt).

![Windows11](doc/ScreenShot_Windows11.png)
