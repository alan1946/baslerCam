#ifndef __BASLERCAMERA_H__
#define __BASLERCAMERA_H__

#include <pylon/PylonIncludes.h>

//#include "../config.h"
//#include "../gps/gps.h"   //asb: why is gps i camera module?

//#include <pylon/gige/BaslerGigECamera.h>
//typedef Pylon::CBaslerGigECamera BaslerCamera_t;


//TODO make this configurable?
#define WAIT_TIMEOUT 2500

using namespace std;
// Namespace for using pylon objects.
using namespace Pylon;

/*BaslerCamera class
Manages all Basler gige camera:
TODO: inherit from camera base class that works for IDS,FLIR,....
  - inits all cameras
  - acquires and stores images for all cameras
  - closes all cameras
*/


/* 
 c_maxCamerasToUse: Limits the number of cameras used for grabbing.
 It is important to manage the available bandwidth when grabbing with multiple cameras.
 This applies, for instance, if two GigE cameras are connected to the same network adapter via a switch.
 To manage the bandwidth, the GevSCPD interpacket delay parameter and the GevSCFTD transmission delay
 parameter can be set for each GigE camera device.
 The "Controlling Packet Transmission Timing with the Interpacket and Frame Transmission Delays on Basler GigE Vision Cameras"
 Application Notes (AW000649xx000)
 provide more information about this topic.
 The bandwidth used by a FireWire camera device can be limited by adjusting the packet size.
*/
static const size_t c_maxCamerasToUse = 10;

//Singleton definition reference:
//http://www.yolinux.com/TUTORIALS/C++Singleton.html

// NOTE: NOT SURE THERE'S ANY ADAVANTAGE for  A SNGLETON CLASS HERE


class baslerCamera {
    
public:
   static baslerCamera* Instance(); //create as singleton
//--------------------------------------------------------------------------------------------------------------  
  baslerCamera();   //constructor;
  //change to be similar to idscamera:  IdsCamera(int cameraID, Config *config, string logPath);	
//----------------------------------------------------------------------------------------
//close()
// - closes all cameras
// - terminates transport if needed
//----------------------------------------------------------------------------------------
void  close();
//----------------------------------------------------------------------------------------
//init()
// - initialize camera system
// - find all cameras and create camera objects
// - program camera configuration parameters
// - return true if all cameras present,configured and in operational state
//----------------------------------------------------------------------------------------    
bool init();
//----------------------------------------------------------------------------------------
//acquire_images()
// - acquire 1 image from each camera either sequentially or in parallel
// - returns either after all images acquired or timeout
// - returns true if all images successfully acquired
// - returns false if not all images acquired. If false then:
//   - cameraFailureCounters incremented for camera not acquired
//   - errorFlag for camera not acquired set to true;
//----------------------------------------------------------------------------------------        
 bool acquire_images();

//public parameters
 CInstantCameraArray cameras;   //new api
 unsigned int cameraFailureCounters[c_maxCamerasToUse] = {0};
 bool errorFlags[c_maxCamerasToUse] = {true};
 int nmbCams;

 //-------------------------------------------------------------------------------------------------------------
private:
    static baslerCamera* m_pInstance;  //for singleton




};
//extern baslerCamera bascam;
#endif
