폴더이름변경
_LibJPEG
_LibJXR
_LibOpenJPEG
_LibPNG
_LibRawLite
_LibTIFF4
_LibWebP
_OpenEXR
_ZLib


src\Source\MapIntrospector.h

template<class _Maptype>
class MapIntrospector: private _Maptype {
public:
	static size_t GetNodesMemorySize(size_t node_count) {
		return (node_count + 1) * sizeof(_Maptype::_Node);
	}
};

Source\Utilities.h
Source\Conversion.cpp
BOOL -> bool 타입으로



Source\FreeImageToolkit\JPEGTransform.cpp
#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"
#include "transupp.h"


Source\FreeImage\PluginJ2K.cpp
Source\FreeImage\PluginJP2.cpp
Source\FreeImage\J2KHelper.cpp
#include "openjpeg.h"


Source\FreeImage\PluginEXR.cpp
#include "ImfIO.h"
#include "Iex.h"
#include "ImfOutputFile.h"
#include "ImfInputFile.h"
#include "ImfRgbaFile.h"
#include "ImfChannelList.h"
#include "ImfRgba.h"
#include "ImfArray.h"
#include "ImfPreviewImage.h"
#include "half.h"
IMath::Int64 => uint64_t로 변경

Source\Metadata\XTIFF.cpp
Source\FreeImage\PluginG3.cpp
#include "tiffiop.h" //맨상단.

Source\FreeImage\PluginTIFF.cpp
#include "tiffiop.h"  

#include "FreeImage.h"
#include "Utilities.h"
#include "../Metadata/FreeImageTag.h"
#include "half.h"

#include "FreeImageIO.h"
#include "PSDParser.h"

Source\FreeImage\PluginJPEG.cpp
#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"
jpeg_read_icc_profile 제거


Source\FreeImage\PluginJXR.cpp
#include "JXRGlue.h"

Source\FreeImage\PluginPNG.cpp
#include "zlib.h"
#include "png.h"


Source\FreeImage\PluginWebP.cpp
#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/mux.h"

Source\FreeImage\ZLibInterface.cpp
#include "zlib.h"
#include "zutil.h"
