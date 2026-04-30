# Deploy for Linux

Geminiと確認しながら、AppImageの作成方法としてこの資料を作成しました。

ディレクトリやツールの配置イメージは以下のようになります。

```
Deploy/
　├linuxdeploy-x86_64.AppImage
　├linuxdeploy-plugin-qt-x86_64.AppImage
　├DeployAnalogClock.sh
　└AnalogClock/
 　├AnalogClock
 　└AnalogClock.png
```

## 1. 作業用ディレクトリの作成と作成モジュールの配置

1. Deploy環境のベースとして、Deployディレクトリを作成します。
1. その配下に今回作成した AnalogClockのデプロイ用ディレクトリを作成します。
1. AnalogClockディレクトリに今回作成したモジュールとアイコン用のPNGファイルを配置します。


## 2. 必要なツールのインストール

Deployディレクトリに、AppImage作成に必要なツールを揃えます。実行はDeployディレクトリで行ってください。

- linuxdeploy: アプリの実行に必要なライブラリをかき集めるツール。
- linuxdeploy-plugin-qt: Qt固有の依存関係（プラグイン等）を解決するプラグイン。

```bash
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x *.AppImage
```

## 3. デプロイ実行

以下のコマンドを Deployディレクトリに Bashシェルとして作成します。
作成の際は、各位の環境にパス等を調整してください。

**qmakeの場所は要修正です。**

**現時点でWindowsの最新版は6.8.3で、Ubuntuの最新版は6.11.0になります。**

>今回は、DeployAnalogClock.shとします。

```bash
#!/bin/bash

# qmakeがあるディレクトリを一時的にPATHの先頭に追加
export PATH=/home/<your name>/Qt/6.x.y/gcc_64/bin:$PATH

# Qtの qmake へのパスをセット
export QMAKE=/home/<your name>/Qt/6.x.y/gcc_64/bin/qmake

# linuxdeploy を実行
../linuxdeploy-x86_64.AppImage \
    --executable AnalogClock \
    --icon-file AnalogClock.png \
    --create-desktop-file \
    --appdir AppDir \
    --plugin qt \
    --output appimage
```

AnalogClockディレクトリに移動して、１つ上位のフォルダにあるスクリプトを実行します。

## Tips: FUSE問題への対応方法

AppImageを作成して、Linuxで実行したときに起動に失敗する（なにも反応がない）ことがあります。コンソールを起動して、コンソールから、AnalogClockを実行した際に、"Fuseがぁ"という文字が確認できた場合、この「FUSE問題」が発生しています。

### libfuse2 のインストール:

新しいUbuntu環境だと、AppImage作成ツール自体を動かすために libfuse2（または libfuse2t64）が必要になることがあります。以下のコマンドを実行して古いFUSEをインストールすることで対応することができます。

```bash
sudo apt install libfuse2t64
```

## Tips: 起動時に libOpenGL.so.0 がないと言われる場合（Ubuntu 26.04等）

新しいUbuntu環境では、標準でOpenGL関連のライブラリがインストールされていない場合があります。実行時に `error while loading shared libraries: libOpenGL.so.0` と表示された場合は、以下のパッケージをインストールしてください。

```bash
sudo apt install libopengl0
```

## Tips: 特定の環境（Lubuntuなど）で背景が透過しない場合

本アプリは透過処理を使用していますが、Lubuntuなどの軽量なデスクトップ環境では、OS側の「コンポジター」が動作していないと背景が黒くなります。

対処法: > - picom 等のコンポジターがインストール・起動されているか確認してください。

OS側の設計により、透過処理をサポートしていない場合があります。その際はOSの仕様となります。


# Deploy for Windows

WindowsへのDeployは、Qtパッケージに同梱されている`windeployqt.exe`を実行することで完了します。Linux同様にDeploy用フォルダを作成してから作業を行います。

ディレクトリやバイナリの配置イメージは以下のようになります。

```
Deploy/
　└AnalogClock/
 　└AnalogClock.exe
```

AnalogClockフォルダに移動して、以下のコマンドを実行します。なお、実行の際は、WindowsのメニューからQtの「Qt 6.x.y (MSVC 2022 64-bit)」を選択してコンソールを起動した後、以下のコマンドをコンソールで実行してください。

```cmd
windeployqt -release AnalogClock.exe
```

AnalogClockフォルダをZIP圧縮して完了です。

## Tips: vcruntime140.dll が見つからない・・・と出る場合

AnalogClock.exeを実行する環境が、Windows11のインストール直後の場合、この`vcruntime140.dllがない`というダイアログが表示されます。
>私の場合は、`「vcruntime140_1.dllがない」`でした。

このダイアログが表示される場合は、以下の場所から「Microsoft Visual C++ 再頒布可能パッケージ（64ビット版）」をインストールする必要があります。

### インストール手順（概要）

- 公式サイトからダウンロード<br>
  Microsoft の公式リンク（例: [https://aka.ms/vc14/vc_redist.x64.exe](https://aka.ms/vc14/vc_redist.x64.exe)）から取得します。
- インストーラーを実行<br>
  ダウンロードした vcredist_x64.exe をダブルクリックして起動します。
- 使用許諾に同意<br>
  チェックボックスをオンにして「インストール」をクリック。
- 管理者権限の許可<br>
  UAC（ユーザーアカウント制御）が表示されたら「はい」を選択。
- インストール完了後に再起動<br>
  再起動することで、依存アプリがランタイムを認識します。
