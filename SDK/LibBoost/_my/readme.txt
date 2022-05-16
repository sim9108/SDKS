bootstrap.bat

b2 --toolset=msvc-14.3  --build-type=complete

//b2 --toolset=msvc-14.3  --build-type=complete --with-iostreams -sZLIB_SOURCE="..\..\LibZ\doc\zlib" -sZLIB_INCLUDE="..\..\LibZ\doc\zlib"

#define BOOST_ASIO_DISABLE_CONCEPTS