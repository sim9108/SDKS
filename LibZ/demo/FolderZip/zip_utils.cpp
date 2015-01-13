#include "zip_utils.h"
zip_fileinfo& operator<< (zip_fileinfo& info, std::tr2::sys::path& mpath){
	memset(&info, 0, sizeof(zip_fileinfo));
	std::time_t tm = std::tr2::sys::last_write_time(mpath);
	struct tm* tdata = std::localtime(&tm);
	info.tmz_date.tm_year = tdata->tm_year;
	info.tmz_date.tm_mon = tdata->tm_mon;
	info.tmz_date.tm_mday = tdata->tm_mday;
	info.tmz_date.tm_hour = tdata->tm_hour;
	info.tmz_date.tm_min = tdata->tm_min;
	info.tmz_date.tm_sec = tdata->tm_sec;

	return info;
}