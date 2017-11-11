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