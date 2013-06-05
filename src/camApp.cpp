/*
 *  camApp.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 26/12/2009.
 *
 * Copyright Theodore Watson 2010
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include "camApp.h"

extern colorGroup globalColors;
typeLoader globalType;
imageSequenceLoader globalSeqLoader;
backgroundSoundManager soundMan;
ofEvent <eventStruct> globalEventStruct;
myPersonElement myPerson;
float riseFallPct = 1.0;
float durationSameDirection = 0.0;
bool bRise = true;

bool bLoadFullTextures	= false;
bool bLoadRegTextures	= false;

ofPoint camPos;
ofPoint personScreenPos;
bool bSlowDown = false; 
imageElement endGraphic; 
bool endCreatedInRise = false;

//--------------------------------------------------------------
camApp::camApp(){
	resourcesLoaded = false;
	bStartedWithKeys = false;
}

//--------------------------------------------------------------
camApp::~camApp(){
	for(int i = 0; i < nodes.size(); i++){
		if( nodes[i] != NULL ){
			printf("deleting node %s\n", nodes[i]->nodeName.c_str());
			delete (nodes[i]);
			nodes[i] = NULL;
		}
	}
	nodes.clear();
}

//--------------------------------------------------------------
void camApp::loadResources(){

	background.loadImage("assets/global/rise/backgrounds/background.jpg");
	background.setAnchorPercent(0.5, 0.5);
	
	background2.loadImage("assets/global/fall/backgrounds/background.jpg");
	background2.setAnchorPercent(0.5, 0.5);
	
	backgroundEnd.loadImage("assets/global/rise/backgrounds/backgroundStar.jpg");
	backgroundEnd.setAnchorPercent(0.5, 0.5);
	
	backgroundEnd2.loadImage("assets/global/fall/backgrounds/backgroundStar.jpg");
	backgroundEnd2.setAnchorPercent(0.5, 0.5);	
	
	//TODO: switch out DINPro
	globalType.loadFont("assets/global/type/Vera.ttf", 14, "DIN_REG_S");
	globalType.loadFont("assets/global/type/Vera.ttf", 18, "DIN_REG_M");
	globalType.loadFont("assets/global/type/Vera.ttf", 24, "DIN_REG_L");
	globalType.loadFont("assets/global/type/Vera.ttf", 32, "DIN_REG_XL");

	//TODO: check texture loading limit issue?
	globalSeqLoader.loadSequence("assets/nodes/node10-time/rise/sunRiseSeq/", "png", "TIME_SEQ_RISE");	
	globalSeqLoader.loadSequence("assets/nodes/node10-time/fall/moonSeqB/", "png", "TIME_SEQ_FALL");
	globalSeqLoader.loadSequence("assets/nodes/node04-scaleField/both/handSeq/hand01", "png", "SCALE_SEQ");
	globalSeqLoader.loadSequence("assets/nodes/node05-plant/both/growthSeq3/", "png", "PLANT_SEQ");
	globalSeqLoader.loadSequence("assets/nodes/node16-beast/both/blinkSeq/", "png", "BEAST_EYE_SEQ");
	globalSeqLoader.loadSequence("assets/nodes/node12-handPin/rise/handPinSeq/", "png", "PIN_SEQ");
	globalSeqLoader.loadSequence("assets/nodes/node12-handPin/fall/handBlowSeq/", "png", "FAN_SEQ");

	endGraphic.loadImage("assets/nodes/node15-end/end.png");
	endGraphic.img.setAnchorPercent(0.5, 0.5);
	endGraphic.pos.set(0, 9000000, 0);
	
	soundMan.setup("assets/global/sounds/");

	resourcesLoaded = true;
}

//--------------------------------------------------------------
void camApp::setup(){
	
	if( !resourcesLoaded ){
		loadResources();
	}

	world.setOrigin(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0.0));
	fov = 60.0;
				
	soundMan.play();
	
	camMan.setPosZ(0.0);		
	camMan.setPosY(100.0);	
	camMan.setRotLimitMax(0,  24,  20000);
	camMan.setRotLimitMin(0, -24, -20000);
	
	durationSameDirection = 0.0;
	
	bRise				= true;
	dragAmnt			= 1.0;
	updateSpeed			= 1.0;
	riseFallPct			= 0.0;
	camShiftAmntY		= 0.0;
	nextTime			= ofRandom(20, 45);
	bRestoreToIntro		= false;
	percentOver			= 0.0;
	nodeCreatedTime		= 0.0;

	title.setup(ofPoint());
	hand.setup(ofPoint());
	weather.setup(ofPoint());
	howTo.setup(ofPoint());

	nodeOrder.push_back("sunMoonNode");	
	nodeOrder.push_back("fishBirdNode");
	nodeOrder.push_back("plantNode");
	nodeOrder.push_back("scaleFieldNode");
	nodeOrder.push_back("oilWaterNode");
	nodeOrder.push_back("chargeNode");
	nodeOrder.push_back("hairyNode");	
	nodeOrder.push_back("timeNode");		
	nodeOrder.push_back("deepBirdFishNode");	
	nodeOrder.push_back("endNode");
	
	myPerson.setup( ofPoint(0, 0, 10), ofPoint(), ofPoint(), 0.94, 10.0);	
	myPerson.img.setAnchorPercent(0.5, 0.0);
	
	camMan.setPos( ofPoint(0, 0, 0) );
	myGravity.set(0, 0.0, 0);

	state = CAM_APP_TITLE;

	if( state == CAM_APP_TITLE ){
		title.setState(NODE_ACTIVE);
	}
	
}

//-------------------------------------------------------------
void camApp::limitPersonMovement( ofPoint relativeTo, float minX, float maxX, float minY, float maxY ){
	
	float velAmnt = 0.35;
	float graAmnt = 0.3;
	
	float distTol = 30.0;
	float pctSlowDown = 0.85;
	
	
	ofPoint relDist = myPerson.pos - relativeTo;
	

	if( relDist.y < ( minY + distTol ) ){
		float pct = ofMap( relDist.y, minY+distTol, minY, 0, 1, true);

		if( myPerson.vel.y < 0 ){
			myPerson.vel.y *= pctSlowDown + (1.0-pctSlowDown)*(1.0-pct);
		}
		
		if( myGravity.y < 0){
			myGravity.y *= ( 1.0-pct );				
		}
	}

	if( relDist.y > ( maxY - distTol) ){
		float pct = ofMap( relDist.y, maxY-distTol, maxY, 0, 1, true);
	
		if( myPerson.vel.y > 0 ){
			myPerson.vel.y *= pctSlowDown + (1.0-pctSlowDown)*(1.0-pct);
		}
		
		if( myGravity.y > 0){
			myGravity.y *= ( 1.0-pct );				
		}		
	}	
	
	if( relDist.x < ( minX + distTol ) ){
		float pct = ofMap( relDist.x, minX+distTol, minX, 0, 1, true);

		if( myPerson.vel.x < 0 ){
			myPerson.vel.x *= pctSlowDown + (1.0-pctSlowDown)*(1.0-pct);
		}
		
		if( myGravity.x < 0){
			myGravity.x *= ( 1.0-pct );				
		}
	}

	if( relDist.x > ( maxX - distTol) ){
		float pct = ofMap( relDist.x, maxX-distTol, maxX, 0, 1, true);
	
		if( myPerson.vel.x > 0 ){
			myPerson.vel.x *= pctSlowDown + (1.0-pctSlowDown)*(1.0-pct);
		}
		
		if( myGravity.x > 0){
			myGravity.x *= ( 1.0-pct );				
		}		
	}
	
}


//--------------------------------------------------------------
void camApp::handleRiseFall(){

	durationSameDirection += ofGetLastFrameTime();
	
	myPerson.setRotation(0, 0, 12.0 * ofSignedNoise(0.0, ofGetElapsedTimef() * 0.15) );
	myPerson.update(myGravity * 0.65, dragAmnt, updateSpeed );

	if( updateSpeed >= 0.9 ){

		riseFallPct  = ofMap( myPerson.vel.y,  -1.0, 1.0, 0.0, 1.0, true);
			
		if( riseFallPct > 0.55 ){
			if( bRise != true ){
				durationSameDirection = 0.0;
			}
			bRise = true;
		}
		if( riseFallPct < 0.45 ){
		
			if( bRise != false ){
				durationSameDirection = 0.0;
			}
			bRise = false;
		}
		
	}

	if( state == CAM_APP_NORMAL && percentOver <= 0.0 && durationSameDirection >= nextTime/2 && ofGetElapsedTimef() - hand.lastTimeBurst > nextTime ){
		
		if( bSlowDown ){
			nextTime += ofRandom(4, 15);
		}else{
			if( bRise && updateSpeed > 0.9 && myPerson.vel.y > 2.0 ){
				if( hand.doPinPrick() ){				
					nextTime = ofRandom(30, 70);
					durationSameDirection = 0.0;
				}
			}

			if( !bRise && updateSpeed > 0.9 && myPerson.vel.y < -2.0 ){
				if( hand.doFanBlow() ){
					nextTime = ofRandom(30, 70);
					durationSameDirection = 0.0;
				}
			}
		}
		
	}

	if( riseFallPct >= 0.6 ){
		myPerson.which = 0;
	}

	if( riseFallPct <= 0.4 ){
		myPerson.which = 1;
	}
		
}

//-------------------------------------------------------------
void camApp::titleUpdate(){
	title.update();
	if( title.holdPerson() ){
		limitPersonMovement( title.pos,  -400, 400, -ofGetHeight()/3, ofGetHeight()/3 );
	}
}

//-------------------------------------------------------------
void camApp::riseFallUpdate(){
	
	bSlowDown = false;
	
	for(int i = 0; i < nodes.size(); i++){
		nodes[i]->checkState(myPerson.pos.x, myPerson.pos.y, myPerson.pos.z);
		
		if( nodes[i]->shouldPersonWait() ){
			bSlowDown = true;
		}
	}
	
	//TODO: make this escapable by allowing people to move out of the node!
	if( percentOver >= 1.0 ){
		bSlowDown = true;
	}
	
	if( bSlowDown ){
		dragAmnt = ofLerp(dragAmnt, 0.75, 0.2);
		updateSpeed = ofLerp(updateSpeed, 0.06, 0.25);
	}else{
		dragAmnt	= ofLerp(dragAmnt, 1.0, 0.2);
		updateSpeed = ofLerp(updateSpeed, 1.0, 0.05);
	}


}
//--------------------------------------------------------------
void camApp::updateControls(magazine magIn){
		
		mag = magIn;
		
		if( mag.angleZ > 280 ){
			myGravity.y = ofMap( mag.angleZ, 280, 360, 0, 0.8, true);
			myGravity.x = ofMap( mag.angleZ, 360, 280, 0, -0.6, true);
		}else
		if( mag.angleZ < 80 ){
			myGravity.y = ofMap( mag.angleZ, 80, 0, 0, 0.8, true);
			myGravity.x = ofMap( mag.angleZ, 80, 0, 0.6, 0, true);				
		}
		else
		if( mag.angleZ > 110 && mag.angleZ < 180){
			myGravity.y = ofMap( mag.angleZ, 110, 180, 0, -0.8, true);
			myGravity.x = ofMap( mag.angleZ, 180, 110, 0, 0.6, true);
		}
		else
		if( mag.angleZ < 260  && mag.angleZ > 180 ){
			myGravity.y = ofMap( mag.angleZ, 260, 180, 0, -0.8, true);
			myGravity.x = ofMap( mag.angleZ, 180, 260, 0, -0.6, true);
		}else{
			myGravity.set(0, 0, 0);
		}
		
		if( state == CAM_APP_NORMAL ){
			camMan.setTargetRotY(ofMap(mag.center.x, mag.trackAreaW * 0.35, mag.trackAreaW * 0.65, -20, 20, true));
		}else{
			camMan.setTargetRotY(0);
		}
		
		if( state == CAM_APP_TITLE ){
			title.updateAngle(mag.angleZ);
		}

}

//-----------------------------
bool killNodesAndDelete( baseNode * A ){
	if( A == NULL) return true;
	
	if( A->isKillable() ){
		if( fabs(A->pos.y-myPerson.pos.y) > ofGetHeight() * 5 ){
			printf("killing node - %s\n", A->nodeName.c_str());					
			delete A;
			A = NULL;
			return true;
		} 
	}
		
	return false;
}

//-----------------------------
void camApp::nextNode(bool makeAbove){

	if( nodeOrder.size() == 0){
		return;
	}
	
	string nodeStr = nodeOrder.front();
	nodeOrder.erase(nodeOrder.begin(), nodeOrder.begin()+1);

	ofPoint dest;
	
	if( makeAbove )dest.set( world.camPos.x, world.camPos.y + (float)ofGetHeight()*1.0, 0 );
	else dest.set( world.camPos.x, world.camPos.y - (float)ofGetHeight()*1.0, 0 );
	
	if( nodes.size() ){
		if( makeAbove ){
			dest += ofPoint(0, nodes.back()->maxBoundsY, 0);
		}else{
			dest += ofPoint(0, nodes.back()->minBoundsY, 0);
		}
	}
	
	if( nodeStr == "endNode"){
		endNode * p = new endNode();
				
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY + ofGetHeight(), 0);
						
		}else{
			dest -= ofPoint(0, p->maxBoundsY+ ofGetHeight(), 0);
		}
		
		p->setup(dest);
		nodes.push_back(p);
		endCreatedInRise = bRise;		
	}
		
	if( nodeStr == "sunMoonNode"){
		sunMoonNode * p = new sunMoonNode();
			
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
		
		p->setup(dest);
		nodes.push_back(p);
	}
	
	if( nodeStr == "fishBirdNode" ){
		fishBirdNode * p = new fishBirdNode();
		
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}
				
		p->setup(dest);
		nodes.push_back(p);
	}

	//this one is complicated as we don't really want to discover it in fall.
	if( nodeStr == "plantNode" ){
		if( makeAbove || nodeOrder.size() == 1 ){
			plantNode * p = new plantNode();
				
			if( makeAbove ){
				dest += ofPoint(0, -p->minBoundsY, 0);
			}else{
				dest -= ofPoint(0, p->maxBoundsY, 0);
			}			
			
			if( makeAbove )p->setup(dest);
			else p->setup(dest + ofPoint(0, -ofGetHeight()/2, 0) );
			
			nodes.push_back(p);
		}else{
			if( nodeOrder.size() > 1 ){
				nodeOrder.insert( nodeOrder.begin()+1, "plantNode" );
			}else{
				nodeOrder.insert( nodeOrder.begin(), "plantNode" );
			}
		}
	}
	
	if( nodeStr =="timeNode" ){
		timeNode * p = new timeNode();
	
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
					
		p->setup(dest);
		nodes.push_back(p);
	}

	if( nodeStr == "oilWaterNode" ){
		oilWaterNode * p = new oilWaterNode();
		
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
					
		p->setup(dest, 250);
		nodes.push_back(p);
	}

	if( nodeStr == "chargeNode" ){
		chargeNode * p = new chargeNode();
		
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
				
		p->setup(dest);
		nodes.push_back(p);
	}

	if( nodeStr == "scaleFieldNode" ){
		scaleFieldNode * p = new scaleFieldNode();
		
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
					
		p->setup(dest);
		nodes.push_back(p);
	}
	
	if( nodeStr == "deepBirdFishNode" ){
		deepBirdFishNode * p = new deepBirdFishNode();
		
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
					
		p->setup(dest);
		nodes.push_back(p);	
	}

	if( nodeStr == "hairyNode" ){
		hairyNode * p = new hairyNode();
		
		if( makeAbove ){
			dest += ofPoint(0, -p->minBoundsY, 0);
		}else{
			dest -= ofPoint(0, p->maxBoundsY, 0);
		}	
					
		p->setup(dest);
		nodes.push_back(p);
	}
	
}

//-------------------------------------------------------
void camApp::setHowTo(){
	if( state != CAM_APP_HOWTO ){
		if( state == CAM_APP_TITLE ){
			bRestoreToIntro = true;
		}else{
			bRestoreToIntro = false;
		}
		
		magRotZStartHowTo = mag.angleZ;
		
		howToStorePos = camPos;
		howTo.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
		howTo.state = NODE_ACTIVATING;
		state = CAM_APP_HOWTO;
	}
}

//-------------------------------------------------------
void camApp::setNormal(){
	if( state == CAM_APP_HOWTO || howTo.state != NODE_INACTIVE ){
		if( howTo.getState() == NODE_ACTIVE || howTo.getState() == NODE_ACTIVATING ){
			howTo.setState(NODE_DEACTIVATING);
			howTo.resetMovie();
		}
		if( bRestoreToIntro ){
			state = CAM_APP_TITLE;
		}else{
			state = CAM_APP_NORMAL;
		}
	}
}

//-------------------------------------------------------
void camApp::updateAppInfo(){
	
	appInfo info;
	
	info.appFps				= ofGetFrameRate();
	info.worldGravity		= myGravity;
	info.personVel			= myPerson.vel;
	
	info.trackingPos		= mag.center;
	info.magRot.set(mag.angleX, mag.angleY, mag.angleZ);
		
	info.numNodesActive		= nodes.size();
	info.numNodesToDiscover	= nodeOrder.size();
	
	info.numObjectsDrawn	= sceneGraph.numDrawn;
	info.numObjectsNotDrawn	= sceneGraph.numSkipped;

	howTo.updateAppInfo(info);
}

//-------------------------------------------------------
bool camApp::makeNewNode(float distFromNodeToMakeNew ){
	
	bool bMakeNewNode = false;
	bool searchAbove  = false;
	
	if( myPerson.vel.y > 0 && myGravity.y > 0 ){
		searchAbove = true;
	}else if( myPerson.vel.y < 0 && myGravity.y < 0 ){
		searchAbove = false;
	}else{
		//we only want to make a new node if someone is heading in a direction and not changing direction
		return false;
	}
		
	bool noNodesAheadOfUs = true;
	
	float nodeDist = 9999999.0;
	for(int i = 0; i < nodes.size(); i++){
		if( fabs(nodes[i]->pos.y - myPerson.pos.y) < nodeDist ){
			nodeDist = fabs(nodes[i]->pos.y - myPerson.pos.y);
		}
		
		if( searchAbove ){
			if( nodes[i]->pos.y >= myPerson.pos.y ){
				noNodesAheadOfUs = false;
			}
		}else{
			if( nodes[i]->pos.y <= myPerson.pos.y ){
				noNodesAheadOfUs = false;
			}
		}
	}

	//we don't want to create nodes if there are ones both above and below us. 
	if( !noNodesAheadOfUs )return false;
	
	//finally we check if the dist from the closest node is further than the threshold to make a new node
	if( nodeDist > distFromNodeToMakeNew ){
		return true;
	}	
		
}

//--------------------------------------------------------------
void camApp::update(){

	updateAppInfo();

	if( title.state == NODE_ACTIVE && title.introAllDone() ){
		title.setState(NODE_DEACTIVATING);
		state = CAM_APP_NORMAL;
		durationSameDirection = 0.0;
	}
	
	if( title.state != NODE_INACTIVE ){
		titleUpdate();
	}

	if( howTo.state != NODE_INACTIVE ){
		if( howTo.timeActive < 1.0 ){
			magRotZStartHowTo = mag.angleZ;
		}
		
		howTo.update();
		if( fabs( ofAngleDifferenceDegrees(magRotZStartHowTo, mag.angleZ) ) > 120 ){
			howTo.playMovie();
		}
		
	}
		
	if( state == CAM_APP_TITLE && title.holdPerson() ){
		//camMan.setTargetPosX(currentNode->pos.x); 
		camMan.setTargetPosY(title.pos.y);
	}else{

		riseFallUpdate();	
		
		camShiftAmntY = ofLerp(camShiftAmntY, ofMap(riseFallPct, 0, 1, -(float)ofGetHeight() * 0.175, (float)ofGetHeight() * 0.175, true), 0.03);
		
		if( state == CAM_APP_NORMAL ){

			float endGraphicOffset = 210.0;
			percentOver = ofMap( fabs(myPerson.pos.y - (endGraphic.pos.y+endGraphicOffset) ), 1000, endGraphicOffset*0.5, 0.0, 1.0, true);
			
			if( percentOver >= 0.7 ){
				
				float pct	= ofMap(percentOver, 0.7, 1.0, 0.0, 1.0, true);
				float ipct	= 1.0-pct;
				
				myPerson.vel *= 0.7 + 0.3*ipct;
				myPerson.pos.x = ofLerp(myPerson.pos.x, endGraphic.pos.x, 0.01 + pct * 0.02);
				myPerson.pos.y = ofLerp(myPerson.pos.y, endGraphic.pos.y+endGraphicOffset, 0.01 + pct * 0.02);
				
				camMan.setTargetPosY(endGraphic.pos.y * pct + (myPerson.pos.y+camShiftAmntY) * ipct);			
			}else{
				camMan.setTargetPosY(myPerson.pos.y+camShiftAmntY);
			}
						
		}
		
		if( howTo.pct > 0.2 ){
			camMan.setTargetPosY(howToStorePos.y);
		}
			
		if( makeNewNode( ofGetHeight() ) ){

			if( state == CAM_APP_NORMAL && howTo.state == NODE_INACTIVE && ofGetElapsedTimef() - nodeCreatedTime > 1.2 ){

				if( myPerson.vel.y > 0.2 ){
					nextNode(true);
				}else if( myPerson.vel.y < -0.2 ){
					nextNode(false);		
				}
				
				nodeCreatedTime = ofGetElapsedTimef();
			}
		}
		
		if( state == CAM_APP_NORMAL && nodes.size() > 0 ){
			nodes.erase(nodes.begin(), partition(nodes.begin(), nodes.end(), killNodesAndDelete)); 		
		}

	}

	if( state == CAM_APP_HOWTO ){
		limitPersonMovement(howToStorePos, -ofGetWidth()/2, ofGetWidth()/2, -(float)ofGetHeight()*0.4, (float)ofGetHeight()*0.4);
	}	

	soundMan.update(riseFallPct);
	handleRiseFall();	
	
	camMan.update(0.1, 0.04);
	world.camPos = camMan.getPosition();
	world.camRot = camMan.getRotation();
	
	camPos = world.camPos;

	if( state != CAM_APP_TITLE && !( state == CAM_APP_HOWTO && howTo.state == NODE_ACTIVE ) ){
		for(int i = 0; i < nodes.size(); i++){
			nodes[i]->update();
		}
	}
	
	sceneGraph.clearElements();
	sceneGraph.addElement(&myPerson);
	sceneGraph.addElement(&weather);
	
	if( title.state != NODE_INACTIVE ){
		sceneGraph.addElement(&title);
	}
	
	for(int i = 0; i < nodes.size(); i++){
		sceneGraph.addElement(nodes[i]);
	}
	
	//MANUAL NODEs
	weather.update();
	hand.update();
}

//--------------------------------------------------------------
void camApp::draw(){

	//hitTest();
	ofSetupScreen();
	
	float width  = ofGetWidth();
	float height = ofGetHeight();

	//TODO: check why the background was fading weird on rise background - OpenGL related bug?

	ofEnableAlphaBlending();	
	ofSetColor(255, 255, 255, 255);
	background.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight() );	

	ofSetColor(255, 255, 255, ofMap(riseFallPct, 0.49, 0.0, 0, 255, true));	
	background2.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());

	world.setScale(ofGetWidth(), ofGetHeight(), ofGetWidth());
	world.setOrigin(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0.0));
	world.begin();
				
		world.ourCameraSetup(width/2, height/2, 0.0, width, height, fov );	

		//world.drawDebugGrid();
		glDisable(GL_DEPTH_TEST);			

		sceneGraph.updateCameraPos(world.camPos + ofPoint(0, 0, world.camDist));
		sceneGraph.sortZ();
		
		ofRectangle clipRect;
		
		clipRect.width	= 2.0 * ofGetWidth();
		clipRect.height	= 2.2 * ofGetHeight(); // lets give us some extra room!
		clipRect.x		= world.camPos.x - clipRect.width/2;
		clipRect.y      = world.camPos.y - clipRect.height/2;
		
		//TODO: MAKE PLANT CLIPPING BETTER
		sceneGraph.renderWithClipping(clipRect, -180+world.camPos.z, 100 + world.camPos.z,  world.camDist, fov );	
				
		hand.updateCamToScreenCoords();
		
		GLdouble model_view[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

		GLdouble projection[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);			
												
		GLdouble X, Y, Z = 0;	
		gluProject(myPerson.pos.x, myPerson.pos.y, myPerson.pos.z, model_view, projection, viewport, &X, &Y, &Z);
		
		personScreenPos.set(X, ofGetHeight()-Y, Z);
	
	world.end();
	
	ofSetupScreen();	
	
	//this node draws in screen space.
	hand.draw();
	
	if( percentOver > 0.0 ){
		ofSetColor(255, 255, 255, 255*percentOver*1.2);	
		
		if( endCreatedInRise ){
			backgroundEnd.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
		}else{
			backgroundEnd2.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
		}
		
		world.begin();
			world.ourCameraSetup(width/2, height/2, 0.0, width, height, fov );
			endGraphic.draw();	
			myPerson.draw();
		world.end();
		ofSetupScreen();	
	}
	
	if( howTo.state != NODE_INACTIVE ){
		howTo.draw();
		myPerson.drawAtScreenPos(personScreenPos.x, personScreenPos.y, 0);
	}	
					
	ofSetColor(10, 50, 150);
	ofDrawBitmapString(hitsString, 10, 50);
	
	ofEnableAlphaBlending();

}


float lastx, lasty = 0.0;
//--------------------------------------------------------------
void camApp::keyPressed (int key){

	if( bStartedWithKeys ){

		if( key == 't'){
			setHowTo();
		}
		if( key == 'n'){
			setNormal();
		}
		
		if( key == 'K'){
			if( state == CAM_APP_TITLE && !title.introAllDone() ){
				title.forceNext();
			}
		}
		else if( key == OF_KEY_UP ){
			if( myGravity.y < 0.8 )myGravity.y += 0.2;
		}
		else if( key == OF_KEY_DOWN ){
			if( myGravity.y > -0.8 )myGravity.y += -0.2;
		}
		else if( key == OF_KEY_RIGHT ){
			if( myGravity.x < 0.5)myGravity.x += 0.15;
		}	
		else if( key == OF_KEY_LEFT ){
			if( myGravity.x > -0.5)myGravity.x -= 0.15;
		}		
			
	}
}

//--------------------------------------------------------------
void camApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void camApp::mouseMoved(int x, int y ){
	mouse.set(x, ofGetHeight()-y);
}

float prevY = 0.0;
float prevX = 0.0;

//--------------------------------------------------------------
void camApp::mouseDragged(int x, int y, int button){
//	//printf("button is %i\n", button);
//		
//	if( button == 0 ){
//		int diffx=x-lastx; //check the difference between the current x and the last x position
//		int diffy=y-lasty; //check the difference between the current y and the last y position
//		lastx=x; //set lastx to the current x position
//		lasty=y; //set lasty to the current y position
//		camMan.rotateX( (float) diffy ); //set the xrot to xrot with the addition of the difference in the y position
//		camMan.rotateY( (float) diffx );// set the xrot to yrot with the addition of the difference in the x position
//		
//	}else if( button == 2 ){
//		camMan.moveZ(  2.0 * ((float)y-(float)prevY) );
//		camMan.moveX( -2.0 * ((float)x-(float)prevX) );
//
//		prevX = x;
//		prevY = y;
//	}else{
//		camMan.moveY(  2.0 * ((float)y-(float)prevY) );
//		camMan.moveX( -2.0 * ((float)x-(float)prevX) );
//
//		prevX = x;
//		prevY = y;
//	}
//	
}

//--------------------------------------------------------------
void camApp::mousePressed(int x, int y, int button){
	lastx = x;
	lasty = y;
	prevY = y;
	prevX = x;		
}

//--------------------------------------------------------------
void camApp::mouseReleased(int x, int y, int button){

}

void camApp::windowResized(int w, int h){

}

