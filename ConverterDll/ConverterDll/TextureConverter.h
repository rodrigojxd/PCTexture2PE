#pragma once
#include <string>
#include <vector>

#ifdef Converter_EXPORTS
#define Converter_API __declspec(dllexport)

#include <Rpc.h> //for UUID (need to include RpcRT4.Lib from windows kits)

//image editing
#include "opencv2/imgproc/imgproc.hpp"
#include "Color.h"

using namespace std;

#else
#define Converter_API __declspec(dllimport)
#endif

namespace ConverterFuncs {
#ifdef Converter_EXPORTS
	void delUseless(const string& path);
	void fsCopy(const string& old_, const string& new_);
	void fsCopyDir(const string& old_, const string& new_);
	void fsRename(const string& old_, const string& new_);
	void fsRemove(const string& path);
	void fsRemoveAll(const string& path);
	void fsMoveFile(const string& old_, const string& new_);
	void fsMoveDir(const string& old_, const string& new_);
	void fsCreate_directory(const string& path);
	
	const std::string newUUID();
	
	cv::Mat PushOnSide(const cv::Mat& img, const cv::Mat& img2);
	cv::Mat blend_multiply(const cv::Mat& level1, const int& opacity, const Color& cor);
	void EqualizeSizes(cv::Mat& img, cv::Mat& img2);
	void CopyTo(const cv::Mat4b& src, cv::Mat4b& dst, const int& alphaFilter);
	
	void CutHalf(const string& old, const string& new_);
	void CreateStatic(const string& old, const string& new_);
	void CreateItemAtlas(const string& path, const string& staticName, const string& atlasName);
	void OverlapImg(const string& overlay, const string& base, const string& newPath);
	void OverlapImg(const string& overlay, const string& base);
	void OverlapImgWithColor(const string& path, const string& pathNew, const Color& color);
	void CreateOpaque(const string& path, const string& pathNew, const int& grayLevel);
	void CreateTgaWithOverlay(const string& path, const string& overlay, const int& method, const string& newPath);
	void CreatePistonARM(const string& BlocksPath, const string& PistonARMPath);
	void OverlapImgAndColor(const string& basePath, const string& OverlayPath, const Color& color, const string& result);
	void CreateSheepTga(const string& path, const string& FurPath, const string& newPath);
	void FlipImage90(const string& path, const string& newPath);
	void CreateSpawnEgg(const string& basePath, const string& OverlayPath,
		const Color& color, const Color& color2, const string& result);
	void OverlapEnderman(const string& base, const string& overlay);
	void CreateCauldronWater(const string& path, const string& newPath);
	void CheckForAnimated(const string& path);
	void CropTripWire(const string& path);
	void MountChestSide(const string& entitychest, const string& dst);
	void MountChestFront(const string& entitychest, const string& dst);
	void MountChestTop(const string& entitychest, const string& dst);
	
	void operator << (ofstream& file, vector<string>& vec);
	
	std::vector<std::string> outputLog;
	const vector<string> defaultAnimated = {
		"cauldron_water.png",
		"fire_0.png",
		"fire_1.png",
		"lava_flow.png",
		"lava_still.png",
		"portal.png",
		"prismarine_rough.png",
		"sea_lantern.png",
		"water_flow.png",
		"water_still.png",
		"compass_atlas.png",
		"watch_atlas.png"
	};
	error_code err;
#endif
	Converter_API bool ConvertPack(const std::string& inPack, const std::string& packname, const std::string& output_folder);
	Converter_API const std::vector<std::string> *GetLog();
	Converter_API void Print(const std::string& text);
	Converter_API void ClearLog();
}