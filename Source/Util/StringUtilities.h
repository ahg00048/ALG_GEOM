#pragma once

#include <string>
#include <string_view>
#include <vector>

const std::vector<std::string_view>& split(std::string_view str, char del) 
{	
	std::vector<std::string_view> sbstrs;
	std::string aux = "";
	unsigned int prePos = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str.at(i) == del) {
			sbstrs.push_back(str.substr(prePos, i));
			prePos = i + 1;
		}
	}

	return sbstrs;
}