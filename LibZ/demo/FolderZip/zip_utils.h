#pragma once

#include <zip.h>
#include <filesystem>
zip_fileinfo& operator<< (zip_fileinfo& info, std::tr2::sys::path& mpath);
