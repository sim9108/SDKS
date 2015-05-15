MSYS2 ¼³Ä¡

pacman -S wget vim swig mc pkg-config subversion python2 git curl tar binutils autoconf make libtool automake mingw-w64-i686-gcc mingw-w64-x86_64-gcc

mv /bin/link.exe /bin/mingw_link.exe

 ./configure --toolchain=msvc --arch=x86 --yasmexe='C:/yasm/yasm.exe' --enable-gpl --enable-nonfree --cpu=i686 --enable-shared
make
make install

mv /bin/mingw_link.exe /bin/link.exe