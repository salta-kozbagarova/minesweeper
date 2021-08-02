#ifndef RUSSIAN_H
#define RUSSIAN_H
#pragma once

#include <iostream>
#include <locale>
#include <string>
#include <vector>

class russian
{
public:
	russian(){}
	std::wstring MakeRussian(std::wstring const &s, char default_char = '?')
	{
		locale loc(russian_locale_designator);
		return this->wide_string(this->narrow_string(s, loc), loc);
	}
	std::wstring wide_string(std::string const &s, std::locale const &loc)
	{
		if (s.empty())
			return std::wstring();
		std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);
		char const *first = s.c_str();
		char const *last = first + s.size();
		std::vector<wchar_t> result(s.size());

		facet.widen(first, last, &result[0]);

		return std::wstring(result.begin(), result.end());
	}

	std::string narrow_string(std::wstring const &s, std::locale const &loc, char default_char = '?')
	{
		if (s.empty())
			return std::string();
		std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);
		wchar_t const *first = s.c_str();
		wchar_t const *last = first + s.size();
		std::vector<char> result(s.size());

		facet.narrow(first, last, default_char, &result[0]);

		return std::string(result.begin(), result.end());
	}
	char const *russian_locale_designator = "rus"; // обозначение локали зависит от реализации,
	// "rus" подходит для VC++
};



#endif