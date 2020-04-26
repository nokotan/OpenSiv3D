# OpenSiv3D Web版

## ビルド要件

ビルドには CMake と emscripten が必要です。

依存しているライブラリは以下の通りです。

- OpenGLES 3.0 (`-s FULL_ES3`)
- libpng (`-s USE_LIBPNG=1`)
- FreeType2, HarfBuzz (`-s USE_FREETYPE=1, -s USE_HARFBUZZ=1`)
- OpenAL
- libogg, libvorbis (`-s USE_OGG=1, -s USE_VORBIS=1`)
- Boost (`-s USE_BOOST_HEADERS=1`)
- zlib (`-s USE_ZLIB=1`)

現在サポートしていないが、将来依存関係に追加される予定のライブラリは以下の通りです。

- OpenCV4

## OpenSiv3D のビルド

1. ソースコードのクローンとブランチ変更。
    1. `git clone https://github.com/Siv3D/OpenSiv3D.git`
    1. `cd OpenSiv3D`
1. ビルド用のディレクトリを作ります。ここでは `Web/Build` とします。
    1. `mkdir Web/Build`
    1. `cd Web/Build`
1. ビルドを行います。ビルドに成功すると `libSiv3D.a` が得られます。
    1. `emcmake cmake -DCMAKE_BUILD_TYPE=Release ..`
    1. `emmake make`

### 補足

- `-DCMAKE_BUILD_TYPE=Debug` とすると、`-g3 -O0` オプションでコンパイルされます。
- 上記の例では `make` を使っていますが、`ninja` でもコンパイルできます。
- コンパイラを指定する場合は `cmake` を実行する前に環境変数 `CC` と `CXX` を設定してください。

## アプリケーションのビルド

`Web/App` ディレクトリにアプリケーションに必要なファイルの例があります。
OpenSiv3Dと同様に `cmake` を使ってビルドできます。

- `Main.cpp`
  - アプリケーションのソースコードのサンプル。
- `CMakeLists.txt`
  - アプリケーションビルド用の `CMakeLists.txt` のサンプル。
- `resources/` ディレクトリ
  - アプリケーションの実行に必要なリソースを配置したディレクトリ。

### 注意点

- `App` ディレクトリを移動やコピーする場合は `CMakeLists.txt` 内に書かれている `libSiv3D.a`のパスとインクルードディレクトリのパスを適切なパスに書き換えてください。
- アプリケーションの実行時には `resources/` ディレクトリが実行ファイルと同階層のディレクトリにあるようにしてください。
