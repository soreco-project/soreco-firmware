#ifndef SONOSRESPONSEPARSER_H
#define SONOSRESPONSEPARSER_H

#include <string>

/**
 * Helper class to parse and read values from the Sonos SOAP UPnP response.
 */
class SonosResponseParser {

public:

    static std::string findOne(const std::string& startTag, const std::string& endTag, const std::string& response);

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