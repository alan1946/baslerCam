// Ceresda
#include "../src/plog/Log.h"
#include "../src/plog/Appenders/ColorConsoleAppender.h"
#include "../src/basler/baslerCamera.h"

// Namespace for using cout.
using namespace std;

static const string CERESDA_VERSION = "0.4.0 alpha";//TODO make version public and maybe move somewhere

 int main(int argc, char* argv[])
{
    string logPath = "debug_log.txt";
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, logPath.c_str()).addAppender(&consoleAppender);
    LOGD << "Log initialized";
    LOGI << "ceresda Basler version:  " << CERESDA_VERSION;

//=================================================================  
//bascam class is a singleton since it manages all cameras as a single class
//#ifdef BASLERCLASS_SINGLETON
    baslerCamera::Instance()->init();
	LOGI << "start grabbing LOOP:  ";
    while (true){
    	//trigger ... either hardware or software....
    	baslerCamera::Instance()->acquire_images();
    	usleep(1000*1000); //sleep (msec)
    }
    baslerCamera::Instance()->close();
	exit(0);
//#endif


#ifdef NOT_SINGLETON_CLASS
    baslerCamera *pGigeCam = new baslerCamera(1);
    pGigeCam->init();
    //if (!pGigeCam->init()){
    //	pGigeCam->close();
    //	exit(0);
    //}

    LOGI << "start grabbing LOOP:  ";
    while (true){
    	pGigeCam->acquire_image();
    	usleep(1000*1000); //sleep (msec)
    }
	pGigeCam->close();
    exit(0);

    //baslerCamera *camGige = new baslerCamera(camera_nmb);
    //IdsCamera *cam = new IdsCamera(1);
#endif




}
