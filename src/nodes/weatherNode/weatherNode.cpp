/*
 *  weatherNode.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 12/02/2010.
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

#include "weatherNode.h"

void weatherNode::setup(ofPoint position){
	setupName("weatherNode"); 
	
	cloudImagesSun[0].loadImage("assets/nodes/node11-weather/both/clouds/cloudLite.png");
	cloudImagesSun[0].setAnchorPercent(0.5, 0.5);

	cloudImagesSun[1].loadImage("assets/nodes/node11-weather/both/clouds/cloudDark.png");
	cloudImagesSun[1].setAnchorPercent(0.5, 0.5);

	cloudImagesSun[2].loadImage("assets/nodes/node11-weather/both/clouds/cloudBack1.png");
	cloudImagesSun[2].setAnchorPercent(0.5, 0.5);

	cloudImagesSun[3].loadImage("assets/nodes/node11-weather/both/clouds/cloudBack2.png");
	cloudImagesSun[3].setAnchorPercent(0.5, 0.5);

	sunRainPct = 0.0;

//	rain.assign(200, rainParticleElement());
//	for(int i = 0; i < rain.size(); i++){
//		rain[i].visible = false;
//		rain[i].reset();
//		
//		if( i % 2 == 0 ){
//			rain[i].imageState = 0;
//			rain[i].setImageReference(rainImages[0], true);
//		}else{
//			rain[i].imageState = 1;
//			rain[i].setImageReference(rainImages[1], true);			
//		}
//		
//		rain[i].color.set( ofRandom(160, 255) );
//		
//		float myScale = ofRandom(0.2, 0.45);
//		rain[i].setScale( myScale, myScale, 1.0);
//		elements.push_back(&rain[i]);		
//	}
	
	float screenW = ofGetWidth();
	float screenH = ofGetHeight();
	
	ofPoint randPos;
	clouds.assign(12, cloudElement() );
	for(int i = 0; i < clouds.size(); i++){
		
		bool big = false;
		
		if( i < 7 ){
			if( i == 0 || i == 4 )big = true;	
			
			if( big || i == 3 ){
				clouds[i].setImageReference(cloudImagesSun[i%2], cloudImagesSun[i%2], true);
				clouds[i].setScale(0.67, 0.67, 1.0);
			}else{
				float scale = ofRandom(0.8, 1.2);
				clouds[i].setScale(scale, scale, 1.0);
				clouds[i].setImageReference(cloudImagesSun[i%2], cloudImagesSun[i%2], true);
			}
			randPos = myPerson.pos + ofPoint( ofRandom(-screenW*0.8, screenW*0.8), ofRandom(-screenH*2.0, screenH*2.0), big?ofRandom(10, 70):ofRandom(-100, -500) );
		}else{
			float scale = ofRandom(0.98, 1.5);
			clouds[i].setImageReference(cloudImagesSun[2 + i%2], cloudImagesSun[2 + i%2], true);
			clouds[i].setScale(scale, scale, 1.0);
			randPos = myPerson.pos + ofPoint( ofRandom(-screenW*1.4, screenW*1.4), ofRandom(-screenH*2.0, screenH*2.0), ofRandom(-550, -750) );
		}
		
		clouds[i].setPosition( randPos.x, randPos.y, randPos.z );
		
		elements.push_back(&clouds[i]);
	}
	
}

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> weatherNode::getElements(){
	return elements;
}


ofPoint weatherNode::getRandomCloudPosition(){
	if( clouds.size() == 0 )return ofPoint();
	
	int offset = ofRandom(0, (float)clouds.size() * 0.99);
	offset = ofClamp(offset, 0, clouds.size()-1);
	
	return clouds[offset].pos;
}
		
void weatherNode::update(){

	sunRainPct = ofNoise(ofGetElapsedTimef() * 0.05, 100.0);
	float rate = ofMap(sunRainPct, 0.2, 0.8, 0.075, 0.0, true);
	
	if( riseFallPct > 0.7 ){
		for(int i = 0; i < clouds.size(); i++){
			clouds[i].color.r = ofLerp( clouds[i].color.r, 255.0, 0.1);
			clouds[i].color.g = clouds[i].color.r;
			clouds[i].color.b = clouds[i].color.r;
		}
	}
	if( riseFallPct < 0.3 ){
		for(int i = 0; i < clouds.size(); i++){
			clouds[i].color.r = ofLerp( clouds[i].color.r, 20.0, 0.1);
			clouds[i].color.g = clouds[i].color.r;
			clouds[i].color.b = clouds[i].color.r;
		}
	}
	
	float screenW = ofGetWidth();
	float screenH = ofGetHeight();	

	for(int i = 0; i < clouds.size(); i++){
	
		float fade = ofMap( sunRainPct, 0.25, 0.75, 0.0, 1.0, true);
		clouds[i].fadePct = fade;
	
		if( fabs( clouds[i].pos.y - myPerson.pos.y ) > screenH * 1.65 ){
			if( myPerson.vel.y > 0 && clouds[i].pos.y < myPerson.pos.y ){
				ofPoint pos = myPerson.pos + ofPoint( ofRandom(-screenW*0.8, screenW*0.8), screenH * ofRandom(1.2, 1.9),  clouds[i].pos.z );
				clouds[i].setPosition( pos.x, pos.y, pos.z );
			 } 

			if( myPerson.vel.y < 0 && clouds[i].pos.y > myPerson.pos.y ){
				ofPoint pos = myPerson.pos + ofPoint( ofRandom(-screenW*0.8, screenW*0.8), -screenH * ofRandom(1.2, 1.9), clouds[i].pos.z );
				clouds[i].setPosition( pos.x, pos.y, pos.z );
			 } 
			
		}
	}
	
//	for(int i = 0; i < rain.size(); i++){
//		if( !rain[i].visible && ofNoise(ofGetElapsedTimef()*10.0, i) > 0.925 + rate ){
//			
//			ofPoint p = getRandomCloudPosition() + ofPoint( ofRandom(-100, 100), ofRandom(-50, 50), ofRandom(-2, -10) );
//		
//			rain[i].visible = true;
//			rain[i].setPosition( p.x, p.y, p.z );
//			rain[i].color.a = 200;		
//			
//			ofPoint v = ofPoint( ofSignedNoise(ofGetElapsedTimef() * 0.01, 0.1) * 0.4, -0.5) * ofRandom(9, 15);
//			rain[i].setVelocity( v.x, v.y, 0.0 );
//		}
//		
//		if( bRise && rain[i].imageState == 2 ){
//			rain[i].setImageReference(rainImages[1], true);
//			rain[i].imageState = 1;
//		}
//		else if( !bRise && rain[i].imageState == 1 ){
//			rain[i].setImageReference(rainImages[2], true);
//			rain[i].imageState = 2;			
//		}
//
//		rain[i].update();
//		
//		if( rain[i].distTravelled >= 400 ){
//			rain[i].color.a = ofMap(rain[i].distTravelled, 400, 600, 200, 0, true);
//		}
//		if( rain[i].distTravelled >= 600 ){
//			rain[i].reset();
//			rain[i].visible = false;
//		}
//	}
	

}

void weatherNode::draw(){

}		
		
