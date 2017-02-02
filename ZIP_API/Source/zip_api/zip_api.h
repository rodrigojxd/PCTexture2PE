#pragma once
#ifdef ZIP_EXPORTS
#define ZIP_API __declspec(dllexport)
#else
#define ZIP_API __declspec(dllimport)
#endif // ziplib

namespace zip_api
{
	ZIP_API void extractAll(const std::string& zipArchiveName, const std::string& folder);
	ZIP_API void compressAll(const std::string& folder, const std::string& zipFilename);
}