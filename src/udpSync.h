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
    long entropy;
    uint8_t mirror;
} Settings;

// make up a settings packet
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
    t.mirror = mirror;
    return t;
}

// set all the globals from a settings packet
void setGlobals(const Settings * s) {
    power = s->power;
    autoplay = s->autoplay;
    autoplayDuration = s->autoplayDuration;
    solidColor = s->solidColor;
    currentPatternIndex = s->currentPatternIndex;
    currentPaletteIndex = s->currentPaletteIndex;
    brightness = s->brightness;
    saturationBpm = s->saturationBpm;
    saturationMin = s->saturationMin;
    saturationMax = s->saturationMax;
    brightDepthBpm = s->brightDepthBpm;
    brightDepthMin = s->brightDepthMin;
    brightDepthMax = s->brightDepthMax;
    brightThetaIncBpm = s->brightThetaIncBpm;
    brightThetaIncMin = s->brightThetaIncMin;
    brightThetaIncMax = s->brightThetaIncMax;
    msMultiplierBpm = s->msMultiplierBpm;
    msMultiplierMin = s->msMultiplierMin;
    msMultiplierMax = s->msMultiplierMax;
    hueIncBpm = s->hueIncBpm;
    hueIncMin = s->hueIncMin;
    hueIncMax = s->hueIncMax;
    sHueBpm = s->sHueBpm;
    sHueMin = s->sHueMin;
    sHueMax = s->sHueMax;
    mirror = s->mirror;
}

WiFiUDP udp;

#define UDP_PORT 4210

void udpSetup() {
    #ifdef RECEIVER
    udp.begin(UDP_PORT);
    #endif
}

  // Add entropy to random number generator; we use a lot of it.

void forceUdpSync() {
    #ifndef RECEIVER
    udp.beginPacketMulticast(IPAddress(192,168,4,255), UDP_PORT, WiFi.softAPIP());
    Settings s = readGlobals();
    s.entropy = random(65535);
    random16_add_entropy(s.entropy);
    udp.write((uint8_t *)&s, sizeof(s));
    udp.endPacket();
    Serial.printf("power=%d, autoplay=%d, autoplayDuration=%d, currentPatternIndex=%d, currentPaletteIndex=%d, brightness=%d\n", s.power, s.autoplay, s.autoplayDuration, s.currentPatternIndex, s.currentPaletteIndex, s.brightness);
    #endif
}
void udpLoop() {
    #ifdef RECEIVER
    if (WiFi.isConnected()) {
        int bytesAvailable = udp.parsePacket();
        if (bytesAvailable) {
            Settings s;
            // read into s
            udp.read((char *)&s, bytesAvailable);
            random16_add_entropy(s.entropy);
            setGlobals(&s);
            FastLED.setBrightness(s.brightness);
            // Serial.printf("power=%d, autoplay=%d, autoplayDuration=%d, currentPatternIndex=%d, currentPaletteIndex=%d, brightness=%d\n", s.power, s.autoplay, s.autoplayDuration, s.currentPatternIndex, s.currentPaletteIndex, s.brightness);
        }
    }
    #else
    EVERY_N_MILLIS(100) {
        forceUdpSync();
    }
    #endif
}