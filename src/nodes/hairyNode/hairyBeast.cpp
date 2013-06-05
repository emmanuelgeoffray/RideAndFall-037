/*
 *  hairyBeast.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 20/02/2010.
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

#include "hairyBeast.h"
#include "hairyStem.h"

//-----------------------------------
hairyBeast::hairyBeast(){
	pVec = NULL;
}

//-----------------------------------
void hairyBeast::setup( ofPoint posIn, float scaleXY, float rotZ, float innerRadius, string image1, string image2 ){
	setPosition(posIn.x, posIn.y, posIn.z);
	rot.z	= rotZ;
	scale.x = scaleXY;
	scale.y = scaleXY;
	
	blinkSeq.loadSequence(globalSeqLoader.getSequencePtr("BEAST_EYE_SEQ"), ofPoint(0.5, 0.5), true); 
	timeNextBlink = ofGetElapsedTimef() + ofRandom(2.0, 8.0);			
	blinkSeq.play();
	
	lastTimeSoundPlayed = 0.0;
	
	radius = innerRadius;
	
	pupil.loadImage("assets/nodes/node16-beast/both/hairyPupil.png");
	eye.loadImage("assets/nodes/node16-beast/both/hairyEye.png");
	eye.setPosition(0, 0, 1);
	
	img.loadImage(image1, image2);
	img.setPosition(0, 0, 0);
	
	dustImg.loadImage("assets/nodes/node16-beast/both/spore.png");
	dustImg.setAnchorPercent(0.5, 0.5);
	
	dust.assign(100, imageParticleElement());
	for(int i = 0; i < dust.size(); i++){
		dust[i].setup(pos, ofPoint(), ofPoint(), ofRandom(0.98, 0.995), ofRandom(2, 4));
		dust[i].setImagePtr(&dustImg, true);
		dust[i].color = (globalColors.getColor("darkBlue"));
		dust[i].color.modulateRGB(ofRandom(0.3, 1.0));
		dust[i].visible = false;
	}
	
	float scaleAmnt = 1.3 * ( radius/(img.img1.width*0.5) );
	
	img.setScale(scaleAmnt, scaleAmnt, 1);
	pupil.setScale(scaleAmnt, scaleAmnt, 1);
	eye.setScale(scaleAmnt*1.3, scaleAmnt*1.3, 1);
	blinkSeq.setScale(scaleAmnt*0.95, scaleAmnt*0.95, 1);
	blinkSeq.setPosition(0, 0, 3);
	
	ogEyeScale = pupil.scale.x;
	eyeScale = ogEyeScale;
}

//-----------------------------------
void hairyBeast::setTentaclePointer(vector <hairyStem> * ptr){
	pVec = ptr;
}

//-----------------------------------
void hairyBeast::update(){
	if( bRise ){
		img.setFade(img.fade + 0.04);
		eyeScale = ofLerp(eyeScale, 0.9*ogEyeScale, 0.1);

		for(int i = 0; i < dust.size(); i++){
			dust[i].color = (globalColors.getColor("darkBlue"));
		}
	}else{
		img.setFade(img.fade - 0.04);
		eyeScale = ofLerp(eyeScale, 1.05*ogEyeScale, 0.1);			
		
		for(int i = 0; i < dust.size(); i++){
			dust[i].color = (globalColors.getColor("lightRise"));		
		}
	}
	
	float timeF = ofGetElapsedTimef();
	
	if( pVec != NULL && pVec->size() ){
	
		//TODO: OPTOMIZE!!!!!!!!!!!!!!!!!
		for(int i = 0; i < dust.size(); i++){
			if( dust[i].visible ){
								
				if( dust[i].vel.lengthSquared() < 1 ){
					dust[i].scale.x *= 0.968;					
					dust[i].scale.y *= 0.968;
					
					if( dust[i].scale.x <= 0.05 ){
						dust[i].visible = false;
					}
				}else{
					dust[i].updateForce( ofPoint(ofSignedNoise(dust[i].unique, 1 + timeF), ofSignedNoise(dust[i].unique, 10-timeF), 0) * 0.15  );
				}
				
			}else if( bRise && ofRandom(0, 100) > 80 ){
				
				int randTen = ofRandom(0, (float)pVec->size() * 0.99);
				
				hairyStem & stem = pVec->at(randTen);
				
				if( stem.tipSize > 0.65 && fabs(stem.tipVel.x) > 0.6 && fabs(stem.tipVel.y) > 0.6 ){
					dust[i].visible = true;
					
					ofVec2f p1(-stem.tipVel.y, stem.tipVel.x);
					ofVec2f p2(stem.tipVel.y, -stem.tipVel.x);
					ofVec2f tip = stem.tip;
					
					if( (tip+p2).lengthSquared() > (tip+p1).lengthSquared() ){
						dust[i].vel = (tip.normalized() * 2.5 + p1 * 0.5) * 0.5;
					}else{
						dust[i].vel = (tip.normalized() * 2.5 + p1 * 0.5) * 0.5;
					}
					
					if( ofGetElapsedTimef() -lastTimeSoundPlayed > ofRandom(0.1, 0.9) ){

						float volPct = ofMap(fabs(myPerson.pos.y - pos.y), 200, ofGetHeight(), 1.0, 0.0, true); 
						
						if( volPct > 0.0 ){
							globalSnd.play("HAIRY_RISE_DUST_RELEASE", ofRandom(0.3, 0.7)*volPct, 1.0, ofRandom(-0.5, 0.5) );
							lastTimeSoundPlayed = ofGetElapsedTimef();
						}
					}
					
					dust[i].pos = stem.tip;
					dust[i].scale = ofVec3f(1.0,1,1);
				}
				
				//dust[i].update();				
				
			}
		}
	}

	pupil.setScale(eyeScale, eyeScale, 1);
	
	blinkSeq.update();
	
	if( blinkSeq.isDone() && ofGetElapsedTimef() > timeNextBlink ){
		blinkSeq.restart();
		blinkSeq.play();
		timeNextBlink = ofGetElapsedTimef() + ofRandom(2.0, 8.0);		
	}
}

//-----------------------------------
void hairyBeast::setPersonPos(float x, float y, float z){
	vec.set(x-pos.x, y - pos.y);
	vec.normalize();
}

//-----------------------------------
void hairyBeast::drawEye(float vx, float vy){
	eye.draw();	
	pupil.setPosition(vx*eye.img.width*0.24, vy*eye.img.height*0.24, 2);
	pupil.draw();	
	blinkSeq.draw();
}

//-----------------------------------	
void hairyBeast::draw(){
	if( pVec == NULL ){
		return;
	}
	
	ofPushStyle();
	
		ofPushMatrix();
			
			ofTranslate(pos.x, pos.y, pos.z);
			ofScale(scale.x, scale.y, 1);
			
			ofPushMatrix();
				ofRotate(-rot.z);
				
				globalColors.setColor("darkBlue");		
				for(int i = 0; i < pVec->size(); i++){
					pVec->at(i).draw();
				}

				ofFill();
				img.draw();
				
			ofPopMatrix();

			drawEye(vec.x, vec.y);
			
			ofPushMatrix();
				//TODO: OPTOMIZE!!!!!!!!!!!!!!!!!
				ofRotate(-rot.z);
				for(int k = 0; k < dust.size(); k++){
					dust[k].draw();
				}		
			ofPopMatrix();
			
		ofPopMatrix();	
	
	ofPopStyle();
}
