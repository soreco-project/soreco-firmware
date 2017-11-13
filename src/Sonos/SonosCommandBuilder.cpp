#include "SonosCommandBuilder.h"

#include <ESP8266HTTPClient.h>
#include "XmlStringEscapeUtils.h"

// static initialization
const char* SonosCommandBuilder::TRANSPORT_ENDPOINT             = "/MediaRenderer/AVTransport/Control";
const char* SonosCommandBuilder::TRANSPORT_SERVICE              = "urn:schemas-upnp-org:service:AVTransport:1";
const char* SonosCommandBuilder::RENDERING_ENDPOINT             = "/MediaRenderer/RenderingControl/Control";
const char* SonosCommandBuilder::RENDERING_SERVICE              = "urn:schemas-upnp-org:service:RenderingControl:1";
const char* SonosCommandBuilder::DEVICE_ENDPOINT                = "/DeviceProperties/Control";
const char* SonosCommandBuilder::DEVICE_SERVICE                 = "urn:schemas-upnp-org:service:DeviceProperties:1";
const char* SonosCommandBuilder::CONTENT_DIRECTORY_ENDPOINT     = "/MediaServer/ContentDirectory/Control";
const char* SonosCommandBuilder::CONTENT_DIRECTORY_SERVICE      = "urn:schemas-upnp-org:service:ContentDirectory:1";
const char* SonosCommandBuilder::ZONE_GROUP_TOPOLOGY_ENDPOINT   = "/ZoneGroupTopology/Control";
const char* SonosCommandBuilder::ZONE_GROUP_TOPOLOGY_SERVICE    = "urn:upnp-org:serviceId:ZoneGroupTopology";

SonosCommandBuilder::SonosCommandBuilder(const std::string& endpoint, const std::string& service, const std::string& action) {
    m_endpoint = endpoint;
    m_service = service;
    m_action = action;
}


std::string SonosCommandBuilder::getDeviceDescription(const IPAddress& ip) {
    HTTPClient httpClient;
    std::string response;
    if (httpClient.begin(ip.toString(), SOAP_PORT, "/xml/device_description.xml")) {
        int httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payLoad = httpClient.getString();
            return payLoad.c_str();
        }
    }
    return response;
}

SonosCommandBuilder SonosCommandBuilder::transport(const std::string& action) {
    return SonosCommandBuilder(TRANSPORT_ENDPOINT, TRANSPORT_SERVICE, action);
}

SonosCommandBuilder SonosCommandBuilder::rendering(const std::string& action) {
    return SonosCommandBuilder(RENDERING_ENDPOINT, RENDERING_SERVICE, action);
}

SonosCommandBuilder SonosCommandBuilder::device(const std::string& action) {
    return SonosCommandBuilder(DEVICE_ENDPOINT, DEVICE_SERVICE, action);
}

SonosCommandBuilder SonosCommandBuilder::contentDirectory(const std::string& action) {
    return SonosCommandBuilder(CONTENT_DIRECTORY_ENDPOINT, CONTENT_DIRECTORY_SERVICE, action);
}

SonosCommandBuilder SonosCommandBuilder::zoneGroupTopology(const std::string& action) {
    return SonosCommandBuilder(ZONE_GROUP_TOPOLOGY_ENDPOINT, ZONE_GROUP_TOPOLOGY_SERVICE, action);
}

SonosCommandBuilder& SonosCommandBuilder::put(const std::string& key, std::string value) {
    if(!XmlStringEscapeUtils::isXml11Escaped(value)) {
        XmlStringEscapeUtils::escapeXml11(value);
    }
    m_bodyEntries.insert({key, value});
    return *this;
}

std::string SonosCommandBuilder::executeOn(const IPAddress& ip) {
    HTTPClient httpClient;
    if (httpClient.begin(ip.toString(), SOAP_PORT, m_endpoint.c_str())) {
        // Note: this is highly inefficient, but for strange reasons the std::stringstream can't be used here due to linker errors once including <sstream>
        std::string content = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:";
        content += m_action;
        content += " xmlns:u=\"";
        content += m_service;
        content += "\">";
        content += getBody();
        content += "</u:";
        content += m_action;
        content += "></s:Body></s:Envelope>";

        httpClient.addHeader("Content-Type", "text/xml; charset=utf-8");
        std::string soapHeader = m_service + "#" + m_action;
        httpClient.addHeader("SOAPACTION", soapHeader.c_str());

        // perform the actual request
        int httpCode = httpClient.POST(content.c_str());
        if (httpCode == HTTP_CODE_OK) {
            std::string result;
            readResponse(httpClient.getStream(), result);
            return result;
        }
    }
    Serial.print(F("ERROR: Sonos - unable to connect to ")); Serial.println(ip);
    return "";
}

std::string SonosCommandBuilder::getBody(void) {
    std::string str;
    for (auto it : m_bodyEntries) {
        str.append("<").append(it.first).append(">").append(it.second).append("</").append(it.first).append(">");
    }
    return str;
}

void SonosCommandBuilder::readResponse(WiFiClient& stream, std::string& result) {
    // TODO: wait until end of response instead of waiting for a timeout
    const unsigned long timeLimitMs = 1000;
    bool isTimedOut = false;
    unsigned long startTimeMs = millis();
    while (!stream.available() && !isTimedOut) {
        //wait until available
        delay(10);
        if ((millis() - startTimeMs) > timeLimitMs) {
            isTimedOut = true;
        }
    }
    // we have now a connection, reset timeout to wait again for all data
    startTimeMs = millis();
    while (stream.available() && !isTimedOut) {
        const char c = stream.read();
        result += c;
        if ((millis() - startTimeMs) > timeLimitMs) {
            isTimedOut = true;
        }
    }
    if (isTimedOut) {
        stream.stop();
    }
}