$PATH에 NAMS 경로등록
Perl 최신 버전 설치

x64 Native Tools Command Prompt for VS 2022 링크 cmd.lnk를 수행한 후
다음을 수행한다.

cd ..
mkdir lib64
cd src

// release
perl Configure VC-WIN64A no-shared
nmake
copy /Y "./libcrypto.lib" "../lib64/libcrypto.lib"
copy /Y "./libssl.lib" "../lib64/libssl.lib"

//debug
perl Configure VC-WIN64A no-shared -d
nmake
copy /Y "./libcrypto.lib" "../lib64/libcrypto.lib"
copy /Y "./libssl.lib" "../lib64/libssl.lib"