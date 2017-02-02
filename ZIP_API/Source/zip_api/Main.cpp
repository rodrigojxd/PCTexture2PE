#include <experimental\filesystem>
#include <fstream>
#include "zip_api.h"
#include "../ZipLib/ZipFile.h"

namespace fs = std::experimental::filesystem;

void zip_api::extractAll(const std::string& zipArchiveName, const std::string& folder)
{
	const std::string folder_ = folder + '/';
	ZipArchive::Ptr archive = ZipFile::Open(zipArchiveName);
	size_t entries = archive->GetEntriesCount(); //number of archives + folders
	std::error_code err;
	fs::create_directory(folder_, err);
	for (size_t i = 0; i < entries; ++i)
	{
		auto entry = archive->GetEntry(int(i));
		std::string path = entry->GetFullName();
		if (entry->GetName() != "") //if it's not a folder
		{
			size_t pos = path.find_last_of('/');
			if (pos != std::string::npos) //if found a '/'
			{
				std::string dir = path;
				dir.erase(pos); //remove the file name (get the folder)
				fs::create_directory(folder_ + dir, err);
			}
			ZipFile::ExtractFile(zipArchiveName, path, folder_ + path);
		}
		else //if it's a folder then create it
		{
			fs::create_directory(folder_ + path, err);
		}
	}
}

void zip_api::compressAll(const std::string& folder, const std::string& zipFilename)
{
	ZipArchive::Ptr archive = ZipFile::Open(zipFilename);
	DeflateMethod::Ptr ctx = DeflateMethod::Create();
	ctx->SetCompressionLevel(DeflateMethod::CompressionLevel::L1);

	for(auto& f : fs::recursive_directory_iterator(folder)) //looks for all files in the folder one by one
	{
		if (fs::is_regular_file(f)) //if it's a file
		{
			std::string filename = fs::path(f).string().substr(folder.size() + 1); //get the 'internal' zip path
			ZipArchiveEntry::Ptr entry = archive->CreateEntry(filename);
			entry->UseDataDescriptor();
			std::ifstream contentStream;
			contentStream.open(fs::path(f).string(), std::ios::binary);
			entry->SetCompressionStream(   // compress the data
				contentStream,
				ctx,
				ZipArchiveEntry::CompressionMode::Immediate
			);
		}
	}
	ZipFile::SaveAndClose(archive, zipFilename);
}
