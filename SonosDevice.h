/*************************************************** 
 Representation of a Sonos device.
 ****************************************************/

#ifndef SONOSDEVICE_H
#define SONOSDEVICE_H

#include "IPAddress.h"
#include <vector>

class SonosDevice {
public:

    enum PlayState {
        // Player has an error
        ERROR,
        // Player is stopped
        STOPPED,
        // Player is playing
        PLAYING,
        // Player is paused
        PAUSED_PLAYBACK,
        // Player is loading
        TRANSITIONING
    };

    /**
     * Constructor.
     */
    SonosDevice(IPAddress ipAddress);

    /**
     * Destructor.
     */
    ~SonosDevice(void);

    /**
     * Get the play state of the device.
     * @return current PlayState of the device
     */
    PlayState getPlayState(void);

    /**
     * Play a given stream. Pauses the queue.
     * @param uri URI of a stream to be played.
     * @param meta The track metadata to show in the player (DIDL format).
     */
    void playUri(String uri, String meta);    

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
    int getVolume(void);

    /**
     * Set the Sonos speaker volume.
     * @param volume A volume value between 0 and 100
     */
    void setVolume(int volume);

    /**
     * Return the mute state of the Sonos speaker.
     * @return True if is muted, false if isn't
     */
    bool isMuted(void);

    /**
     * Mute or unmute the Sonos speaker.
     * @param state true to mute, false to unmute
     */
    void setMute(bool state);

    /**
     * Get the zone name of the Sonos speaker.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     */
    String getZoneName(void);

    /**
     * Return if the Sonos is joined with another one.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     * @return True if is joined, false if is isn't
     */
    bool isJoined(void);

    /**
     * Check if the speaker is a group coordinator or not.
     * Note: If a speaker is joined with other speakers in the zone, only the group coordinator can perform actions
     * @return True if the speaker is a group coordinator, otherwise False.
     */
    bool isCoordinator(void);

private:
    /**
     * Private default constructor.
     */
    SonosDevice(void);

    /**
     * Private copy constructor.
     */
    SonosDevice(const SonosDevice&);

    /**
     * Private assignment constructor.
     */
    SonosDevice& operator=(const SonosDevice&);
    
    // instance fields
    IPAddress m_ipAddress;
};

#endif //  SONOSDEVICE_H
