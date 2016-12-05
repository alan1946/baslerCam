#include <iostream>
#include <cstdlib>
#include <pylon/PylonIncludes.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include "baslerCamera.h"

#include "../utils.h"
#include "../plog/Log.h"
#include "../config.h"

// Global static pointer used to ensure a single instance of the class.
	baslerCamera* baslerCamera::m_pInstance = NULL; 


int jCam = 0 ;

// Namespace for using pylon objects.
using namespace Pylon;
//using namespace Basler_GigECamera;
// Namespace for using GenApi objects.
using namespace GenApi;
// Settings to use Basler GigE cameras.
using namespace Basler_GigECameraParams;

namespace ObjectGlobalVars {
   //Put all of your global variables here
   int myvariable = 0;
}

//==============================================================================================
//image event handler.
//The data of a grabbed image is held by a Grab Result Data object. The Grab Result Data object cannot be directly accessed.
//It is always held by a grab result smart pointer, e.g. the basic grab result smart pointer CGrabResultPtr .
//The combination of smart pointer and Grab Result Data object is also referred to as grab result.
//The smart pointer controls the reuse and the lifetime of the Grab Result Data object and the associated image buffer.
//When all smart pointers referencing a Grab Result Data object go out of scope, the grab result's image buffer is reused for grabbing.
//Due to the smart pointer concept, a Grab Result Data object and the associated image buffer can live longer than the camera object 
//used for grabbing the image data. Each Device Specific Instant Camera class has a device specific Grab Result Data object
//and a device specific grab result smart pointer. A device specific grab result smart pointer can be converted to or from
//the basic grab result smart pointer CGrabResultPtr by copying or assigning.
//==============================================================================================
class BaslrImageEventHandler : public CImageEventHandler{
public:
     virtual void OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
    {
        LOGI << "ImageEventHandler";     
        camera.StopGrabbing();
        intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();              
        if(ptrGrabResult->GrabSucceeded()){
        	//LOGD << "camera Number: " << cameraContextValue;
        	LOGD << "Grab Succeeded" << "for camera number: " << cameraContextValue;
        	//LOGD << "Time tick count: " << ptrGrabResult->GetTimeStamp ();   //whe image exposure started.not supported for emulaton
        	//LOGD << "Image Number: " << ptrGrabResult->GetImageNumber();           //always 1
        	//LOGD << "Payload Size: " << ptrGrabResult->GetPayloadSize();           //not correc
        	// LOGD << "BlockId: " << ptrGrabResult->GetBlockID();   //very big number , what is it?
            // Now, the image data can be processed.        
            //cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
            //cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
            const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
            CImagePersistence::Save( ImageFileFormat_Png, "GrabbedImage.png", ptrGrabResult);
            LOGD << "Image saved: ";
        }
        else
        	LOGE <<  "Grab Failed " <<  "for camera number: " << cameraContextValue;
    }
}
;
//http://www.yolinux.com/TUTORIALS/C++Singleton.html

/* This function is called to create an instance of the class.
	    Calling the constructor publicly is not allowed. The constructor
	    is private and is only called by this Instance function.
	*/

	baslerCamera* baslerCamera::Instance()
	{
	   if (!m_pInstance)   // Only allow one instance of class to be generated.
	      m_pInstance = new baslerCamera;
	   return m_pInstance;
	}

//================================================
//create a singleton
//baslerCamera bascam;

baslerCamera::baslerCamera(){   //constructor

   // LOGI << "Using device ";

};


//baslerCamera::~baslerCamera() {
//	Pylon::PylonTerminate(false);
//}


bool  baslerCamera :: init() {
//void baslerCamera :: init(CTlFactory&  tlFactory,DeviceInfoList_t devices){
    PylonInitialize();    //WHERE SHOULD THIS GO? IT's BASLER SPECIFIC, not part of class

//transport layer is used as an abstraction for a physical interface such as IEEE 1394, GigE. 
//For each of these interfaces, there are drivers providing access to camera devices. 
//pylon currently includes four different transport layers:      
        CTlFactory& tlFactory = CTlFactory::GetInstance();

// Get all attached devices and exit application if no device is found.
        DeviceInfoList_t devices;
        tlFactory.EnumerateDevices(devices);
        nmbCams = devices.size();
        LOGD << "nmb camera found: " << nmbCams;
    
        if ( tlFactory.EnumerateDevices(devices) == 0 )
        {
            throw RUNTIME_EXCEPTION( "No camera present.");
        }        
// An Instant Camera provides convenient access to a camera device.
//It allows to grab images with few lines of code providing instant access to grabbed images from a camera device.
//Internally a pylon Device is used. A pylon Device needs to be created and attached to the Instant Camera object for operation. 
//Additional Device Specific Instant Camera classes provide more convenient access to the parameters of the camera. Furthermore, the Instant Camera Array classes eases programming for image grabbing from multiple camera devices.       
// Set instant cameras array for the found devices
        cameras.Initialize(nmbCams);

        for ( size_t i = 0; i < nmbCams; ++i)
        {

            cameras[ i ].Attach( tlFactory.CreateDevice( devices[ i ]));
            cameras[i].Open();            
           // cameras[i].TriggerControlImplementation = TriggerControlImplementation_Standard;   //not found
           // cameras[i].RegisterConfiguration(new CHardwareTriggerConfiguration(CHwCfgType:: Master),RegistrationMode_ReplaceAll ,Cleanup_Delete
            //cameras[ i ].RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete); //software trig -> not supported by emulation
            cameras[i].RegisterConfiguration( new CAcquireSingleFrameConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);   //single frame acq

            //cameras[i].RegisterConfiguration( new CAcquireContinuousConfiguration,RegistrationMode_ReplaceAll, Cleanup_Delete);   //continous mode

            String_t str = cameras[ i ].GetDeviceInfo().GetModelName();
            LOGI << "Using device " << cameras[ i ].GetDeviceInfo().GetModelName();
            

            LOGI << "========================= Camera Device Information =========================";           
            INodeMap& nodemap = cameras[i].GetNodeMap();
            str = CStringPtr( nodemap.GetNode( "DeviceVendorName") )->GetValue();
            

            LOGI << "Vendor name: " << str; 
            str = CStringPtr( nodemap.GetNode( "DeviceModelName") )->GetValue();
            LOGI << "Model      : " << str; 
            str = CStringPtr( nodemap.GetNode( "DeviceManufacturerInfo") )->GetValue();
            LOGI << "Manf info      : " << str;              
            str = CStringPtr( nodemap.GetNode( "DeviceFirmwareVersion") )->GetValue();
            LOGI << "Camera firmware ver:" << str;
            //str = CStringPtr( nodemap.GetNode("IpAddress") )->GetValue();  //not correct, crashes code
            //LOGI << "Camera IP address::" << str;
             // Access the GainRaw integer type node. This node is available for IIDC 1394 and GigE camera devices.
            CIntegerPtr gainRaw( nodemap.GetNode( "GainRaw"));
            int64_t newGainRaw = gainRaw->GetMin() + ((gainRaw->GetMax() - gainRaw->GetMin()) / 2);
            // Make sure the calculated value is valid.
            //newGainRaw = Adjust(newGainRaw, gainRaw->GetMin(), gainRaw->GetMax(), gainRaw->GetInc());
            gainRaw->SetValue(newGainRaw);

            
    
            LOGI << "=============================================================================";          

            
        }
    
// When using the grab loop thread provided by the Instant Camera object, an image event handler processing the grab
// results must be created and registered.
    for ( size_t i = 0; i < cameras.GetSize(); ++i){
        cameras[i].RegisterImageEventHandler( new BaslrImageEventHandler, RegistrationMode_Append, Cleanup_Delete);
    }
       
    if (nmbCams > 0) return true;
    else             return false;
    

} //init

//==========================================================================================================
// start image capture
// Basler supports several modes of image capture
//==========================================================================================================
bool baslerCamera::acquire_images(){
	bool bOk;
    unsigned long startGrabTime = Utils::millisecondsSinceEpoch();  
    unsigned long nowTime,grabIntervalTime;
    CGrabResultPtr ptrGrabResult;
    
#ifdef GrabOne strategy
// Grab one image from each camera sequentially 
// set the TimeoutHandlingException to just return on timeout; not throw a system exception    
// camera.WaitForFrameTriggerReady( 100, TimeoutHandling_ThrowException);   //need this?
// Timing with simulated camera ~150msec/camera => ~600 msec total (Intel 2Ghz, 32 bit)

      for (size_t  i = 0; i < nmbCams; ++i){
          //cameras[ i ].ExecuteSoftwareTrigger();          //software trigger not supported in emulation mode
    	   bOk = cameras[i].GrabOne( 5000, ptrGrabResult, TimeoutHandling_Return);
          if (!bOk){              
               LOGD << "image capture failed after waiting : " << bOk;
          }
       }     
     unsigned long nowTime = Utils::millisecondsSinceEpoch();  
     unsigned long grabIntervalTime = nowTime - startGrabTime;     
     LOGD << "Time to capture all images: " << grabIntervalTime;
     return bOk;

#endif

//#ifdef GRAB_LOOP_THREAD_STRATEGY

//Grab Loop Thread
// The Instant Camera class provides a grab loop thread. The thread runs a grab loop 
// calling RetrieveResult() repeatedly. When using the grab loop thread an image event handler is 
// required to process the grab results.
// Timing with simulated camera ~400 msec total for 5 cameras (Intel 2Ghz, 32 bit)
     for (size_t  i = 0; i < nmbCams; ++i){
    	 cameras[i].StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
     }
     LOGD << " camera capture started ";

     errorFlags[nmbCams] = {true};  //init all camera error flags to true
     bool bLoopOk = true;
     while(bLoopOk){
    	 bool bAnyCamerasStillGrabbing = false;
    	 for (size_t  i = 0; i < nmbCams; ++i){
    		 if (cameras[i].IsGrabbing())   bAnyCamerasStillGrabbing = true;
    		 else errorFlags[i] = false;
    	  }
          nowTime = Utils::millisecondsSinceEpoch(); 
       	  grabIntervalTime = nowTime - startGrabTime;
          if (!bAnyCamerasStillGrabbing)            //true if grabbing complete
        	  bLoopOk = false;                      //exit loop
          else{
              if (grabIntervalTime > WAIT_TIMEOUT){  //too long to grab?
                cameras.StopGrabbing();
                LOGE << "Time to capture all images expired: " << WAIT_TIMEOUT;
                for (size_t  i = 0; i < nmbCams; ++i){                //update missed image counts
                	if (errorFlags[i]) cameraFailureCounters[i]++;
                }
                return false;
              }    
        	  usleep(10*1000); //sleep (msec)        //try again after sleeping
          }
     	 } //while
          LOGD << "Time to capture all images: " << grabIntervalTime;


return true;

//#endif

#ifdef START_GRABBING_STRATEGY        
// Start grabbing for all cameras starting with index 0. The grabbing is started for one camera after the other.
// That's why the images of all cameras are not taken at the same time. However, a hardware trigger will cause all cameras
// to grab images synchronously.According to their default configuration, the cameras are
// set up for free-running continuous acquisition.
// THIS STRATEGY ISN"T IMPLEMENTED CORRECTLY and CRASHES... but it's just a sequential image acquisition mode anyway    
    
    cameras.StartGrabbing();
    //try{

       //Cameras are processed using a round-robin strategy.        
       //cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
       bool bOk = cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_Return); //try TimeoutHandling_Return 
       LOGD << "exiting acquire with results: " << bOk;

     }   
     catch(int e){
    	 cout << "timeout exception";
      //catch (const TimeoutException &e){
        // Auto functions did not finish in time.
        
       // cerr << "A timeout has occurred." << endl
          //   << e.GetDescription() << endl;
        //cerr << "Time out." << endl;
        exitCode = 1;
         
    }  
    return (1);

#endif
    }
             
//==========================================================================================================
// close cameras
//==========================================================================================================
void  baslerCamera::close(){
    PylonTerminate();   

}



