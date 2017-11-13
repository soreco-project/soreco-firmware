#ifndef SONOSCOMMANDBUILDER_H
#define SONOSCOMMANDBUILDER_H

#include <Arduino.h>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <IPAddress.h>
#include <WiFiClient.h>

/**
 * Helper class to create, execute and process the SOAP UPnP commands.
 */
class SonosCommandBuilder {

public:

    SonosCommandBuilder(const std::string& endpoint, const std::string& service, const std::string& action);
    static std::string getDeviceDescription(const IPAddress& ip);

    static SonosCommandBuilder transport(const std::string& action);
    static SonosCommandBuilder rendering(const std::string& action);
    static SonosCommandBuilder device(const std::string& action);
    static SonosCommandBuilder contentDirectory(const std::string& action);
    static SonosCommandBuilder zoneGroupTopology(const std::string& action);

    SonosCommandBuilder& put(const std::string& key, std::string value);
    std::string executeOn(const IPAddress& ip);

private:

    // constants
    static const uint16_t SOAP_PORT = 1400;
    static const char* TRANSPORT_ENDPOINT;
    static const char* TRANSPORT_SERVICE;
    static const char* RENDERING_ENDPOINT;
    static const char* RENDERING_SERVICE;
    static const char* DEVICE_ENDPOINT;
    static const char* DEVICE_SERVICE;
    static const char* CONTENT_DIRECTORY_ENDPOINT;
    static const char* CONTENT_DIRECTORY_SERVICE;
    static const char* ZONE_GROUP_TOPOLOGY_ENDPOINT;
    static const char* ZONE_GROUP_TOPOLOGY_SERVICE;

    /**
     * Private default constructor.
     */
    SonosCommandBuilder(void);
    
    /**
     * Private copy constructor.
     */
    SonosCommandBuilder(const SonosCommandBuilder&);

    /**
     * Private assignment operator.
     */
    SonosCommandBuilder& operator=(const SonosCommandBuilder&);

    std::string getBody(void);
    void readResponse(WiFiClient& stream, std::string& result);

    // instance members
    std::string m_endpoint;
    std::string m_service;
    std::string m_action;
    std::unordered_map<std::string, std::string> m_bodyEntries;
};

#endif //SONOSCOMMANDBUILDER_H