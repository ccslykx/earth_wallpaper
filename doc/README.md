# 🌏earth_wallpaper

### [中文](https://github.com/ambition-echo/earth_wallpaper#readme)

Get real-time earth photos as wallpapers

Not only real-time earth

[![Build](https://github.com/ambition-echo/earth_wallpaper/actions/workflows/build.yml/badge.svg)](https://github.com/ambition-echo/earth_wallpaper/actions/workflows/build.yml)
[![Aur](https://github.com/ambition-echo/earth_wallpaper/actions/workflows/aur.yml/badge.svg)](https://github.com/ambition-echo/earth_wallpaper/actions/workflows/aur.yml)
[![pipeline](https://jihulab.com/ambition-echo/earth_wallpaper/badges/main/pipeline.svg)](https://jihulab.com/ambition-echo/earth_wallpaper/commits/main)

[![downloads](https://img.shields.io/github/downloads/ambition-echo/earth_wallpaper/total)](https://github.com/ambition-echo/earth_wallpaper/releases)
[![Release](https://img.shields.io/github/v/release/ambition-echo/earth_wallpaper)](https://github.com/ambition-echo/earth_wallpaper/releases)
[![License](https://img.shields.io/github/license/ambition-echo/earth_wallpaper)](https://github.com/ambition-echo/earth_wallpaper/blob/main/LICENSE)

## Quick Start

### Deepin

Go to the [release](https://github.com/ambition-echo/earth_wallpaper/releases) page to download the installation
package ```earth-wallpaper-deepin-amd64.deb```

### Debian/Ubuntu

Go to the [release](https://github.com/ambition-echo/earth_wallpaper/releases)page to download the installation
package ```earth-wallpaper-other-amd64.deb```

### Arch

[![AUR version](https://img.shields.io/aur/version/earth-wallpaper-bin)](https://aur.archlinux.org/packages/earth-wallpaper-bin)
[![AUR version](https://img.shields.io/aur/version/earth-wallpaper-nightly)](https://aur.archlinux.org/packages/earth-wallpaper-nightly)

Download PKGBUILD from [AUR](https://aur.archlinux.org/packages/earth-wallpaper-bin)

### Usage Notice

When you run it for the first time, the settings window will pop up, click ```Apply``` to start running

## Support Interface

- [x] Himawari-8 (Weather satellites from Japan)
- [x] FY-4A (Weather satellites from China)
- [x] Bing Wallpaper (Call [@xCss](https://github.com/xCss/bing) API)
- [x] Anime Wallpaper (Call [waifu.im](https://waifu.im/) API)
- [x] Local Wallpaper
- [x] 24h Wallpaper (Inspired by [windynamicdesktop](https://github.com/t1m0thyj/windynamicdesktop))

> 24h Wallpaper recommended download address:
>
> [https://github.com/MiniBusiest/24Hour-Wallppe](https://github.com/MiniBusiest/24Hour-Wallppe)
>
> [https://windd.info/themes/index.html](https://windd.info/themes/index.html)

## Supported Desktop Environment

### Linux

- [x] KDE Plasma
- [x] Deepin
- [x] GNOME
- [x] ubuntu:GNOME
- [x] Cinnamon
- [x] XFCE
- [x] MATE
- [x] Cutefish
- [x] LXQt (pcmanfm-qt 或者 pcmanfm)
- [x] LXDE (pcmanfm)

### Windows

- [x] Windows 10

## Depends

- Qt5
- Python3
- qdbus
- python3-pil.imagetk
- python3-requests

## Compile Manually

- Clone the repository

```shell
git clone https://jihulab.com/ambition-echo/earth_wallpaper.git
cd earth_wallpaper
mkdir build && cd build
```

- Compile and Build

```shell
cmake ..
make
```

- Package

```shell
cd ../package
chmod +x ./package.sh
./package.sh
```

## Public API

bing: [https://github.com/xCss/bing](https://github.com/xCss/bing)

waifu.im: [https://waifu.im/](https://waifu.im/)

ipapi: [https://ipapi.co](https://ipapi.co)

## ScreenShot

![image-20220917003305855](https://jihulab.com/ambition-echo/img_bed/-/raw/main/img/image-20220917003305855.png)

![image-20220917003345620](https://jihulab.com/ambition-echo/img_bed/-/raw/main/img/image-20220917003345620.png)

![image-20220917003459088](https://jihulab.com/ambition-echo/img_bed/-/raw/main/img/image-20220917003459088.png)

![image-20220917003531050](https://jihulab.com/ambition-echo/img_bed/-/raw/main/img/image-20220917003531050.png)