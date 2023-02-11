# ブロック崩し 
## 概要
C言語と[ncurses](https://invisible-island.net/ncurses/)で作ったシンプルなブロック崩しです
## ビルドと実行
### Linuxの場合
```bash
$ make all
$ ./block
```
### Windowsの場合
1. [MSYS2](https://www.msys2.org/)のCLANG64環境上でビルドします  
   必要なパッケージ:
   - [mingw-w64-clang-x86_64-toolchain](https://packages.msys2.org/groups/mingw-w64-clang-x86_64-toolchain)
   - [mingw-w64-clang-x86_64-ncurses](https://packages.msys2.org/package/mingw-w64-clang-x86_64-ncurses?repo=clang64)
   ```bash
   $ CFLAGS="-DWINDOWS" mingw32-make all
   ```
2. Windowsの環境変数の設定でPATHにC:\msys64\clang64\bin\を追加します  
3. コマンドプロンプトやダブルクリックでblock.exeを実行するとゲームが始まります
## 既知のバグ
- Windows上では速度が想定より遅くなる  
  原因を調査しましたが､どうもwinpthreadのnanosleep関数の実装にバグがあるようで､指定した時間よりも長くsleepしてしまうようです  