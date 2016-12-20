# PCTexture2PE

Texture pack converter (MCPC to MCPE)

## How to compile

You need Visual Studio 2015 to compile this. You can download the community version for free here: https://www.visualstudio.com/downloads/

In the installation configuration select only the Visual C++/Commom Tools for Visual c++ 2015, the other things are optional.

Clone this repository.

```sh
git clone https://github.com/rodrigojxd/PCTexture2PE.git
```

Download the opencv files. Create a folder named "opencv" in the directory "ProjectFolder/ConverterDll/ConverterDl" and extract the two folders from .zip file to this folder.
(If you want you can compile your own binaries from [opencv repository](https://github.com/opencv/opencv))

```
https://github.com/rodrigojxd/PCTexture2PE/files/629215/opencv_files.zip
```

Open PCTexture2PE.sln in the Visual Studio and change the settings to Release and x86. After that, click Build.
