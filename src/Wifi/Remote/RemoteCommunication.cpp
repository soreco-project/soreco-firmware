#include "RemoteCommunication.h"


#include "../../DeviceSettings/DeviceSettings.h"

RemoteCommunication::RemoteCommunication(RemoteEventHandlerIfc& handler) :
    m_server(RemoteCommunication::PORT),
    m_eventHandler(handler) {
}

RemoteCommunication::~RemoteCommunication(void) {
}

void RemoteCommunication::setup(void) {
    m_server.begin();
}

bool RemoteCommunication::loop(void) {
    WiFiClient client = m_server.available();
    // check for a client to connect
    if (client && client.connected()) {
        byte buffer[MAX_PACKAGE_SIZE];
        int bytesRead = client.readBytes(buffer, sizeof(buffer));
        Serial.print(F("received bytes: ")); Serial.println(bytesRead);
        
        if (bytesRead > 0) {
            _Protocol builder = processRequest(buffer);

            // switch dependend on received message
            if (builder.which_payload == Protocol_WifiConfig_tag) {
                handleWifiConfig(builder.payload.WifiConfig);
            } else {
                // TODO handle unsupported message
            }

            // TODO extract
            // create response message and send back to client
            pb_ostream_t streamOut = pb_ostream_from_buffer(buffer, sizeof(buffer));
            builder.payload.Response = ResponseType_OK;
            builder.which_payload = Protocol_Response_tag;
            bool statusEncode = pb_encode(&streamOut, Protocol_fields, &builder);
            int builder_length = streamOut.bytes_written;
            for (int i = 0; i < builder_length; i++) {
                client.write(buffer[i]);
            }
        }
    }

    // stop client request
    client.stop();
}

// decode received builder
_Protocol& RemoteCommunication::processRequest(byte* pData) {
    _Protocol builder = Protocol_init_zero;
    pb_istream_t streamIn = pb_istream_from_buffer(pData, WifiConfigMessage_size);
    bool statusDecode = pb_decode(&streamIn, Protocol_fields, &builder);

    return builder;
}

void RemoteCommunication::handleWifiConfig(const WifiConfigMessage& msg) {
    DeviceSettings::WiFiConfig config = DeviceSettings::getWiFiConfig();    
    memcpy(config.ssid, msg.ssid, sizeof(msg.ssid));
    memcpy(config.passphrase, msg.password, sizeof(msg.password));

    DeviceSettings::setWiFiConfig(config);
    DeviceSettings::save();

    m_eventHandler.onEventWifiConfigReceived();
}