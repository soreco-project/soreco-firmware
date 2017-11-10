#include "XmlStringEscapeUtils.h"

#include <algorithm>

// static initialization
std::vector<std::string> XmlStringEscapeUtils::s_escapeList {"&amp;", "&lt;", "&gt;", "&quot;", "&apos;"};

bool XmlStringEscapeUtils::isXml11Escaped(const std::string& s) {
    return std::find(s_escapeList.begin(), s_escapeList.end(), s) != s_escapeList.end();
}

void XmlStringEscapeUtils::escapeXml11(std::string& s) {
    // we expect less than 5% to be escaped
    std::string buffer;
    buffer.reserve(s.length() * 1.05);
    for(size_t i = 0; i != s.length(); ++i) {
        switch(s[i]) {
            case '&':
                buffer.append(s_escapeList[0]);
                break;
            case '<':
                buffer.append(s_escapeList[1]);
                break;
            case '>':
                buffer.append(s_escapeList[2]);
                break;
            case '\"':
                buffer.append(s_escapeList[3]);
                break;
            case '\'':
                buffer.append(s_escapeList[4]);
                break;
            default:
                buffer.append(&s[i], 1);
                break;
        }
    }
    s.swap(buffer);
}