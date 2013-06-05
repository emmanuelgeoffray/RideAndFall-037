#include "testApp.h"

#include "colorLoader.h"
colorGroup globalColors;
easySoundPlayer globalSnd;
magManager tracker;

//	APP_JUST_LAUNCHED,
//	APP_LOAD_SOUNDS,
//	APP_SETUP_RISEFALL,
//	APP_LOAD_CAMERA,
//	APP_RUN_NORMALLY,
//	APP_HOW_TO,

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(false);
	ofSetFrameRate(30);
	ofBackground(0, 0, 0);
	
	ofSetWindowPosition(ofGetScreenWidth()/2 - ofGetWidth()/2, ofGetScreenHeight()/2 - ofGetHeight()/2);
	
	state = APP_JUST_LAUNCHED;
	splash.setup();
	
	//ofDisableArbTex();

	bShow		= false;	
	bSetup		= false;
	bCapture	= false;
	
	skipCount		= 5;
	currentCount	= 0;
	
	//we capture frames @4fps
	//we should drop this down if the app is running to slow however.
	capFPS			= 4.0;
	capWaitTime	    = 1.0/capFPS;
	lastTimeCap     = 0.0;
	showMouseTillTime = 0.0;
	exitAlpha         = 0.0;
	
	state = APP_LOAD_SOUNDS;
	launchState = "Loading sounds";
	
	exitImage.loadImage("assets/global/misc/exit.png");
	exitImage.setAnchorPercent(0.5, 0.5);	
	
	colorLoader::loadColors("assets/global/colors/RiseAndFallMainColors.xml", globalColors.colors);
}

//--------------------------------------------------------------
void testApp::loadSoundFx(){

	string soundDir = "assets/global/sounds/fx/_done/";
	globalSnd.addSound(soundDir+"balloon_pop_nodes_fx72.mp3", "BALLOON_POP", false, false, false);
	globalSnd.addSound(soundDir+"chargeTmp.mp3", "CHARGE", true, false, false);
	globalSnd.addSound(soundDir+"hairy1.mp3", "HAIRY_1", true, false, false);
	globalSnd.addSound(soundDir+"hairy2.mp3", "HAIRY_2", true, false, false);
	globalSnd.addSound(soundDir+"OIL_WATER_COLLISION.mp3", "OIL_WATER_COLLISION", true, false, false);
	globalSnd.addSound(soundDir+"plant_wilt.mp3", "PLANT_WILT", false, false, false);
	globalSnd.addSound(soundDir+"sunmoon_nodes_fx65.mp3", "SUN_MOON", false, false, false);
	globalSnd.addSound(soundDir+"burst_birds.mp3", "BURST_BIRDS", false, false, false);
	globalSnd.addSound(soundDir+"burst_fish.mp3", "BURST_FISH", false, false, false); 	
	
	string newSoundDir		= "assets/global/sounds/fx/todo/multiTrigger";
	ofDirectory dirLoader;
	int numFiles = dirLoader.listDir(newSoundDir);
	
	for(int i = 0; i < numFiles; i++){
		string soundName = dirLoader.getName(i);
		if( soundName.length() > 4 ){
			globalSnd.addSound(dirLoader.getPath(i), soundName.substr(0, soundName.length()-4) , true, false, false); 
			printf("loading %s as %s\n", soundName.substr(0, soundName.length()-4).c_str(), dirLoader.getPath(i).c_str());		
		}	
	}

	newSoundDir				= "assets/global/sounds/fx/todo/singleTrigger";
	dirLoader.reset();
	numFiles = dirLoader.listDir(newSoundDir);

	for(int i = 0; i < numFiles; i++){
		string soundName = dirLoader.getName(i);
		if( soundName.length() > 4 ){
			globalSnd.addSound(dirLoader.getPath(i), soundName.substr(0, soundName.length()-4) , false, false, false); 
			printf("loading %s as %s\n", soundName.substr(0, soundName.length()-4).c_str(), dirLoader.getPath(i).c_str());
		}	
	}

}

//--------------------------------------------------------------
void testApp::updateCapture(){

	if( bCapture ){
	
		float fps = ofGetFrameRate();
		if( fps < 25 ){
			capWaitTime = ofLerp( capWaitTime, ofMap(fps, 8, 25, 1.5, 1.0/capFPS, true), 0.2 );
		}else{
			capWaitTime = ofLerp(capWaitTime, 1.0/capFPS, 0.2);
		}
	
		if( ofGetElapsedTimef() > lastTimeCap+capWaitTime  ){
			grabber.update();
			if( grabber.isFrameNew() ){
				tracker.updatePixels(grabber.getPixels());
				lastTimeCap = ofGetElapsedTimef();
			}
		}		

		currentCount++;
		
		if( currentCount > 30 ){
			tracker.updateTracking();
			mag.smoothIntoMe(tracker.getMagazine(), 0.17);
		}
				
		if( mag.area > 20 * 20 ){
			cam.updateControls(mag);
					
			if( tracker.isBack() >= 1.0 ){
				cam.setHowTo();
			}else if( tracker.isFront() >= 1.0){
				cam.setNormal();
			}		
				
		}
		
	}
	
}

//--------------------------------------------------------------
void testApp::update(){
	globalSnd.update();
	
	if( state >= APP_RUN_NORMALLY ){
		if( cam.percentOver >= 0.98 ){
			showMouseTillTime = ofGetElapsedTimef() + 1.0;
		}
	
		if( ofGetElapsedTimef() < showMouseTillTime ){
			ofShowCursor();
		}else{
			ofHideCursor();
		}
	}
	
	if( ofGetFrameNum() > 5 ){
		
		if( state == APP_LOAD_SOUNDS ){
			loadSoundFx();
			state		= APP_SETUP_RISEFALL;	
			launchState = "Loading Rise Fall";
		}else if( state == APP_SETUP_RISEFALL ){
			splash.setButtonVisible();
			cam.loadResources();
			state = APP_WAITING_LAUNCH;
			launchState = "Waiting for launch";			
		}else if( state == APP_LOAD_CAMERA ){
			if( grabber.initGrabber(320, 240, true) ){
				launchState = "camera launched - loading ferns";			
				tracker.setup("tracking/front/model.bmp", "tracking/back/model.bmp", grabber.width, grabber.height); 
				launchState = "camera launched - ferns loaded";		
				bCapture = true;
				state = APP_RUN_NORMALLY;	
				//we do this to give the window time to go fullscreen
				targetWaitFrame = ofGetFrameNum() + 10; 
				ofSetFullscreen(true);
			}else{
				state = APP_WAITING_LAUNCH;
				launchState = "Load camera failed - check it is not in use by another application!";
				splash.setButtonVisible();
			}
		}else if( state == APP_RUN_NORMALLY ){
			if( !bSetup && ofGetFrameNum() > targetWaitFrame ){
				cam.setup();
				bSetup = true;
			}else{		
				updateCapture();
				cam.update();
			}				
		}

	}

}

//--------------------------------------------------------------
void testApp::draw(){

	if( state >= APP_RUN_NORMALLY ){
		if( bSetup ){
			cam.draw();
		}
		
		if( bShow && bCapture){
			ofPushMatrix();
				//ofSetColor(255, 255, 255, 255);
				//ofTranslate(25, 25, 0);
				//grabber.draw(grabber.width, 0, -grabber.width, grabber.height);
				tracker.debugDraw(25, 25, 320, 240);
//				ofSetColor(255, 255, 255, 255);				
//				mag.debugDraw(0, 0, grabber.width, grabber.height);
			ofPopMatrix();
		}	
	}else{
		splash.draw();	
		ofPushStyle();
			globalColors.setColor("lightRise");
			ofDrawBitmapString("Info: "+launchState, 44, 682);
		ofPopStyle();
	}
	
	//TODO: fix this - we do it here as we want to update the screen before trying the camera
	if( state == APP_LAUNCH_PRESSED ){
		state = APP_LOAD_CAMERA;
	}
	
	exitAlpha = ofLerp(exitAlpha, ofMap( showMouseTillTime-ofGetElapsedTimef(), 0.0, 1.0, 0, 255, true), 0.1);

	if( ofGetElapsedTimef() < showMouseTillTime ){		
		ofSetColor(255, 255, 255, exitAlpha);
		exitImage.draw(25, 25);
	}
	
	if( ofGetFrameRate() <= 15 || bShow ){
		ofPushStyle();
			ofSetColor(33, 33, 33);
			ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 0)+"\ncapFps is "+ofToString(1.0/capWaitTime, 2), 20, 20);
		ofPopStyle();		
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	if( key == 'f' ){
		ofToggleFullscreen();
	}	
	else if(key == 'h'){
		bShow = !bShow;
	}

	if( state == APP_WAITING_LAUNCH ){
		if( key == 'K'){
			state = APP_RUN_NORMALLY;
			targetWaitFrame = ofGetFrameNum() + 1; 
			ofSetFullscreen(true);
			cam.bStartedWithKeys = true;
			ofSetVerticalSync(true);			
		}
	}
	else if( state >= APP_RUN_NORMALLY ){
		cam.keyPressed(key);
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

	if( state >= APP_RUN_NORMALLY ){
		showMouseTillTime = ofGetElapsedTimef() + 6.0;
		cam.mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if( state >= APP_RUN_NORMALLY ){
		cam.mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if( state == APP_WAITING_LAUNCH ){
		splash.checkPress(x, y);
	}
	
	if( state >= APP_RUN_NORMALLY ){
		if( x < 50 && y < 50 && ofGetElapsedTimef() < showMouseTillTime ){
			OF_EXIT_APP(0);
		}else{
			cam.mousePressed(x, y, button);
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if( state == APP_WAITING_LAUNCH ){
		if( splash.checkRelease(x, y) ){
			state = APP_LAUNCH_PRESSED;
			launchState = "Opening camera";
		}	
	}
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

