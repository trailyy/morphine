#pragma once
#include <string>
#include <sstream>
#include <vector>

/*
* shoutout protonsdk, shoutout seth
* 
* https://github.com/SethRobinson/proton/blob/master/shared/util/TextScanner.cpp
* https://github.com/SethRobinson/proton/blob/master/shared/util/MiscUtils.cpp
*/

std::vector<std::string> StringTokenize(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> string_vector;

	if (!str.empty())
	{
		size_t  start = 0, end = 0;

		while (end != std::string::npos)
		{
			end = str.find(delimiter, start);

			// If at end, use length=maxLength.  Else use length=end-start.
			string_vector.push_back(str.substr(start,
				(end == std::string::npos) ? std::string::npos : end - start));

			// If at end, use start=maxSize.  Else use start=end+delimiter.
			start = ((end > (std::string::npos - delimiter.size()))
				? std::string::npos : end + delimiter.size());
		}
	}

	return string_vector;
}

class TextScanner
{
public:
	TextScanner() = default;
	TextScanner(const std::string& text) { Load(text); }

	void Load(const std::string& text)
	{
		std::istringstream ss(text);
		std::string line;

		while (std::getline(ss, line))
			m_lines.push_back(line);
	}

	std::string Dump() const
	{
		std::ostringstream ss;

		for (const std::string& line : m_lines)
			ss << line << '\n';

		return ss.str();
	}

	void Set(const std::string& key, const std::string& value, std::size_t index = 0)
	{
		for (std::string& line : m_lines)
		{
			auto data = StringTokenize(line, "|");

			if (data.size() < (index + 2))
				continue;

			if (data[0] == key)
			{
				line.replace(line.find(data[1 + index]), data[1 + index].size(), value);
				return;
			}
		}
	}

	std::string Get(const std::string& key, std::size_t index = 0) const noexcept
	{
		for (const std::string& line : m_lines)
		{
			auto data = StringTokenize(line, "|");

			if (data.size() < (index + 2))
				continue;

			if (data[0] == key)
				return data[1 + index];
		}

		return {};
	}

	bool Has(const std::string& key) const
	{
		for (const std::string& line : m_lines)
		{
			auto data = StringTokenize(line, "|");

			if (data.empty())
				continue;

			if (data[0] == key)
				return true;
		}

		return false;
	}

	void Remove(const std::string& key, std::size_t index = 0)
	{
		auto it = std::find_if(m_lines.begin(), m_lines.end(), [&](const std::string& line) { auto data = StringTokenize(line, "|"); if (!data.empty() && data[0] == key) return true; });

		if (it != m_lines.end())
			m_lines.erase(it);
	}
private:
	std::vector<std::string> m_lines;
};