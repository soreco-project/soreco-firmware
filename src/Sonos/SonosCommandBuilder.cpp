#include "SonosCommandBuilder.h"

#include <vector>
#include <algorithm>

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
    if(!isEscaped(value)) {
        escapeXml11(value);
    }
    m_bodyEntries.insert({key, value});
    return *this;
}

std::string SonosCommandBuilder::executeOn(const IPAddress& ip, WiFiClient& client) {
    if (client.connect(ip, UPNP_PORT)) {
        std::string arguments = getBody();
        Serial.println(F("FIXME: SonosCommandBuilder - verify contentLength with Wireshark"));
        int contentLength = 246 + m_service.length() + m_action.length() + m_action.length() + m_service.length() + arguments.length() + m_action.length();

        client.print(F("POST "));
        client.print(m_endpoint.c_str());
        client.println(F("HTTP/1.1"));
        client.print(F("Host: "));
        client.println(ip);
        client.print(F("Content-Length: "));
        client.println(contentLength);
        client.println(F("Content-Type: text/xml; charset=utf-8"));
        client.print(F("Soapaction: "));
        client.print(m_service.c_str());
        client.print(F("#"));
        client.println(m_action.c_str());
        client.println(F("Connection: close"));
        client.println("");
        client.print(F("<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\""));
        client.print(F("s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"));
        client.print(F("<s:Body>"));
        client.print(F("<u:"));
        client.print(m_action.c_str());
        client.print(F(" xmlns:u=\""));
        client.print(m_service.c_str());
        client.print(F("\">"));
        client.print(arguments.c_str());
        client.print(F("</u:"));
        client.print(m_action.c_str());
        client.print(F(">"));
        client.print(F("</s:Body>"));
        client.println(F("</s:Envelope>"));

        //wait if client is not immediately available
        unsigned long starttimer = millis();
        const unsigned long timelimit = 10000;
        bool timeout = false;
        while (!client.available() && !timeout) {
            //wait until client available
            delay(100);
            if ((millis() - starttimer) > timelimit) {
                timeout = true;
            }
        }
        //timeout shall keep current state for the coming while-loop. Only starttimer should be updated
        starttimer = millis();
        std::string result;
        while (client.available() && !timeout) {
            const char c = client.read();
            result.append(&c);
            if ((millis() - starttimer) > timelimit) {
                timeout = true;
            }
        }
        if (timeout) {
            client.stop();
        }
        return result;
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

bool SonosCommandBuilder::isEscaped(const std::string& s) {
    std::vector<std::string> escapeList {"&amp;", "&lt;", "&gt;", "&quot;", "&apos;"};
    return std::find(escapeList.begin(), escapeList.end(), s) != escapeList.end();
}

void SonosCommandBuilder::escapeXml11(std::string& s) {
    // we expect less than 5% to be escaped
    std::string buffer;
    buffer.reserve(s.length() * 1.05);
    for(size_t i = 0; i != s.length(); ++i) {
        switch(s[i]) {
            case '&':
                buffer.append("&amp;");
                break;
                case '<':
                buffer.append("&lt;");
                break;
            case '>':
                buffer.append("&gt;");
                break;
            case '\"':
                buffer.append("&quot;");
                break;
            case '\'':
                buffer.append("&apos;");
                break;
            default:
                buffer.append(&s[i], 1);
                break;
        }
    }
    s.swap(buffer);
}