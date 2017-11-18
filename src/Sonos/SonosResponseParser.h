#ifndef SONOSRESPONSEPARSER_H
#define SONOSRESPONSEPARSER_H

#include <string>
#include <vector>

/**
 * Helper class to parse and read values from the Sonos SOAP UPnP response.
 */
class SonosResponseParser {

public:

    /**
     * Find the value between the given startTag and endTag in the given response.
     */
    static std::string findOne(const std::string& startTag, const std::string& endTag, const std::string& response);

    /**
     * Splits a string into substrings that are that are delimited by the given character.
     */
    static std::vector<std::string> split(const std::string& s, char delimiter);

private:

    /**
     * Private default constructor.
     */
    SonosResponseParser(void);

    /**
     * Private copy constructor.
     */
    SonosResponseParser(const SonosResponseParser&);

    /**
     * Private assignment operator.
     */
    SonosResponseParser& operator=(const SonosResponseParser&);
};

#endif // SONOSRESPONSEPARSER_H