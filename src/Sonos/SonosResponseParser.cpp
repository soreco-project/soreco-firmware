#include "SonosResponseParser.h"

std::string SonosResponseParser::findOne(const std::string& startTag, const std::string& endTag, const std::string& response) {
    size_t start = response.find(startTag);
    size_t end = response.find(endTag);

    if ((start != std::string::npos) && (end != std::string::npos)) {
        start += startTag.length();
        end = end + startTag.length() - endTag.length() + 1;
        return response.substr(start, end - start);
    } else {
        return "";
    }
}

std::vector<std::string> SonosResponseParser::split(const std::string& s, char delimiter) {
    // note: we cannot use std::stringstream in Arduino since it will create linker errors
    std::vector<std::string> items;
    std::size_t start = 0;
    std::size_t end;
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        std::size_t length = end - start;
        std::string item = std::string(s, start, length);
        items.push_back(item);
        start = end + 1;
    }
    items.push_back(std::string(s, start));
    return items;
}