#pragma once

#include <string_view>
#include <vector>

namespace StringUtilities 
{
	inline const std::vector<std::string_view>& split(std::string_view str, const char del) 
	{
		std::vector<std::string_view> strs;
		int prePos = 0;
	
		for (int i = 0; i < str.size(); i++) {
			if (str.at(i) == del) {
				strs.push_back(str.substr(prePos, i));
				prePos = i + 1;
			}
		}
		strs.push_back(str.substr(prePos, str.size() - prePos));

		return strs;
	}
}