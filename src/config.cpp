#include "config.h"
#include "plog/Log.h"
#include "ueye.h"

Config::Config(INIReader const &reader) :
        numIDSCameras(0),
        gigeVision(true),
        gps(true),
        tempSensor(true),
        arduino(false),
        gpio(true),
        width(1280),
        height(1024),
        hwTriggering(true),
        imageFormat(IS_IMG_PNG),
        exposure(0.9),
        pixelClock(40),
        swTriggerHz(1),
        blackLevelOffset(100),
        autoParameter(false),
        gainBoost(false),
        hwGainFactor(100),
        hwGammaCorrection(false),
        swGammaCorrection(100),
        hotPixelEnableSensorCorrection(true),
        colorMode(IS_CM_SENSOR_RAW10),
        bitDepth(16),
        logMode(IS_LOG_MODE_AUTO),
        shutterMode(IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL),
	maxConsecutiveFailsBeforeReboot(11),
        reader(reader)
{
    set<string> sections = reader.GetSections();
    for (string const &section : sections) {
        this->sections.insert(section);
    }
    numIDSCameras = this->reader.GetInteger("global", "numIDSCameras", numIDSCameras);
    gigeVision = this->reader.GetBoolean("global", "gigeVision", gigeVision);
    gps = this->reader.GetBoolean("global", "gps", gps);
    tempSensor = this->reader.GetBoolean("global", "tempSensor", tempSensor);
    arduino = this->reader.GetBoolean("global", "arduino", arduino);
    gpio = this->reader.GetBoolean("global", "gpio", gpio);
    width = this->reader.GetInteger("global", "width", width);
    height = this->reader.GetInteger("global", "height", height);
    hwTriggering = this->reader.GetBoolean("global", "hwTriggering", hwTriggering);
    imageFormat = ToImageFormat(
            this->reader.Get("global", "imageFormat", ToStringImageFormat(imageFormat))
            );
    exposure = this->reader.GetReal("global", "exposure", exposure);
    pixelClock = this->reader.GetInteger("global", "pixelClock", pixelClock);
    swTriggerHz = this->reader.GetInteger("global", "swTriggerHz", swTriggerHz);
    blackLevelOffset = this->reader.GetInteger("global", "blackLevelOffset", blackLevelOffset);
    autoParameter = this->reader.GetBoolean("global", "autoParameter", autoParameter);
    gainBoost = this->reader.GetBoolean("global", "gainBoost", gainBoost);
    hwGainFactor = this->reader.GetInteger("global", "hwGainFactor", hwGainFactor);
    hwGammaCorrection = this->reader.GetBoolean("global", "hwGammaCorrection", hwGammaCorrection);
    swGammaCorrection = this->reader.GetInteger("global", "swGammaCorrection", swGammaCorrection);
    hotPixelEnableSensorCorrection = this->reader.GetBoolean("global", "hotPixelEnableSensorCorrection",
            hotPixelEnableSensorCorrection);
    colorMode = ToColorMode(
            this->reader.Get("global", "colorMode", ToStringColorMode(colorMode))
            );
    bitDepth = this->reader.GetInteger("global", "bitDepth", bitDepth);
    logMode = ToLogMode(
            this->reader.Get("global", "logMode", ToStringLogMode(logMode))
            );
    shutterMode = ToShutterMode(
            this->reader.Get("global", "shutterMode", ToStringShutterMode(shutterMode))
            );
    maxConsecutiveFailsBeforeReboot = this->reader.GetInteger("global","maxConsecutiveFailsBeforeReboot",maxConsecutiveFailsBeforeReboot);
    yoctoMaxiIO = this->reader.GetBoolean("global","yoctoMaxiIO",false);        
    yoctoMaxiIOExternalOutputVoltage = this->reader.GetBoolean("global","yoctoMaxiIOExternalOutputVoltage",false);
    LOGD << "yoctoMaxiIOExternalOutputVoltage " << yoctoMaxiIOExternalOutputVoltage;
    redPin = this->reader.GetInteger("global","redPin",0);
    greenPin = this->reader.GetInteger("global","greenPin",1);
    triggerPin = this->reader.GetInteger("global","triggerPin",2);
    triggerPulseMillis = this->reader.GetInteger("global","triggerPulseMillis",50);
    targetIntervalMillis = this->reader.GetInteger("global","targetIntervalMillis",1000);
}

string Config::cameraNotFoundWarning = "Asking for a camera that does not exist in config: ";

int Config::getNumIDSCameras() {
    int num = reader.GetInteger("global", "numIDSCameras", 0);
    LOGW_IF(num == 0) << "Number of IDS cameras in the config returned 0";
    return num;
}

bool Config::getGigeVision() {
    bool gigeVision = reader.GetBoolean("global", "gigeVision", true);
    return gigeVision;
}

bool Config::getGps() {
    bool gps = reader.GetBoolean("global", "gps", true);
    return gps;
}

bool Config::getTempSensor() {
    bool tempSensor = reader.GetBoolean("global", "tempSensor", true);
    return tempSensor;
}

bool Config::getArduino() {
    bool arduino = reader.GetBoolean("global", "arduino", false);
    return arduino;
}

bool Config::getGpio() {
    bool gpio = reader.GetBoolean("global", "gpio", true);
    return gpio;
}

int Config::getWidth() {
    return width;
}

int Config::getHeight() {
    return height;
}

bool Config::getHwTriggering() {
    return reader.GetBoolean("global", "hwTriggering", hwTriggering);
}

int Config::getMaxConsecutiveFailsBeforeReboot() {
    return maxConsecutiveFailsBeforeReboot;
}

bool Config::getYoctoMaxiIO() {
        return yoctoMaxiIO;
}

bool Config::getYoctoMaxiIOExternalOutputVoltage() {
        return yoctoMaxiIOExternalOutputVoltage;
}


int Config::getRedPin() {
        return redPin;
}

int Config::getGreenPin(){
        return greenPin;
}

int Config::getTriggerPin(){
        return triggerPin;
}

int Config::getTriggerPulseMillis(){
        return triggerPulseMillis;
}

int Config::getTargetIntervalMillis() {
        return targetIntervalMillis;
}


int Config::getImageFormat(string camera) {
    if (sections.find(camera) != sections.end()) {
        ToImageFormat(
            reader.Get(camera, "imageFormat", ToStringImageFormat(imageFormat))
        );
    }
    LOGW << cameraNotFoundWarning << camera;
    return imageFormat;
}

double Config::getExposure(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetReal(camera, "exposure", exposure);
    }
    LOGW << cameraNotFoundWarning << camera;
    return exposure;
}

int Config::getPixelClock(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "pixelClock", pixelClock);
    }
    LOGW << cameraNotFoundWarning << camera;
    return pixelClock;
}

int Config::getSwTriggerHz(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "swTriggerHz", swTriggerHz);
    }
    LOGW << cameraNotFoundWarning << camera;
    return swTriggerHz;
}

int Config::getBlackLevelOffset(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "blackLevelOffset", blackLevelOffset);
    }
    LOGW << cameraNotFoundWarning << camera;
    return blackLevelOffset;
}

bool Config::getAutoParameter(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetBoolean(camera, "autoParameter", autoParameter);
    }
    LOGW << cameraNotFoundWarning << camera;
    return autoParameter;
}

bool Config::getGainBoost(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetBoolean(camera, "gainBoost", gainBoost);
    }
    LOGW << cameraNotFoundWarning << camera;
    return gainBoost;
}

int Config::getHwGainFactor(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "hwGainFactor", hwGainFactor);
    }
    LOGW << cameraNotFoundWarning << camera;
    return hwGainFactor;
}

bool Config::getHwGammaCorrection(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetBoolean(camera, "hwGammaCorrection", hwGammaCorrection);
    }
    LOGW << cameraNotFoundWarning << camera;
    return hwGammaCorrection;
}

int Config::getSwGammaCorrection(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "swGammaCorrection", swGammaCorrection);
    }
    LOGW << cameraNotFoundWarning << camera;
    return swGammaCorrection;
}

bool Config::getHotPixelEnableSensorCorrection(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetBoolean(camera, "hotPixelEnableSensorCorrection",
                hotPixelEnableSensorCorrection);
    }
    LOGW << cameraNotFoundWarning << camera;
    return hotPixelEnableSensorCorrection;
}

int Config::getColorMode(string camera) {
    if (sections.find(camera) != sections.end()) {
        return ToColorMode(
            reader.Get(camera, "colorMode", ToStringColorMode(colorMode))
        );
    }
    LOGW << cameraNotFoundWarning << camera;
    return colorMode;
}

int Config::getBitDepth(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "bitDepth", bitDepth);
    }
    LOGW << cameraNotFoundWarning << camera;
    return bitDepth;
}

int Config::getLogMode(string camera) {
    if (sections.find(camera) != sections.end()) {
        return ToLogMode(
            reader.Get(camera, "logMode", ToStringLogMode(logMode))
        );
    }
    LOGW << cameraNotFoundWarning << camera;
    return logMode;
}

int Config::getShutterMode(string camera) {
    if (sections.find(camera) != sections.end()) {
        return ToShutterMode(
            reader.Get(camera, "shutterMode", ToStringShutterMode(shutterMode))
        );
    }
    LOGW << cameraNotFoundWarning << camera;
    return shutterMode;
}

int Config::getBand(string camera) {
    if (sections.find(camera) != sections.end()) {
        return reader.GetInteger(camera, "band", -1);
    }
    LOGW << cameraNotFoundWarning << camera;
    return -1;
}

set<string> Config::getSections() {
    return sections;
}
