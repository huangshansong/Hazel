#pragma once
#include "hzpch.h"

#include "Hazel/HObject.h"

namespace Hazel {

	enum class FileSuffix
	{
		obj = 0,
		fbx,
		jpg,
		png
	};
	const char* const fileSuffixName[] =
	{
		".obj",
		".fbx",
		".jpg",
		".png"
	};

	class HAZEL_API FileSystem : public HObject
	{
	public:
		static inline std::string getPath(std::string relativePath) {

			return "C:/dev/Hazel/Hazel/" + relativePath;

		};
	};
}