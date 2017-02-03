MSYS2_x86_64 설치

pacman -S wget vim swig mc pkg-config subversion python2 git curl tar binutils autoconf make libtool automake mingw-w64-i686-gcc mingw-w64-x86_64-gcc

set MSYS2_PATH_TYPE=inherit -->check msys2.bat  개발자 모드 환경 전송을 위한 모드

컴파일 개발 도구 컨솔 모드로 실행.

mv /bin/link.exe /bin/mingw_link.exe

 ./configure --toolchain=msvc --arch=x86 --yasmexe='C:/yasm/yasm.exe' --enable-gpl --enable-nonfree --cpu=i686 --enable-shared --enable-libx264

make
make install

mv /bin/mingw_link.exe /bin/link.exe

#ifdef FFMOEG_DLL_USE
#define CONFIG_SHARED 1
#endif