#pragma once
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ippicvmt.lib")
#pragma comment(lib, "Vfw32.lib")

#if defined(_DEBUG)
#if defined(_DLL)
#define LIBTYPE "Sd.lib"
#else
#define LIBTYPE "SMTd.lib"
#endif
#else
#if defined(_DLL)
#define LIBTYPE "S.lib"
#else
#define LIBTYPE "SMT.lib"
#endif
#endif

#pragma comment(lib, "opencv_calib3d" LIBTYPE)
#pragma comment(lib, "opencv_core" LIBTYPE)
#pragma comment(lib, "opencv_features2d" LIBTYPE)
#pragma comment(lib, "opencv_flann" LIBTYPE)
#pragma comment(lib, "opencv_core" LIBTYPE)
#pragma comment(lib, "opencv_hal" LIBTYPE)
#pragma comment(lib, "opencv_highgui" LIBTYPE)
#pragma comment(lib, "opencv_imgcodecs" LIBTYPE)
#pragma comment(lib, "opencv_imgcodecs" LIBTYPE)
#pragma comment(lib, "opencv_imgcodecs" LIBTYPE)
#pragma comment(lib, "opencv_imgproc" LIBTYPE)
#pragma comment(lib, "opencv_ml" LIBTYPE)
#pragma comment(lib, "opencv_objdetect" LIBTYPE)
#pragma comment(lib, "opencv_photo" LIBTYPE)
#pragma comment(lib, "opencv_shape" LIBTYPE)
#pragma comment(lib, "opencv_stitching" LIBTYPE)
#pragma comment(lib, "opencv_superres" LIBTYPE)
#pragma comment(lib, "opencv_ts" LIBTYPE)
#pragma comment(lib, "opencv_video" LIBTYPE)
#pragma comment(lib, "opencv_videoio" LIBTYPE)
#pragma comment(lib, "opencv_videostab" LIBTYPE)
#pragma comment(lib, "LibZ" LIBTYPE)
#pragma comment(lib, "LibTBB" LIBTYPE)
#pragma comment(lib, "LibJpegTurbo" LIBTYPE)
#pragma comment(lib, "LibTiff" LIBTYPE)
#pragma comment(lib, "LibPng" LIBTYPE)
#pragma comment(lib, "LibJasper" LIBTYPE)
#pragma comment(lib, "LibHalf" LIBTYPE)
#pragma comment(lib, "LibIlmImf" LIBTYPE)
#pragma comment(lib, "LibIlmThread" LIBTYPE)
#pragma comment(lib, "LibLex" LIBTYPE)
#pragma comment(lib, "LibMath" LIBTYPE)