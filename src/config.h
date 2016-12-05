#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <iostream>
#include <string>
#include "inih/iniReader.h"
#include "plog/Log.h"
#include "ueye.h"

using namespace std;

inline const string ToStringImageFormat(int format) {
    switch (format) {
        case IS_IMG_PNG:
            return "IS_IMG_PNG";
        default:
            return "IS_IMG_PNG";
    }
}

inline int ToImageFormat(string format) {
    if (format == "IS_IMG_PNG") {
        return IS_IMG_PNG;
    } else {
        LOGW << "Unknown image format: " << format << ". Using IS_IMG_PNG";
        return IS_IMG_PNG;
    }
}

inline const string ToStringColorMode(int mode) {
    switch (mode) {
        case IS_CM_SENSOR_RAW10:
            return "IS_CM_SENSOR_RAW10";
        case IS_CM_RGB8_PACKED:
            return "IS_CM_RGB8_PACKED";
        default:
            return "IS_CM_SENSOR_RAW10";
    }
}

inline int ToColorMode(string mode) {
    if (mode == "IS_CM_SENSOR_RAW10") {
        return IS_CM_SENSOR_RAW10;
    } else if (mode == "IS_CM_RGB8_PACKED") {
        return IS_CM_RGB8_PACKED;
    } else {
        LOGW << "Unknown color mode: '" << mode << "'. Using IS_CM_SENSOR_RAW10";
        return IS_CM_SENSOR_RAW10;
    }
}

inline const string ToStringLogMode(int mode) {
    switch (mode) {
        case IS_LOG_MODE_AUTO:
            return "IS_LOG_MODE_AUTO";
        default:
            return "IS_LOG_MODE_AUTO";
    }
}

inline int ToLogMode(string mode) {
    if (mode == "IS_LOG_MODE_AUTO") {
        return IS_LOG_MODE_AUTO;
    } else {
        LOGW << "Unknown log mode: " << mode << ". Using IS_LOG_MODE_AUTO";
        return IS_LOG_MODE_AUTO;
    }
}

inline const string ToStringShutterMode(int mode) {
    switch (mode) {
        case IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL:
            return "IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL";
        case IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_ROLLING:
            return "IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_ROLLING";
        default:
            return "IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL";
    }
}

inline int ToShutterMode(string mode) {
    if (mode == "IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL") {
        return IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL;
    } else if (mode == "IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_ROLLING") {
        return IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_ROLLING;
    } else {
        LOGW << "Unknown shutter mode: " << mode
            << ". Using IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL";
        return IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL;
    }
}

class Config
{
    static string cameraNotFoundWarning;

    int         numIDSCameras;
    bool        gigeVision;
    bool        gps;
    bool        tempSensor;
    bool        arduino;
    bool        gpio;
    int         width;
    int         height;
    bool        hwTriggering;
    int         imageFormat;
    double      exposure;
    int         pixelClock;
    int         swTriggerHz;
    int         blackLevelOffset;
    bool        autoParameter;
    bool        gainBoost;
    int         hwGainFactor;
    bool        hwGammaCorrection;
    int         swGammaCorrection;
    bool        hotPixelEnableSensorCorrection;
    int         colorMode;
    int         bitDepth;
    int         logMode;
    int         shutterMode;
    int		    maxConsecutiveFailsBeforeReboot;
    bool        yoctoMaxiIO;
    bool        yoctoMaxiIOExternalOutputVoltage;
    int         redPin;
    int         greenPin;
    int         triggerPin;
    int         triggerPulseMillis;
    int         targetIntervalMillis;


    set<string> sections;
    INIReader   reader;

public:
    Config(INIReader const &reader);
    int getNumIDSCameras();
    bool getGigeVision();
    bool getGps();
    bool getTempSensor();
    bool getArduino();
    bool getGpio();
    int getWidth();
    int getHeight();
    bool getHwTriggering();
    int getImageFormat(string camera);
    double getExposure(string camera);
    int getPixelClock(string camera);
    int getSwTriggerHz(string camera);
    int getBlackLevelOffset(string camera);
    bool getAutoParameter(string camera);
    bool getGainBoost(string camera);
    int getHwGainFactor(string camera);
    bool getHwGammaCorrection(string camera);
    int getSwGammaCorrection(string camera);
    bool getHotPixelEnableSensorCorrection(string camera);
    int getColorMode(string camera);
    int getBitDepth(string camera);
    int getLogMode(string camera);
    int getShutterMode(string camera);
    int getBand(string camera);
    int getMaxConsecutiveFailsBeforeReboot();
    bool getYoctoMaxiIO();
    bool getYoctoMaxiIOExternalOutputVoltage();//Whether to use the external source for the output voltage
    int getRedPin();
    int getGreenPin();
    int getTriggerPin();
    int getTriggerPulseMillis();
    int getTargetIntervalMillis();
    
    set<string> getSections();
};

#endif
