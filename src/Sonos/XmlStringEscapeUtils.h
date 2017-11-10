#ifndef XMLSTRINGESCAPEUTILS_H
#define XMLSTRINGESCAPEUTILS_H

#include <vector>
#include <string>

/**
 * Helper class to escape special characters in strings for XML.
 */
class XmlStringEscapeUtils {

public:

    /**
     * Check if the given string is already escaped (XML 1.1).
     * @param const std::string& s
     */
    static bool isXml11Escaped(const std::string& s);
    
    /**
     * Escape special characters in the given string (XML 1.1).
     * @param std::string& s
     */
    static void escapeXml11(std::string& s);

private:

    /**
     * Private default constructor.
     */
    XmlStringEscapeUtils(void);
    
    /**
     * Private copy constructor.
     */
    XmlStringEscapeUtils(const XmlStringEscapeUtils&);

    /**
     * Private assignment operator.
     */
    XmlStringEscapeUtils& operator=(const XmlStringEscapeUtils&);

    static std::vector<std::string> s_escapeList;
};

#endif // XMLSTRINGESCAPEUTILS_H