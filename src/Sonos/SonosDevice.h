#ifndef SONOSDEVICE_H
#define SONOSDEVICE_H

#include <Arduino.h>
#include <string>
#include <unordered_map>
#include <IPAddress.h>
#include <WiFiClient.h>
#include "SonosZoneInfo.h"

/**
 * Representation of a Sonos device.
 * Note: default destructor, copy constructors and assignment operators are fine since we don't store pointers.
 */
class SonosDevice {

public:

    /**
     * List of possible play states.
     */
    class PlayState {
    public:
        enum Id {
            // Player has an error
            ERROR = 0,
            // Player is stopped
            STOPPED = 1,
            // Player is playing
            PLAYING = 2,
            // Player is paused
            PAUSED_PLAYBACK = 3,
            // Player is loading
            TRANSITIONING = 4
        };

        static PlayState::Id valueOf(const std::string& s);
        static std::string toString(const PlayState::Id value);

    private:
        /**
        * Private copy constructor.
        */
        PlayState(void);

        // static fields
        static std::unordered_map<std::string, PlayState::Id> s_valueMap;
    };

    /**
     * Default constructor.
     */
    SonosDevice(void);

    /**
     * Constructor with IP address and unique id.
     */
    explicit SonosDevice(const IPAddress& ip, const std::string& uuid);

    /**
     * Returns true if the device IP address is valid - false otherwise
     */
    bool isIpValid(void);

    /**
     * Update destination to the given IP address.
     * Note: this method will attempt to get the UUID from the device.
     */
    void setIp(const IPAddress& ip);

    /**
     * Get the IP address of the sonos device.
     */
    IPAddress getIp(void) const;

    /**
     * Get the UUID of the sonos device.
     * Note: if no valid UUID is present, it will attempt to get it from the device.
     */
    std::string getUUID(void);

    /**
     * Get the play state of the device.
     * @return current PlayState of the device
     */
    PlayState::Id getPlayState(void) const;

    /**
     * Play a given stream. Pauses the queue.
     * @param uri URI of a stream to be played.
     * @param meta The track metadata to show in the player (DIDL format).
     */
    void playUri(const std::string& uri, const std::string& meta);

    /**
     * Play the currently selected track.
     */
    void play(void);

    /**
     * Pause the currently playing track.
     */
    void pause(void);

    /**
     * Go to the next track on the queue.
     */
    void next(void);

    /**
     * Go back to the previously played track.
     */
    void previous(void);

    /**
     * Get the Sonos speaker volume.
     * @return A volume value between 0 and 100
     */
    uint8_t getVolume(void) const;

    /**
     * Set the Sonos speaker volume.
     * @param volume A volume value between 0 and 100
     */
    void setVolume(uint8_t volume);

    /**
     * Return the mute state of the Sonos speaker.
     * @return True if is muted, false if isn't
     */
    bool isMuted(void) const;

    /**
     * Mute or unmute the Sonos speaker.
     * @param state true to mute, false to unmute
     */
    void setMute(bool state);

    /**
     * Return the information about the Sonos zone.
     */
    SonosZoneInfo getZoneGroupState(void) const;

    /**
     * Get the room name of the Sonos speaker.
     */
    std::string getRoomName(void) const;

    /**
     * Get the room name of the Sonos speaker.
     * @param xmlDeviceDescription as XML string (from SonosCommandBuilder::getDeviceDescription() to reduce network traffic)
     */
    static std::string getRoomName(const std::string& xmlDeviceDescription);

    /**
     * Return if the Sonos is joined with another one.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     * @return True if is joined, false if is isn't
     */
    bool isJoined(void) const;

    /**
     * Return if the Sonos is joined with another one.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     * @param sonosZoneInfo (from getZoneGroupState() to reduce network traffic)
     * @return True if is joined, false if is isn't
     */
    static bool isJoined(const SonosZoneInfo& sonosZoneInfo);

    /**
     * Check if the speaker is a group coordinator or not.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     * @return True if the speaker is a group coordinator, otherwise False.
     */
    bool isCoordinator(void);

    /**
     * Check if the speaker is a group coordinator or not.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     * @param sonosZoneInfo (from getZoneGroupState() to reduce network traffic)
     * @return True if the speaker is a group coordinator, otherwise False.
     */
    bool isCoordinator(const SonosZoneInfo& sonosZoneInfo);

private:

    bool isUUIDValid(void);
    void updateUUID(void);

    // instance fields
    IPAddress m_ip;
    std::string m_uuid;
};

#endif // SONOSDEVICE_H
