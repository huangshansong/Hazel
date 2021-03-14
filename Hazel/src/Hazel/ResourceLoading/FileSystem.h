#pragma once
#include "hzpch.h"

#include "Hazel/HObject.h"

namespace Hazel {

	class HAZEL_API FileSystem : public HObject
	{
	public:
		static inline std::string getPath(std::string relativePath) {

			return "C:/dev/Hazel/Hazel/" + relativePath;

		};
	};
}