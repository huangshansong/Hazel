#pragma once
#include "hzpch.h"

namespace Hazel {

	class FileSystem {
	public:
		static inline std::string getPath(std::string relativePath) {

			return "C:/dev/Hazel/Hazel/" + relativePath;

		};
	};
}