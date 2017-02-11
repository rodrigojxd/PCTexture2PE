#include "TextureConverter.h"
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include "tga.h"
#include "opencv2/imgcodecs.hpp"

namespace fs = std::experimental::filesystem;
using namespace cv;

namespace ConverterFuncs {

	Converter_API void Print(const string & text)
	{
		outputLog.push_back(text);
	}
	void delUseless(const string& path)
	{
		for (auto & p : fs::recursive_directory_iterator(path))
		{
			if (fs::path(p).extension() == ".ini" || fs::path(p).extension() == ".bd" ||
				fs::path(p).extension() == ".mcmeta" || fs::path(p).extension() == ".DS_Store")
			{
				fsRemove(fs::path(p).string());
			}
		}
	}
	void fsCopy(const string& old_, const string& new_)
	{
		if (fs::exists(old_))
		{
			Print("Copying: " + old_);
			fs::copy(old_, new_, err);
			if (err)
			{
				Print(err.message());
			}
		}
	}
	void fsCopyDir(const string& old_, const string& new_)
	{
		if (fs::exists(old_))
		{
			Print("Copying: " + old_);
			fs::copy(old_, new_, fs::copy_options::recursive, err);
			if (err)
			{
				Print(err.message());
			}
		}
	}
	void fsRename(const string& old_, const string& new_)
	{
		if (fs::exists(old_))
		{
			Print("Renaming: " + old_);
			fs::rename(old_, new_, err);
			if (err)
			{
				Print(err.message());
			}
		}
	}
	void fsRemove(const string& path)
	{
		if (fs::exists(path))
		{
			Print("Deleting: " + path);
			fs::remove(path, err);
			if (err)
			{
				Print(err.message());
			}
		}
	}
	void fsRemoveAll(const string& path)
	{
		if (fs::exists(path))
		{
			Print("Deleting: " + path);
			fs::remove_all(path, err);
			if (err)
			{
				Print(err.message());
			}
		}
	}
	void fsMoveFile(const string& old_, const string& new_)
	{
		if (fs::exists(old_))
		{
			Print("Moving: " + old_);
			fs::copy(old_, new_, err);
			if (err)
			{
				Print(err.message());
			}
			fsRemove(old_);
		}
	}
	void fsMoveDir(const string& old_, const string& new_)
	{
		if (fs::exists(old_))
		{
			Print("Moving: " + old_);
			fs::copy(old_, new_, fs::copy_options::recursive, err);
			if (err)
			{
				Print(err.message());
			}
			fsRemoveAll(old_);
		}
	}
	void fsCreate_directory(const string& path)
	{
		Print("Creating: " + path);
		fs::create_directory(path, err);
		if (err)
		{
			Print(err.message());
		}
	}
	
	const std::string newUUID()
	{
		UUID uuid;
		UuidCreate(&uuid);

		unsigned char *str;
		UuidToStringA(&uuid, &str);

		std::string s((char*)str);

		RpcStringFreeA(&str);
		return s;
	}

	Mat PushOnSide(const Mat& img, const Mat& img2)
	{
		Size sz1 = img.size();
		Size sz2 = img2.size();

		Mat img3(sz1.height, sz1.width + sz2.width, 24); //24 = gbra
		img.copyTo(img3(Rect(0, 0, sz1.width, sz1.height)));
		img2.copyTo(img3(Rect(sz1.width, 0, sz2.width, sz2.height)));

		return img3;
	}
	Mat blend_multiply(const Mat& level1, const int& opacity, const Color& cor)
	{
		// Get 4 channel float images
		Mat4f src1;

		if (level1.channels() == 3)
		{
			Mat4b tmp1;
			cvtColor(level1, tmp1, COLOR_BGR2BGRA);
			tmp1.convertTo(src1, CV_32F, 1. / 255.);
		}
		else
		{
			level1.convertTo(src1, CV_32F, 1. / 255.);
		}

		Mat4f dst(src1.rows, src1.cols, Vec4f(0., 0., 0., 0.));

		// Loop on every pixel

		float fopacity = opacity / 255.f;
		float comp_alpha, new_alpha;
		float mult = 1.0f / 255.0f; //To convert to float pixel
		const Vec4f v2 = { mult * (float)cor.b, mult * (float)cor.g, mult * (float)cor.r, mult * (float)cor.a };
		for (int r = 0; r < src1.rows; ++r)
		{
			for (int c = 0; c < src1.cols; ++c)
			{
				const Vec4f& v1 = src1(r, c);
				Vec4f& out = dst(r, c);

				comp_alpha = min(v1[3], v2[3]) * fopacity;
				new_alpha = v1[3] + (1.f - v1[3]) * comp_alpha;

				if ((comp_alpha > 0.) && (new_alpha > 0.))
				{
					float ratio = comp_alpha / new_alpha;

					out[0] = max(0.f, min(v1[0] * v2[0], 1.f)) * ratio + (v1[0] * (1.f - ratio));
					out[1] = max(0.f, min(v1[1] * v2[1], 1.f)) * ratio + (v1[1] * (1.f - ratio));
					out[2] = max(0.f, min(v1[2] * v2[2], 1.f)) * ratio + (v1[2] * (1.f - ratio));
				}
				else
				{
					out[0] = v1[0];
					out[1] = v1[1];
					out[2] = v1[2];
				}

				out[3] = v1[3]; //keep transparency

			}
		}
		Mat4b dst4b;
		dst.convertTo(dst4b, CV_8U, 255.);
		return dst4b;
	}
	void EqualizeSizes(Mat& img, Mat& img2)
	{
		if (img.cols > img2.cols || img.rows > img2.rows)
		{
			resize(img2, img2, Size(img.cols, img.rows), 0.0, 0.0, INTER_NEAREST);
		}
		else if (img2.cols > img.cols || img2.rows > img.rows)
		{
			resize(img, img, Size(img2.cols, img2.rows), 0.0, 0.0, INTER_NEAREST);
		}
	}
	void CopyTo(const Mat4b& src, Mat4b& dst, const int& alphaFilter)
	{
		for (int r = 0; r < dst.rows; ++r)
		{
			for (int c = 0; c < dst.cols; ++c)
			{
				const Vec4b& vf = src(r, c);
				if (vf[3] > alphaFilter) // alpha channel
				{
					Vec4b& vb = dst(r, c);
					vb = vf;
				}
			}
		}
	}
	
	void CutHalf(const string& old, const string& new_)
	{ //crop half of the image (e.g for zombie)

		if (fs::exists(old))
		{
			Mat image = imread(old, IMREAD_UNCHANGED);
			if (!image.empty() && (image.cols == image.rows))
			{
				Mat ROI(image, Rect(0, 0, image.cols, image.rows / 2)); //cols = width rows = heigth
				imwrite(new_, ROI);
			}
		}
	}
	void CreateStatic(const string& old, const string& new_)
	{ //crop 1 frame of the image (e.g for placeholder textures of animated blocks)

		if (fs::exists(old))
		{
			Print("Creating static texture: " + new_);
			Mat image = imread(old, IMREAD_UNCHANGED);
			if (!image.empty())
			{
				Mat ROI(image, Rect(0, 0, image.cols, image.cols));
				imwrite(new_, ROI);
			}
		}
	}
	void CreateItemAtlas(const string& path, const string& staticName, const string& atlasName)
	{
		if (fs::exists(path + "00" + ".png"))
		{
			Print("Creating atlas texture: " + atlasName);
			//read the first
			Mat4b image = imread(path + "00" + ".png", -1);
			fsRemove(path + "00" + ".png");
			if (!image.empty())
			{
				//push 2-10
				for (int i = 1; i <= 9; i++)
				{
					const string path1 = path + "0" + to_string(i) + ".png";
					if (fs::exists(path1))
					{
						Mat4b image2 = imread(path1, -1);
						if (!image2.empty() && image.cols == image2.cols)
						{
							image.push_back(image2);
							fsRemove(path1);
						}
					}
				}
				//push while find
				bool findNext = true;
				int cout = 10;
				while (findNext)
				{
					const string path1 = path + to_string(cout) + ".png";
					if (fs::exists(path1))
					{
						Mat4b image2 = imread(path1, IMREAD_UNCHANGED);
						if (!image2.empty() && image.cols == image2.cols)
						{
							image.push_back(image2);
							fsRemove(path1);
							cout++;
						}
					}
					else
					{
						findNext = false;
					}
				}
				imwrite(atlasName, image);
			}
			if (fs::exists(path + "20.png"))
			{
				fsCopy(path + path + "20.png", staticName); //static item texture
			}
		}
	}
	void OverlapImg(const string& overlay, const string& base, const string& newPath)
	{
		if (fs::exists(overlay) && fs::exists(base))
		{
			Mat4b img_back = imread(base, IMREAD_UNCHANGED);
			Mat4b img2_over = imread(overlay, IMREAD_UNCHANGED);

			if (!img2_over.empty() && !img_back.empty())
			{
				Print("Overlapping: " + newPath);

				EqualizeSizes(img_back, img2_over);

				CopyTo(img2_over, img_back, 0);

				imwrite(newPath, img_back);
			}
		}
	}
	void OverlapImg(const string& overlay, const string& base)
	{
		OverlapImg(overlay, base, overlay);
	}
	void OverlapImgWithColor(const string& path, const string& pathNew, const Color& color)
	{
		if (fs::exists(path))
		{
			Print("Overlapping: " + pathNew);
			Mat bkg = imread(path, IMREAD_UNCHANGED);
			Mat4b blend = blend_multiply(bkg, 255, color);
			if (!blend.empty())
			{
				imwrite(pathNew, blend);
			}
		}
	}
	void CreateOpaque(const string& path, const string& pathNew, const int& grayLevel)
	{
		if (fs::exists(path))
		{
			Print("Creating opaque: " + pathNew);

			Mat4b img = imread(path, IMREAD_UNCHANGED);
			Mat3b roi = Mat(img.rows, img.cols, CV_32SC3);

			for (int r = 0; r < roi.rows; ++r)
			{
				for (int c = 0; c < roi.cols; ++c)
				{
					const Vec4b& vf = img(r, c);
					Vec3b& vb = roi(r, c);
					if (vf[3] > 0) // alpha channel > 0
					{
						vb[0] = vf[0];
						vb[1] = vf[1];
						vb[2] = vf[2];
					}
					else
					{
						vb[0] = grayLevel;
						vb[1] = grayLevel;
						vb[2] = grayLevel;
					}
				}
			}
			imwrite(pathNew, roi);
		}
	}
	void CreateTgaWithOverlay(const string& path, const string& overlay, const int& method, const string& newPath)
	{
		//method = 0 -> path has alpha, method = 1 -> ovelay has alpha  (for lether armor)
		if (fs::exists(path) && fs::exists(overlay))
		{
			Print("Creating tga: " + newPath);
			Mat4b img2 = imread(overlay, IMREAD_UNCHANGED);
			Mat4b img = imread(path, IMREAD_UNCHANGED);

			EqualizeSizes(img, img2);

			//overlay
			CopyTo(img2, img, 254);

			flip(img, img, 0); //flip the image (tga save it inverted)
			flip(img2, img2, 0);

			TGAImage *tga = new TGAImage(img.cols, img.rows);
			Color tmpC;

			for (int x = 0; x < img.cols; x++)
			{
				for (int y = 0; y < img.rows; y++)
				{
					Vec4b& vf = img(y, x);
					Vec4b& vf2 = img2(y, x);

					tmpC.b = vf[0];
					tmpC.g = vf[1];
					tmpC.r = vf[2];

					if (method == 0)
					{
						if (vf2[3] < 255)
						{
							tmpC.a = 0;
						}
						else
						{
							tmpC.a = 255;
						}
					}
					else if (method == 1)
					{
						if (vf[3] < 255)
						{
							tmpC.a = 0;
						}
						else
						{
							tmpC.a = 255;
						}
						if (vf2[3] == 255)
						{
							tmpC.a = 128;
						}
					}

					tga->setPixel(tmpC, y, x);
				}
			}
			tga->WriteImage(newPath);
			delete tga;
		}
	}
	void CreatePistonARM(const string& BlocksPath, const string& PistonARMPath)
	{
		const string top = BlocksPath + "piston_top_normal.png";
		const string topStick = BlocksPath + "piston_top_sticky.png";
		const string side = BlocksPath + "piston_side.png";

		if (fs::exists(top) && fs::exists(topStick) && fs::exists(side))
		{
			Print("Creating PistonARM: " + PistonARMPath);
			//load images
			Mat4b PistonTop = imread(top, IMREAD_UNCHANGED);
			Mat4b PistonSide = imread(side, IMREAD_UNCHANGED);
			Mat4b pistonSideSticky = imread(topStick, IMREAD_UNCHANGED);

			//check for error
			if (!PistonTop.empty() && !PistonSide.empty() && !pistonSideSticky.empty())
			{

				//texture multiplier (eg. 2 for 32x textures)
				const int mult = PistonTop.cols / 16;

				//final texture
				Mat4b pistonARM = Mat(32 * mult, 128 * mult, 24, Scalar(0, 0, 0, 0));

				//top texture normal
				Mat4b top_1 = PushOnSide(PistonTop, PistonTop);
				Mat4b roiARM = pistonARM(Rect(16 * mult, 0, 32 * mult, 16 * mult));
				top_1.copyTo(roiARM);

				//side texture
				Mat4b side_1 = PistonSide(Rect(0, 0, 16 * mult, 4 * mult));
				side_1 = PushOnSide(side_1, side_1);
				side_1 = PushOnSide(side_1, side_1); //push the image 4x
				roiARM = pistonARM(Rect(0, 16 * mult, 64 * mult, 4 * mult));
				side_1.copyTo(roiARM);

				//Extending part 1
				side_1 = PistonSide(Rect(0, 0, 8 * mult, 4 * mult));
				transpose(side_1, side_1); //rotate 90
				side_1 = PushOnSide(side_1, side_1);
				side_1 = PushOnSide(side_1, side_1); //push the image 4x
				roiARM = pistonARM(Rect(64 * mult, 4 * mult, 16 * mult, 8 * mult));
				side_1.copyTo(roiARM);

				//Extending part 2
				Mat4b strip_h = PistonSide(Rect(2 * mult, 0, 5 * mult, mult));
				Mat4b strip_h_2 = PistonSide(Rect(5 * mult, 0, 3 * mult, mult));
				strip_h = PushOnSide(strip_h, strip_h_2);
				Mat4b strip_v;
				transpose(strip_h, strip_v);

				Mat4b edge = Mat(6 * mult, 6 * mult, 24, Scalar(0, 0, 0, 0));

				//horizontal botton
				Mat4b roi_edge = edge(Rect(0, 5 * mult, 6 * mult, mult));
				Mat4b roi_strip = strip_h(Rect(2 * mult, 0, 6 * mult, mult));
				roi_strip.copyTo(roi_edge);

				//horizontal top
				flip(strip_h, strip_h, 0);
				roi_edge = edge(Rect(0, 0, 6 * mult, mult));
				roi_strip = strip_h(Rect(0, 0, 6 * mult, mult));
				roi_strip.copyTo(roi_edge);

				//vertical rigth
				roi_edge = edge(Rect(5 * mult, mult, mult, 4 * mult));
				roi_strip = strip_v(Rect(0, 0, mult, 4 * mult));
				roi_strip.copyTo(roi_edge);

				//vertical left
				flip(strip_v, strip_v, 1);
				roi_edge = edge(Rect(0, mult, mult, 4 * mult));
				roi_strip = strip_v(Rect(0, 2 * mult, mult, 4 * mult));
				roi_strip.copyTo(roi_edge);

				edge = PushOnSide(edge, edge);
				roiARM = pistonARM(Rect(70 * mult, 18 * mult, 12 * mult, 6 * mult));
				edge.copyTo(roiARM);

				//Extending part 3
				side_1 = PistonSide(Rect(mult, 0, 6 * mult, 4 * mult));
				transpose(side_1, side_1);
				resize(side_1, side_1, Size(6 * mult, 8 * mult), 0.0, 0.0, INTER_NEAREST);
				side_1 = PushOnSide(side_1, side_1);
				side_1 = PushOnSide(side_1, side_1); //push the image 4x

				roiARM = pistonARM(Rect(64 * mult, 24 * mult, 24 * mult, 8 * mult));
				side_1.copyTo(roiARM);

				//save normal piston
				fsCreate_directory(PistonARMPath);
				imwrite(PistonARMPath + "pistonArm.png", pistonARM);

				//overlay top texture with sticky texture
				Mat4b roiARMS = pistonARM(Rect(16 * mult, 0, 16 * mult, 16 * mult));
				pistonSideSticky.copyTo(roiARMS);

				//save sticky texture
				imwrite(PistonARMPath + "pistonArmSticky.png", pistonARM);
				fsRemove(BlocksPath + "piston_tmp.png");
			}
		}
	}
	void OverlapImgAndColor(const string& basePath, const string& OverlayPath, const Color& color, const string& result)
	{ //for potions/tripped arrows
		Print("Overlapping: " + result);
		Mat Overlay = imread(OverlayPath, IMREAD_UNCHANGED);
		Mat4b img_back = imread(basePath, IMREAD_UNCHANGED);

		EqualizeSizes(Overlay, img_back);

		Mat4b img2_over = blend_multiply(Overlay, 255, color);

		CopyTo(img_back, img2_over, 128);

		cv::imwrite(result, img2_over);
	}
	void CreateSheepTga(const string& path, const string& FurPath, const string& newPath)
	{
		if (fs::exists(path) && fs::exists(FurPath))
		{
			Print("Creating tga: " + newPath);
			Mat4b sheep = imread(path, IMREAD_UNCHANGED);
			Mat4b fur = imread(FurPath, IMREAD_UNCHANGED);

			flip(sheep, sheep, 0);
			flip(fur, fur, 0);
			TGAImage* sheep_ = new TGAImage(sheep.cols, sheep.rows + fur.rows);
			Color tmpC;

			for (int x = 0; x < sheep.cols; x++)
			{
				for (int y = 0; y < sheep.rows; y++)
				{

					Vec4b& vb = fur(y, x);

					tmpC.b = vb[0];
					tmpC.g = vb[1];
					tmpC.r = vb[2];
					if (vb[3] >= 254)//keep the alpha for the sheep color overlay
					{ 
						tmpC.a = 255;
					}
					else
					{
						tmpC.a = 0;
					}
					sheep_->setPixel(tmpC, y, x);

					Vec4b& vf = sheep(y, x);

					tmpC.b = vf[0];
					tmpC.g = vf[1];
					tmpC.r = vf[2];
					tmpC.a = 0;  //discards the alpha

					sheep_->setPixel(tmpC, y + sheep.rows, x); //save above of the fur
				}
			}
			sheep_->WriteImage(newPath);
			fsRemove(path);
			fsRemove(FurPath);
			delete sheep_;
		}
	}
	void FlipImage90(const string& path, const string& newPath)
	{
		if (fs::exists(path))
		{
			Print("Flipping : " + newPath);
			Mat img = imread(path, IMREAD_UNCHANGED);
			transpose(img, img);
			imwrite(newPath, img);
		}
	}
	void CreateSpawnEgg(const string& basePath, const string& OverlayPath,
		const Color& color, const Color& color2, const string& result)
	{

		Print("Creating spawn egg : " + result);

		Mat4b Overlay = imread(OverlayPath, IMREAD_UNCHANGED);
		Mat4b base = imread(basePath, IMREAD_UNCHANGED);

		EqualizeSizes(base, Overlay);

		Mat4b img2_over = blend_multiply(Overlay, 255, color2);
		Mat4b img_back = blend_multiply(base, 255, color);

		CopyTo(img2_over, img_back, 128);

		cv::imwrite(result, img_back);
	}
	void OverlapEnderman(const string& base, const string& overlay)
	{
		if (fs::exists(base) && fs::exists(overlay))
		{
			Print("Overlapping : " + base);
			Mat4b img_back = imread(base, IMREAD_UNCHANGED);
			Mat4b img2_over = imread(overlay, IMREAD_UNCHANGED);

			EqualizeSizes(img_back, img2_over);

			for (int r = 0; r < img_back.rows; ++r)
			{
				for (int c = 0; c < img_back.cols; ++c)
				{
					const Vec4b& vf = img2_over(r, c);
					if (!(vf[0] == 0 && vf[1] == 0 && vf[2] == 0))
					{
						Vec4b& vb = img_back(r, c);
						vb = vf;
					}
				}
			}
			imwrite(base, img_back);
		}
	}
	void CreateCauldronWater(const string& path, const string& newPath)
	{
		if (fs::exists(path))
		{

			Mat img_2 = imread(path, IMREAD_GRAYSCALE); //load converting in grayscale
			Mat4b img;
			cvtColor(img_2, img, COLOR_GRAY2BGRA); //convert to bgra

			UCHAR highestValue = 0;

			if (!img.empty())
			{

				//get the highest gray value
				for (int c = 0; c < img.cols; ++c)
				{
					for (int r = 0; r < img.rows; ++r)
					{
						const Vec4b& rf = img(r, c);
						if (rf[0] > highestValue)//all channels are the same
						{ 
							highestValue = rf[0];
						}
					}
				}

				for (int c = 0; c < img.cols; ++c)
				{
					for (int r = 0; r < img.rows; ++r)
					{
						Vec4b& rf = img(r, c);
						//transform in a 'white scale' image increasing the gray value
						if (rf[0] - highestValue < 0)
						{
							rf[0] += 255 - highestValue;
						}
						else
						{
							rf[0] = 255;
						}
						rf[1] = rf[2] = rf[0];
						//set transparency
						rf[3] = 190;
					}
				}
				imwrite(newPath, img);
			}
		}
	}
	void CheckForAnimated(const string& path)
	{
		for (auto & p : fs::recursive_directory_iterator(path))
		{
			fs::path name = fs::path(p).filename();

			bool isNotDefaultAnimated = true;
			for (uint i = 0; i < defaultAnimated.size(); ++i)
			{
				isNotDefaultAnimated = name != defaultAnimated[i] && isNotDefaultAnimated;
			}
			if (isNotDefaultAnimated)
			{
				Mat img = imread(p.path().string(), IMREAD_UNCHANGED);
				if (img.rows > img.cols)
				{
					Print("Cropping : " + p.path().string());
					Mat ROI(img, Rect(0, 0, img.cols, img.cols));
					imwrite(p.path().string(), ROI);
				}
				else if (img.rows < img.cols)
				{
					Print("Cropping : " + p.path().string());
					Mat ROI(img, Rect(0, 0, img.rows, img.rows));
					imwrite(p.path().string(), ROI);
				}
			}
		}
	}
	void CropTripWire(const string& path)
	{//MCPE uses only the second part/frame of the texture
		if (fs::exists(path))
		{
			Mat img = imread(path, IMREAD_UNCHANGED);
			if (!img.empty())
			{
				int multp = img.rows / 16;
				Mat ROI(img, Rect(0, 4 * multp, img.cols, 4 * multp));
				Mat img2 = Mat(img.rows, img.cols, img.type(), Scalar(0, 0, 0, 0));
				Mat ROI2(img2, Rect(0, 0, img.cols, 4 * multp));
				ROI.copyTo(ROI2);
				imwrite(path, img2);
			}
		}
	}

	void MountChestSide(const string & entitychest, const string& dst)
	{
		if (fs::exists(entitychest))
		{
			Mat source = imread(entitychest, IMREAD_UNCHANGED);
			if (!source.empty())
			{
				//get the texture resolution multiplier
				int multp = source.rows / 64;
				//copy the top of side texture
				Mat ROI(source, Rect(0, 14 * multp, 14 * multp, 5 * multp));
				//copy the bottom of side texture
				Mat ROI2(source, Rect(0, 34 * multp, 14 * multp, 9 * multp));

				//create a 14*multp x 14*multp empty image
				Mat dst_ = Mat(14 * multp, 14 * multp, source.type(), Scalar(0, 0, 0, 0));
				//mark the position of dst_ where will paste the ROI
				Mat ROId(dst_, Rect(0, 0, 14 * multp, 5 * multp));
				//paste the ROI to the specift position
				ROI.copyTo(ROId);
				//mark the position of dst_ where will paste the ROI2
				ROId = Mat(dst_, Rect(0, 5 * multp, 14 * multp, 9 * multp));
				ROI2.copyTo(ROId);
				
				//resize the image to 16*multp x 16*multp
				resize(dst_, dst_, Size(16 * multp, 16 * multp), 0.0, 0.0, INTER_NEAREST);

				imwrite(dst, dst_);
			}
		}
	}
	void MountChestFront(const string & entitychest, const string & dst)
	{
		if (fs::exists(entitychest))
		{
			Mat source = imread(entitychest, IMREAD_UNCHANGED);
			if (!source.empty())
			{
				int multp = source.rows / 64;
				Mat ROI(source, Rect(14 * multp, 14 * multp, 14 * multp, 5 * multp));
				Mat ROI2(source, Rect(14 * multp, 34 * multp, 14 * multp, 9 * multp));
				Mat ROI3(source, Rect(4 * multp, multp, 2 * multp, 4 * multp));

				Mat dst_ = Mat(14 * multp, 14 * multp, source.type(), Scalar(0, 0, 0, 0));
				Mat ROId(dst_, Rect(0, 0, 14 * multp, 5 * multp));
				ROI.copyTo(ROId);
				ROId = Mat(dst_, Rect(0, 5 * multp, 14 * multp, 9 * multp));
				ROI2.copyTo(ROId);
				ROId = Mat(dst_, Rect(6 * multp, 3 * multp, 2 * multp, 4 * multp));
				ROI3.copyTo(ROId);

				resize(dst_, dst_, Size(16 * multp, 16 * multp), 0.0, 0.0, INTER_NEAREST);

				imwrite(dst, dst_);
			}
		}
	}
	void MountChestTop(const string & entitychest, const string & dst)
	{
		if (fs::exists(entitychest))
		{
			Mat source = imread(entitychest, IMREAD_UNCHANGED);
			if (!source.empty())
			{
				int multp = source.rows / 64;
				Mat ROI(source, Rect(14 * multp, 0, 14 * multp, 14 * multp));

				Mat dst_ = Mat(14 * multp, 14 * multp, source.type(), Scalar(0, 0, 0, 0));
				Mat ROId(dst_, Rect(0, 0, 14 * multp, 14 * multp));
				ROI.copyTo(ROId);

				resize(dst_, dst_, Size(16 * multp, 16 * multp), 0.0, 0.0, INTER_NEAREST);

				imwrite(dst, dst_);
			}
		}
	}
	
	void operator << (ofstream& file, vector<string>& vec)
	{
		string s;
		for (uint i = 0; i < vec.size(); ++i)
		{
			s += vec[i];
		}
		file << s;
	}

	Converter_API bool ConvertPack(const std::string& inPack, const std::string& packname, const std::string& output_folder)
	{
		//check the pack
		if (packname == "")
		{
			Print("Error: Input texture pack folder not found.");
			return false;
		}

		if (!fs::exists(inPack + "\\pack.mcmeta"))
		{
			Print("Error: 'pack.mcmeta' not found. Check the pack and try again.");
			return false;
		}

		//folder of the ouput pack 
		const string outPack = output_folder + "\\" + packname;

		//Clean the folder if it already exists
		fsCreate_directory(outPack);

		//sets up the manifest
		const string description = "Converted by PCTexture2PE";

		manifestModel[1] = newUUID();
		manifestModel[3] = packname;
		manifestModel[5] = manifestModel[7] = description;
		manifestModel[9] = newUUID();

		ofstream manifest;
		

		Print("Creating: " + outPack + "\\pack_manifest.json");
		manifest.open(outPack + "\\pack_manifest.json");
		if (manifest.fail())
		{
			Print("Error: Failed to save file: 'pack_manifest.json'.");
			return false;
		}
		manifest << manifestModel;
		manifest.close();

		//Some texture directories
		const string blocksPath = outPack + "\\textures\\blocks\\";
		const string entityPath = outPack + "\\textures\\entity\\";
		const string itemsPath = outPack + "\\textures\\items\\";
		const string armorPath = outPack + "\\textures\\models\\armor\\";

		fsCreate_directory(outPack + "\\textures");

		//copy the textures from input pack

#define copyTexture(old_,new_) fsCopyDir(inPack + "\\assets\\minecraft\\textures\\" + old_, new_);

		copyTexture("blocks", blocksPath);
		copyTexture("entity", entityPath);
		copyTexture("environment", outPack + "\\textures\\environment");
		copyTexture("items", itemsPath);
		copyTexture("map", outPack + "\\textures\\map");
		copyTexture("misc", outPack + "\\textures\\misc");
		copyTexture("models", outPack + "\\textures\\models");
		copyTexture("painting", outPack + "\\textures\\painting");
		copyTexture("particle", outPack + "\\textures\\particle");
		//copyTexture("font"       , outPack + "\\textures\\font"); //MCPE does not have a good font support

		fsCreate_directory(outPack + "\\textures\\gui");
		copyTexture("gui\\title\\background", outPack + "\\textures\\gui\\background");
		copyTexture("colormap", outPack + "\\textures\\colormap");

#undef copyTexture

		fsCopy(inPack + "\\pack.png", outPack + "\\pack_icon.png");

		//Rename the textures to the correct name
		fsRename(entityPath + "cat\\black.png", entityPath + "cat\\blackcat.png");
		fsRename(entityPath + "chest\\normal_double.png", entityPath + "chest\\double_normal.png");
		fsRename(entityPath + "rabbit\\black.png", entityPath + "rabbit\\blackrabbit.png");
		fsRename(entityPath + "wither", entityPath + "wither_boss");
		fsRename(entityPath + "arrow.png", entityPath + "arrows.png");
		fsRename(entityPath + "enderdragon", entityPath + "dragon");

		fsRename(blocksPath + "fire_layer_0.png", blocksPath + "fire_0.png");
		fsRename(blocksPath + "fire_layer_1.png", blocksPath + "fire_1.png");

		fsRename(itemsPath + "acacia_boat.png", itemsPath + "boat_acacia.png");
		fsRename(itemsPath + "birch_boat.png", itemsPath + "boat_birch.png");
		fsRename(itemsPath + "dark_oak_boat.png", itemsPath + "boat_darkoak.png");
		fsRename(itemsPath + "jungle_boat.png", itemsPath + "boat_jungle.png");
		fsRename(itemsPath + "oak_boat.png", itemsPath + "boat_oak.png");
		fsRename(itemsPath + "spruce_boat.png", itemsPath + "boat_spruce.png");
		fsRename(itemsPath + "beetroot_seeds.png", itemsPath + "seeds_beetroot.png");
		fsRename(itemsPath + "dragon_breath.png", itemsPath + "dragons_breath.png");

		fsRename(outPack + "\\textures\\painting\\paintings_kristoffer_zetterstrand.png", outPack + "\\textures\\painting\\kz.png");

		for (int i = 0; i <= 9; i++)
		{
			fsMoveFile(blocksPath + "destroy_stage_" + to_string(i) + ".png",
				outPack + "\\textures\\environment\\destroy_stage_" + to_string(i) + ".png");
		}

		CutHalf(entityPath + "zombie_pigman.png", entityPath + "pig\\pigzombie.png");
		CutHalf(entityPath + "zombie\\zombie.png", entityPath + "zombie\\zombie.png");
		CutHalf(entityPath + "zombie\\husk.png", entityPath + "zombie\\husk.png");

		CreateStatic(blocksPath + "fire_0.png", blocksPath + "fire_0_placeholder.png");
		CreateStatic(blocksPath + "fire_1.png", blocksPath + "fire_1_placeholder.png");
		CreateStatic(blocksPath + "lava_still.png", blocksPath + "lava_placeholder.png");
		CreateStatic(blocksPath + "portal.png", blocksPath + "portal_placeholder.png");
		CreateStatic(blocksPath + "water_still.png", blocksPath + "water_placeholder.png");

		CreateItemAtlas(itemsPath + "compass_", itemsPath + "compass_item.png", itemsPath + "compass_atlas.png");
		CreateItemAtlas(itemsPath + "clock_", itemsPath + "clock_item.png", itemsPath + "watch_atlas.png");


		OverlapImg(entityPath + "pig\\pig_saddle.png", entityPath + "pig\\pig.png");
		OverlapImg(itemsPath + "map_filled_markings.png", itemsPath + "map_filled.png", itemsPath + "map_filled.png");

		fsCreate_directory(entityPath + "skulls");

		fsCopy(entityPath + "creeper\\creeper.png", entityPath + "skulls\\creeper.png");
		fsCopy(entityPath + "skeleton\\skeleton.png", entityPath + "skulls\\skeleton.png");
		fsCopy(entityPath + "skeleton\\wither_skeleton.png", entityPath + "skulls\\wither_skeleton.png");
		fsCopy(entityPath + "zombie\\zombie.png", entityPath + "skulls\\zombie.png");
		fsCopy(entityPath + "bear\\polarbear.png", entityPath + "polarbear.png");
		fsCopy(entityPath + "elytra.png", armorPath + "elytra.png");
		fsCopy(entityPath + "dragon\\dragon_fireball.png", itemsPath + "dragon_fireball.png");

		fsCopy(blocksPath + "grass_side.png", blocksPath + "grass_side_carried.png");

		CreateOpaque(blocksPath + "leaves_acacia.png", blocksPath + "leaves_acacia_opaque.png", 44);
		CreateOpaque(blocksPath + "leaves_big_oak.png", blocksPath + "leaves_big_oak_opaque.png", 44);
		CreateOpaque(blocksPath + "leaves_birch.png", blocksPath + "leaves_birch_opaque.png", 44);
		CreateOpaque(blocksPath + "leaves_jungle.png", blocksPath + "leaves_jungle_opaque.png", 75);
		CreateOpaque(blocksPath + "leaves_oak.png", blocksPath + "leaves_oak_opaque.png", 44);
		CreateOpaque(blocksPath + "leaves_spruce.png", blocksPath + "leaves_spruce_opaque.png", 49);

#define createCarried(base,name,color) OverlapImgWithColor(blocksPath + base , blocksPath + name , color);

		createCarried("double_plant_fern_top.png", "double_plant_fern_carried.png", Color(79, 156, 37));
		createCarried("double_plant_grass_top.png", "double_plant_grass_carried.png", Color(79, 156, 37));
		createCarried("tallgrass.png", "tallgrass_carried.png", Color(79, 156, 37));
		createCarried("fern.png", "fern_carried.png", Color(79, 156, 37));
		createCarried("grass_top.png", "grass_carried.png", Color(120, 182, 66));
		createCarried("leaves_acacia.png", "leaves_acacia_carried.png", Color(68, 177, 8));
		createCarried("leaves_big_oak.png", "leaves_big_oak_carried.png", Color(68, 177, 8));
		createCarried("leaves_birch.png", "leaves_birch_carried.png", Color(108, 151, 71));
		createCarried("leaves_jungle.png", "leaves_jungle_carried.png", Color(70, 176, 37));
		createCarried("leaves_oak.png", "leaves_oak_carried.png", Color(68, 177, 8));
		createCarried("leaves_spruce.png", "leaves_spruce_carried.png", Color(98, 146, 98));
		createCarried("vine.png", "vine_carried.png", Color(30, 128, 48));
		createCarried("waterlily.png", "carried_waterlily.png", Color(30, 128, 48));

#undef createCarried

		CreateTgaWithOverlay(blocksPath + "grass_side.png", blocksPath + "grass_side_overlay.png", 0, blocksPath + "grass_side.tga");
		CreateTgaWithOverlay(entityPath + "wolf\\wolf_tame.png", entityPath + "wolf\\wolf_collar.png", 0, entityPath + "wolf\\wolf_tame.tga");

		CreateSheepTga(entityPath + "sheep\\sheep.png", entityPath + "sheep\\sheep_fur.png", entityPath + "sheep\\sheep.tga");

		CreatePistonARM(blocksPath, entityPath + "pistonarm\\");

		string emptyBottle = itemsPath + "potion_bottle_empty.png";
		const string potionOverlay = itemsPath + "potion_overlay.png";
		string potion = itemsPath + "potion_bottle_";

		if (fs::exists(potionOverlay))
		{
#define overlapPotion(color,name) OverlapImgAndColor(emptyBottle, potionOverlay, color, potion + name);
			if (fs::exists(emptyBottle))
			{
				overlapPotion(Color(36, 81, 162), "absorption.png");
				overlapPotion(Color(13, 31, 34), "blindness.png");
				overlapPotion(Color(84, 29, 73), "confusion.png");
				overlapPotion(Color(145, 35, 34), "damageBoost.png");
				overlapPotion(Color(73, 65, 23), "digSlowdown.png");
				overlapPotion(Color(214, 189, 66), "digSpeed.png");
				overlapPotion(Color(55, 92, 195), "drinkable.png");
				overlapPotion(Color(224, 152, 57), "fireResistance.png");
				overlapPotion(Color(66, 10, 9), "harm.png");
				overlapPotion(Color(244, 35, 34), "heal.png");
				overlapPotion(Color(244, 123, 34), "healthBoost.png");
				overlapPotion(Color(87, 116, 82), "hunger.png");
				overlapPotion(Color(125, 129, 144), "invisibility.png");
				overlapPotion(Color(33, 251, 75), "jump.png");
				overlapPotion(Color(89, 106, 127), "moveSlowdown.png");
				overlapPotion(Color(122, 172, 195), "moveSpeed.png");
				overlapPotion(Color(31, 31, 158), "nightVision.png");
				overlapPotion(Color(77, 145, 48), "poison.png");
				overlapPotion(Color(202, 91, 168), "regeneration.png");
				overlapPotion(Color(151, 68, 57), "resistance.png");
				overlapPotion(Color(244, 35, 34), "saturation.png");
				overlapPotion(Color(45, 81, 151), "waterBreathing.png");
				overlapPotion(Color(71, 76, 71), "weakness.png");
				overlapPotion(Color(51, 40, 37), "wither.png");
			}
			emptyBottle = itemsPath + "potion_bottle_splash.png";
			potion = itemsPath + "potion_bottle_splash";

			if (fs::exists(emptyBottle))
			{
				overlapPotion(Color(36, 81, 162), "_absorption.png");
				overlapPotion(Color(13, 31, 34), "_blindness.png");
				overlapPotion(Color(84, 29, 73), "_confusion.png");
				overlapPotion(Color(145, 35, 34), "_damageBoost.png");
				overlapPotion(Color(73, 65, 23), "_digSlowdown.png");
				overlapPotion(Color(214, 189, 66), "_digSpeed.png");
				overlapPotion(Color(224, 152, 57), "_fireResistance.png");
				overlapPotion(Color(66, 10, 9), "_harm.png");
				overlapPotion(Color(244, 35, 34), "_heal.png");
				overlapPotion(Color(244, 123, 34), "_healthBoost.png");
				overlapPotion(Color(87, 116, 82), "_hunger.png");
				overlapPotion(Color(125, 129, 144), "_invisibility.png");
				overlapPotion(Color(33, 251, 75), "_jump.png");
				overlapPotion(Color(89, 106, 127), "_moveSlowdown.png");
				overlapPotion(Color(122, 172, 195), "_moveSpeed.png");
				overlapPotion(Color(31, 31, 158), "_nightVision.png");
				overlapPotion(Color(77, 145, 48), "_poison.png");
				overlapPotion(Color(202, 91, 168), "_regeneration.png");
				overlapPotion(Color(151, 68, 57), "_resistance.png");
				overlapPotion(Color(244, 35, 34), "_saturation.png");
				overlapPotion(Color(45, 81, 151), "_waterBreathing.png");
				overlapPotion(Color(71, 76, 71), "_weakness.png");
				overlapPotion(Color(51, 40, 37), "_wither.png");
				overlapPotion(Color(55, 92, 195), ".png"); //will change the color of template (must be the last)
			}
			emptyBottle = itemsPath + "potion_bottle_lingering.png";
			potion = itemsPath + "potion_bottle_lingering";

			if (fs::exists(emptyBottle))
			{
				overlapPotion(Color(36, 81, 162), "_absorption.png");
				overlapPotion(Color(13, 31, 34), "_blindness.png");
				overlapPotion(Color(84, 29, 73), "_confusion.png");
				overlapPotion(Color(145, 35, 34), "_damageBoost.png");
				overlapPotion(Color(73, 65, 23), "_digSlowdown.png");
				overlapPotion(Color(214, 189, 66), "_digSpeed.png");
				overlapPotion(Color(224, 152, 57), "_fireResistance.png");
				overlapPotion(Color(66, 10, 9), "_harm.png");
				overlapPotion(Color(244, 35, 34), "_heal.png");
				overlapPotion(Color(244, 123, 34), "_healthBoost.png");
				overlapPotion(Color(87, 116, 82), "_hunger.png");
				overlapPotion(Color(125, 129, 144), "_invisibility.png");
				overlapPotion(Color(33, 251, 75), "_jump.png");
				overlapPotion(Color(89, 106, 127), "_moveSlowdown.png");
				overlapPotion(Color(122, 172, 195), "_moveSpeed.png");
				overlapPotion(Color(31, 31, 158), "_nightVision.png");
				overlapPotion(Color(77, 145, 48), "_poison.png");
				overlapPotion(Color(202, 91, 168), "_regeneration.png");
				overlapPotion(Color(151, 68, 57), "_resistance.png");
				overlapPotion(Color(244, 35, 34), "_saturation.png");
				overlapPotion(Color(45, 81, 151), "_waterBreathing.png");
				overlapPotion(Color(71, 76, 71), "_weakness.png");
				overlapPotion(Color(51, 40, 37), "_wither.png");
				overlapPotion(Color(55, 92, 195), ".png"); //will change the color of template (must be the last)
			}
#undef overlapPotion
		}

		const string tippedBase = itemsPath + "tipped_arrow_base.png";
		const string tippedHead = itemsPath + "tipped_arrow_head.png";
		const string tippedArrow = itemsPath + "tipped_arrow";

		if (fs::exists(tippedBase) && fs::exists(tippedHead))
		{
#define overlapArrow(color,name) OverlapImgAndColor(tippedBase, tippedHead, color, tippedArrow + name);

			overlapArrow(Color(255, 255, 255), ".png");
			overlapArrow(Color(145, 35, 34), "_strength.png");
			overlapArrow(Color(224, 152, 57), "_fireres.png");
			overlapArrow(Color(66, 10, 9), "_harm.png");
			overlapArrow(Color(244, 35, 34), "_healing.png");
			overlapArrow(Color(125, 129, 144), "_invisibility.png");
			overlapArrow(Color(33, 251, 75), "_leaping.png");
			overlapArrow(Color(89, 106, 127), "_slow.png");
			overlapArrow(Color(122, 172, 195), "_swift.png");
			overlapArrow(Color(31, 31, 158), "_nightvision.png");
			overlapArrow(Color(77, 145, 48), "_poison.png");
			overlapArrow(Color(202, 91, 168), "_regen.png");
			overlapArrow(Color(45, 81, 151), "_waterbreathing.png");
			overlapArrow(Color(71, 76, 71), "_weakness.png");

#undef overlapArrow
		}
		FlipImage90(blocksPath + "redstone_dust_line1.png", blocksPath + "redstone_dust_line.png");
		OverlapImg(blocksPath + "redstone_dust_dot.png", blocksPath + "redstone_dust_line.png", blocksPath + "redstone_dust_cross.png");
		OverlapImg(blocksPath + "redstone_dust_cross.png", blocksPath + "redstone_dust_line0.png");


		OverlapImgWithColor(armorPath + "leather_layer_1.png", armorPath + "cloth_1.png", Color(167, 105, 67));
		OverlapImgWithColor(armorPath + "leather_layer_2.png", armorPath + "cloth_2.png", Color(167, 105, 67));

		CreateTgaWithOverlay(armorPath + "leather_layer_1.png", armorPath + "leather_layer_1_overlay.png", 1, armorPath + "leather_1.tga");
		CreateTgaWithOverlay(armorPath + "leather_layer_2.png", armorPath + "leather_layer_2_overlay.png", 1, armorPath + "leather_2.tga");

		CreateTgaWithOverlay(itemsPath + "leather_boots.png", itemsPath + "leather_boots_overlay.png", 1, itemsPath + "leather_boots.tga");
		CreateTgaWithOverlay(itemsPath + "leather_helmet.png", itemsPath + "leather_helmet_overlay.png", 1, itemsPath + "leather_helmet.tga");
		CreateTgaWithOverlay(itemsPath + "leather_leggings.png", itemsPath + "leather_leggings_overlay.png", 1, itemsPath + "leather_leggings.tga");

		const string spawnegg = itemsPath + "spawn_egg.png";
		const string spawneggOv = itemsPath + "spawn_egg_overlay.png";
		const string eggpath = itemsPath + "egg_";

		if (fs::exists(spawnegg) && fs::exists(spawneggOv))
		{
#define createSpawnEgg(color_1,color_2,name) CreateSpawnEgg(spawnegg, spawneggOv, color_1, color_2, eggpath + name);

			createSpawnEgg(Color(71, 58, 45), Color(10, 10, 10), "bat.png");
			createSpawnEgg(Color(255, 184, 0), Color(255, 248, 120), "blaze.png");
			createSpawnEgg(Color(11, 62, 73), Color(158, 13, 13), "cave_spider.png");
			createSpawnEgg(Color(161, 161, 161), Color(255, 0, 0), "chicken.png");
			createSpawnEgg(Color(64, 51, 36), Color(154, 154, 154), "cow.png");
			createSpawnEgg(Color(13, 168, 11), Color(0, 0, 0), "creeper.png");
			createSpawnEgg(Color(78, 65, 54), Color(123, 108, 93), "donkey.png");
			createSpawnEgg(Color(199, 197, 180), Color(106, 109, 137), "elderguardian.png");
			createSpawnEgg(Color(21, 21, 21), Color(18, 18, 18), "enderman.png");
			createSpawnEgg(Color(21, 21, 21), Color(18, 18, 18), "endermite.png");
			createSpawnEgg(Color(234, 234, 234), Color(167, 167, 167), "ghast.png");
			createSpawnEgg(Color(85, 122, 107), Color(225, 117, 46), "guardian.png");
			createSpawnEgg(Color(187, 154, 122), Color(241, 132, 0), "horse.png");
			createSpawnEgg(Color(216, 192, 136), Color(96, 89, 74), "husk.png");
			createSpawnEgg(Color(49, 0, 0), Color(236, 236, 0), "lava_slime.png");
			createSpawnEgg(Color(25, 0, 0), Color(81, 51, 30), "mule.png");
			createSpawnEgg(Color(156, 16, 17), Color(175, 175, 175), "mushroomcow.png");
			createSpawnEgg(Color(255, 255, 255), Color(255, 255, 255), "null.png");
			createSpawnEgg(Color(232, 215, 120), Color(76, 60, 46), "ocelot.png");
			createSpawnEgg(Color(233, 160, 157), Color(201, 92, 87), "pig.png");
			createSpawnEgg(Color(226, 142, 142), Color(72, 108, 39), "pigzombie.png");
			createSpawnEgg(Color(151, 94, 63), Color(113, 71, 48), "rabbit.png");
			createSpawnEgg(Color(224, 224, 224), Color(233, 165, 165), "sheep.png");
			createSpawnEgg(Color(111, 111, 111), Color(97, 97, 97), "silverfish.png");
			createSpawnEgg(Color(187, 187, 187), Color(70, 70, 70), "skeleton.png");
			createSpawnEgg(Color(107, 107, 81), Color(210, 210, 198), "skeletonhorse.png");
			createSpawnEgg(Color(81, 160, 61), Color(118, 178, 103), "slime.png");
			createSpawnEgg(Color(57, 49, 43), Color(157, 13, 13), "spider.png");
			createSpawnEgg(Color(37, 64, 82), Color(104, 126, 142), "squid.png");
			createSpawnEgg(Color(151, 166, 167), Color(93, 113, 113), "stray.png");
			createSpawnEgg(Color(90, 62, 54), Color(174, 128, 105), "villager.png");
			createSpawnEgg(Color(53, 0, 0), Color(76, 149, 58), "witch.png");
			createSpawnEgg(Color(25, 25, 25), Color(69, 74, 74), "wither.png");
			createSpawnEgg(Color(208, 206, 206), Color(189, 160, 138), "wolf.png");
			createSpawnEgg(Color(0, 172, 172), Color(112, 144, 94), "zombie.png");
			createSpawnEgg(Color(25, 41, 20), Color(138, 177, 121), "zombiehorse.png");
			createSpawnEgg(Color(240, 240, 240), Color(139, 139, 135), "polarbear.png");
			createSpawnEgg(Color(144, 98, 147), Color(69, 49, 76), "shulker.png");

#undef createSpawnEgg
		}

		OverlapEnderman(entityPath + "enderman\\enderman.png", entityPath + "enderman\\enderman_eyes.png");

		CreateCauldronWater(blocksPath + "water_still.png", blocksPath + "cauldron_water.png");
		CreateStatic(blocksPath + "cauldron_water.png", blocksPath + "cauldron_water_placeholder.png");
		
		CropTripWire(blocksPath + "trip_wire.png");
		
		MountChestSide(entityPath + "chest\\normal.png", blocksPath + "chest_side.png");
		MountChestFront(entityPath + "chest\\normal.png", blocksPath + "chest_front.png");
		MountChestTop(entityPath + "chest\\normal.png", blocksPath + "chest_top.png");
		MountChestSide(entityPath + "chest\\ender.png", blocksPath + "ender_chest_side.png");
		MountChestFront(entityPath + "chest\\ender.png", blocksPath + "ender_chest_front.png");
		MountChestTop(entityPath + "chest\\ender.png", blocksPath + "ender_chest_top.png");
		MountChestFront(entityPath + "chest\\trapped.png", blocksPath + "trapped_chest_front.png");

		//del useless files
		{
#define removeEntityDir(name) fsRemoveAll(entityPath + name);
			removeEntityDir("armorstand");
			removeEntityDir("banner");
			removeEntityDir("bear"); //copied before
			removeEntityDir("illager");
			removeEntityDir("llama");
			removeEntityDir("projectiles");
			removeEntityDir("shield");
#undef  removeEntityDir

#define removeEntity(name) fsRemove(entityPath + name);
			removeEntity("enderman\\enderman_eyes,png");
			removeEntity("banner_base.png");
			removeEntity("elytra.png"); //copied before
			removeEntity("end_gateway_beam.png");
			removeEntity("explosion.png");
			removeEntity("shield_base.png");
			removeEntity("shield_base_nopattern.png");
			removeEntity("sweep.png");
			removeEntity("wolf\\wolf_tame.png");
			removeEntity("wolf\\wolf_collar.png");
			removeEntity("zombie_pigman.png");
			removeEntity("dragon\\dragon_fireball.png"); //copied before
#undef  removeEntity

#define removeItem(name) fsRemove(itemsPath + name);
			removeItem("leather_boots.png");
			removeItem("leather_boots_overlay.png");
			removeItem("leather_helmet.png");
			removeItem("leather_helmet_overlay.png");
			removeItem("leather_leggings.png");
			removeItem("leather_leggings_overlay.png");
			removeItem("leather_chestplate_overlay.png");
			removeItem("banner_base.png");
			removeItem("banner_overlay.png");
			removeItem("empty_armor_slot_shield.png");
			removeItem("fireworks.png");
			removeItem("fireworks_charge.png");
			removeItem("fireworks_charge_overlay.png");
			removeItem("record_11.png");
			removeItem("record_13.png");
			removeItem("record_blocks.png");
			removeItem("record_cat.png");
			removeItem("record_chirp.png");
			removeItem("record_far.png");
			removeItem("record_mall.png");
			removeItem("record_mellohi.png");
			removeItem("record_stal.png");
			removeItem("record_strad.png");
			removeItem("record_wait.png");
			removeItem("record_ward.png");
			removeItem("ruby.png");
			removeItem("structure_void.png");
			removeItem("totem.png");
			removeItem("wooden_armorstand.png");
#undef  removeItem

#define removeBlock(name) fsRemove(blocksPath + name);
			removeBlock("grass_side.png");
			removeBlock("grass_side_overlay.png");
			removeBlock("redstone_dust_line0.png");
			removeBlock("redstone_dust_dot.png");
			removeBlock("redstone_dust_line1.png");
			removeBlock("redstone_dust_overlay.png");
			removeBlock("bone_block_side.png");
			removeBlock("bone_block_top.png");
			removeBlock("chain_command_block_back.png");
			removeBlock("chain_command_block_conditional.png");
			removeBlock("chain_command_block_front.png");
			removeBlock("chain_command_block_side.png");
			removeBlock("command_block_back.png");
			removeBlock("command_block_conditional.png");
			removeBlock("command_block_front.png");
			removeBlock("command_block_side.png");
			removeBlock("debug.png");
			removeBlock("debug2.png");
			removeBlock("frosted_ice_0.png");
			removeBlock("frosted_ice_1.png");
			removeBlock("frosted_ice_2.png");
			removeBlock("frosted_ice_3.png");
			removeBlock("glass_black.png");
			removeBlock("glass_blue.png");
			removeBlock("glass_brown.png");
			removeBlock("glass_cyan.png");
			removeBlock("glass_gray.png");
			removeBlock("glass_green.png");
			removeBlock("glass_light_blue.png");
			removeBlock("glass_lime.png");
			removeBlock("glass_magenta.png");
			removeBlock("glass_orange.png");
			removeBlock("glass_pane_top_black.png");
			removeBlock("glass_pane_top_blue.png");
			removeBlock("glass_pane_top_brown.png");
			removeBlock("glass_pane_top_cyan.png");
			removeBlock("glass_pane_top_gray.png");
			removeBlock("glass_pane_top_green.png");
			removeBlock("glass_pane_top_light_blue.png");
			removeBlock("glass_pane_top_lime.png");
			removeBlock("glass_pane_top_magenta.png");
			removeBlock("glass_pane_top_orange.png");
			removeBlock("glass_pane_top_purple.png");
			removeBlock("glass_pane_top_pink.png");
			removeBlock("glass_pane_top_red.png");
			removeBlock("glass_pane_top_silver.png");
			removeBlock("glass_pane_top_white.png");
			removeBlock("glass_pane_top_yellow.png");
			removeBlock("glass_pink.png");
			removeBlock("glass_purple.png");
			removeBlock("glass_red.png");
			removeBlock("glass_silver.png");
			removeBlock("glass_white.png");
			removeBlock("glass_yellow.png");
			removeBlock("jukebox_top.png");
			removeBlock("jukebox_side.png");
			removeBlock("magma.png");
			removeBlock("nether_wart_block.png");
			removeBlock("red_nether_brick.png");
			removeBlock("repeating_command_block_back.png");
			removeBlock("repeating_command_block_conditional.png");
			removeBlock("repeating_command_block_front.png");
			removeBlock("repeating_command_block_side.png");
			removeBlock("shulker_top_black.png");
			removeBlock("shulker_top_blue.png");
			removeBlock("shulker_top_brown.png");
			removeBlock("shulker_top_cyan.png");
			removeBlock("shulker_top_gray.png");
			removeBlock("shulker_top_green.png");
			removeBlock("shulker_top_light_blue.png");
			removeBlock("shulker_top_lime.png");
			removeBlock("shulker_top_magenta.png");
			removeBlock("shulker_top_orange.png");
			removeBlock("shulker_top_pink.png");
			removeBlock("shulker_top_purple.png");
			removeBlock("shulker_top_red.png");
			removeBlock("shulker_top_silver.png");
			removeBlock("shulker_top_white.png");
			removeBlock("shulker_top_yellow.png");
			removeBlock("structure_block.png");
			removeBlock("structure_block_corner.png");
			removeBlock("structure_block_data.png");
			removeBlock("structure_block_load.png");
			removeBlock("structure_block_save.png");
			removeBlock("water_overlay.png");
#undef  removeBlock

			fsRemove(armorPath + "leather_layer_1.png");
			fsRemove(armorPath + "leather_layer_2.png");
			fsRemove(armorPath + "leather_layer_1_overlay.png");
			fsRemove(armorPath + "leather_layer_2_overlay.png");


			fsRemove(outPack + "\\textures\\misc\\forcefield.png");
			fsRemove(outPack + "\\textures\\misc\\underwater.png");
			fsRemove(outPack + "\\textures\\particle\\footPrint.png");

			delUseless(outPack);
		}

		CheckForAnimated(itemsPath);
		CheckForAnimated(blocksPath);
		return true;
	}
	Converter_API const std::vector<std::string> *GetLog()
	{
		return &outputLog;
	}
	Converter_API void ClearLog()
	{
		outputLog.clear();
	}
}


