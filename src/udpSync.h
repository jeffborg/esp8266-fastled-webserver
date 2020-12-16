#include <WiFiUdp.h>

typedef struct {
    uint8_t power;
    uint8_t autoplay;
    uint8_t autoplayDuration;
    CRGB solidColor;
    uint8_t currentPatternIndex;
    uint8_t currentPaletteIndex;
    uint8_t brightness;
    uint8_t saturationBpm;
    uint8_t saturationMin;
    uint8_t saturationMax;
    uint8_t brightDepthBpm;
    uint8_t brightDepthMin;
    uint8_t brightDepthMax;
    uint8_t brightThetaIncBpm;
    uint8_t brightThetaIncMin;
    uint8_t brightThetaIncMax;
    uint8_t msMultiplierBpm;
    uint8_t msMultiplierMin;
    uint8_t msMultiplierMax;
    uint8_t hueIncBpm;
    uint8_t hueIncMin;
    uint8_t hueIncMax;
    uint8_t sHueBpm;
    uint8_t sHueMin;
    uint8_t sHueMax;
} Settings;

Settings readGlobals() {
    Settings t;
    t.power = power;
    t.autoplay = autoplay;
    t.autoplayDuration = autoplayDuration;
    t.solidColor = solidColor;
    t.currentPatternIndex = currentPatternIndex;
    t.currentPaletteIndex = currentPaletteIndex;
    t.brightness = brightness;
    t.saturationBpm = saturationBpm;
    t.saturationMin = saturationMin;
    t.saturationMax = saturationMax;
    t.brightDepthBpm = brightDepthBpm;
    t.brightDepthMin = brightDepthMin;
    t.brightDepthMax = brightDepthMax;
    t.brightThetaIncBpm = brightThetaIncBpm;
    t.brightThetaIncMin = brightThetaIncMin;
    t.brightThetaIncMax = brightThetaIncMax;
    t.msMultiplierBpm = msMultiplierBpm;
    t.msMultiplierMin = msMultiplierMin;
    t.msMultiplierMax = msMultiplierMax;
    t.hueIncBpm = hueIncBpm;
    t.hueIncMin = hueIncMin;
    t.hueIncMax = hueIncMax;
    t.sHueBpm = sHueBpm;
    t.sHueMin = sHueMin;
    t.sHueMax = sHueMax;
    return t;
}
WiFiUDP udp;

void udpSetup() {
}


void udpLoop() {
    EVERY_N_MILLIS(100) {
        udp.beginPacketMulticast(IPAddress(192,168,4,255), 4210, WiFi.softAPIP());
        Settings s = readGlobals();
        udp.write((uint8_t *)&s, sizeof(s));
        udp.endPacket();
    }
}