#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "camApp.h"
#include "ofxFutureUtils.h"
#include "magManager.h"
#include "easySoundPlayer.h"
#include "splashScreen.h"


typedef enum{
	APP_JUST_LAUNCHED,
	APP_LOAD_SOUNDS,
	APP_SETUP_RISEFALL,
	APP_WAITING_LAUNCH,
	APP_LAUNCH_PRESSED,
	APP_LOAD_CAMERA,
	APP_RUN_NORMALLY,
	APP_HOW_TO,
}appState;


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void loadSoundFx();
		void updateCapture();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		magazine mag;

		string launchState;
		appState state;		
		ofImage capImg;
		ofImage exitImage;
		
		float lastTimeCap;
		float capWaitTime;
		float exitAlpha;
		
		splashScreen splash;
		
		int skipCount;
		int currentCount;
		int targetWaitFrame;
		float capFPS;
		
		float showMouseTillTime;
		
		bool bCapture;
		bool bSetup;
		bool bShow;		
		ofVideoGrabber grabber;
		camApp cam;
};

#endif
