/*
 *  hairyStem.cpp
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

#include "hairyStem.h"

static float lastTime;
static ofImage glowImage;
static ofImage podImage;
static ofImage sporeImage;

void hairyStem::init(int num, ofxChipmunk & world, float startX, float startY, float endX, float endY){
	
	circle.assign(num, ofxChipmunkRect());
	pointCache.assign(num+2, ofPoint());
	rotation.assign(num+2, 0.0);
	
	tipSize = 0.0;
		
	if( glowImage.width == 0 ){
		glowImage.loadImage("assets/nodes/node16-beast/both/glow.png");
		glowImage.setAnchorPercent(0.5, 0.5);
	}
	
	if( podImage.width == 0 ){
		podImage.loadImage("assets/nodes/node16-beast/both/pod3.png");
		podImage.setAnchorPercent(0.5, 0.5);		
	}

	if( sporeImage.width == 0 ){
		sporeImage.loadImage("assets/nodes/node16-beast/both/spore.png");
		sporeImage.setAnchorPercent(0.5, 0.5);
	}		
	
	lastTime = ofGetElapsedTimef();
	
	ofVec3f delta(endX-startX, endY-startY);
	float totalLen = delta.length();
	blockLen = totalLen / num;
	
	float angle = HALF_PI + atan2(delta.y, delta.x);
	unique = angle;
	
	delta.normalize();
	
	for(int i = 0; i < circle.size(); i++){
		float posX = ofMap(i, 0, circle.size(), startX, endX);
		float posY = ofMap(i, 0, circle.size(), startY, endY);
		
		float mass = 0.2;
		if( i == 0 )mass = 0.0;
		else if( i == circle.size()-1){
			mass = 0.2;
		}
					
		circle[i].setup(posX, posY, 3.5, blockLen, mass, 0.0, 0.0, (i == 0 ) );
		cpBodySetAngle(circle[i].body, angle);
		world.addShape(&circle[i]);		
	}
	
	if( circle.size() ){
		tip.set(circle.back().body->p.x, circle.back().body->p.y, 0); 
	}
}

void hairyStem::addPins(ofxChipmunk & world){	
	for(int i = 0; i < circle.size(); i++){
		if( i > 0 ){
			world.addPinJoint(&circle[i-1], &circle[i], ofPoint(0.0, blockLen*-0.5), ofPoint(0.0, blockLen*0.5));
		}
	}
}

hairyStem::~hairyStem(){
	for(int i = 0; i < circle.size(); i++){
		circle[i].kill();
	}
	circle.clear();
}

//TODO: OPTOMIZE!!!!!!!!!!!!!!!!!

void hairyStem::updatePoints(){
	cpBody *body;
	cpPolyShape *poly;
	cpVect *verts;
	
	preTip = tip;
			
	int k = 0;
	for(int i = 0; i < circle.size(); i++){	
		ofxChipmunkRect & c = circle[i];
		
		body	= c.shape->body;
		poly	= (cpPolyShape *)c.shape;
		verts	= poly->verts;
	
		cpVect v1 = cpvadd(body->p, cpvrotate(verts[0], body->rot));
		cpVect v2 = cpvadd(body->p, cpvrotate(verts[1], body->rot));
		cpVect v3 = cpvadd(body->p, cpvrotate(verts[2], body->rot));
		cpVect v4 = cpvadd(body->p, cpvrotate(verts[3], body->rot));
		
		if( i ==  0 ){
//				float x = circle[i].shape->body->rot.y + circle[i+1].shape->body->rot.y;
//				float y = circle[i].shape->body->rot.y + circle[i+1].shape->body->rot.y;
//				float rot = atan2( y/2, x/2 );
		
			pointCache[k].set((v2.x+v3.x)/2, (v2.y+v3.y)/2);
			
//				rotation[k]		= rot;
//				rotation[k+1]	= rot;
			k++;
		}
		
		pointCache[k].set((v2.x+v3.x)/2, (v2.y+v3.y)/2);
		
		tip.set(pointCache[k].x, pointCache[k].y,0);
		
//			if( i > 0 && i < circle.size()-1 ){
//				float x = circle[i-1].shape->body->rot.y + circle[i+1].shape->body->rot.y;
//				float y = circle[i-1].shape->body->rot.y + circle[i+1].shape->body->rot.y;
//				float rot = atan2( y/2, x/2 );			
//				rotation[k] = rot;
//			}
		
		k++;
					
		if( i == circle.size()-1 ){
			pointCache[k].set((v4.x+v1.x)/2, (v4.y+v1.y)/2);
			rotation[k] = 0;
			k++;				
		}
				
	}
	
	tipVel = tip - preTip;
	
}

void hairyStem::draw(){

	//TODO: OPTIMIZE ME MOFO!
	
	ofNoFill();
	ofSetCurveResolution(6);
	ofSetLineWidth(1.5);
	
	ofBeginShape();
	for(int i = 0; i < pointCache.size(); i++){
		ofCurveVertex(pointCache[i].x, pointCache[i].y);			
	}
	ofEndShape(false);
	

	if( riseFallPct > 0.0 && pointCache.size() ){
		ofPushStyle();
		ofFill();
		globalColors.setColor("darkBlue");
		for(int i = 0; i < pointCache.size(); i++){
			if( i > pointCache.size()/2  && i < pointCache.size()-1){
			
				float rad = (2.0 + ofMap(i, pointCache.size()/2,  pointCache.size()-2, 6.0, 13.0, true) );
				float scaleXY = ofNoise((float)i*0.2, ofGetElapsedTimef() * 0.7) * ofMap( riseFallPct, 0.3, 0.8, 0.0, 1.0, true);
				
				if( i == pointCache.size() -2){
					tipSize = scaleXY;
				}
				
				sporeImage.draw(pointCache[i].x, pointCache[i].y, scaleXY*rad, scaleXY*rad); 
			}
		}
		ofPopStyle();
	}
	
	if( riseFallPct < 1.0 && pointCache.size() ){
		ofPushStyle();
		
		ofPoint & pt = pointCache[pointCache.size()-2];

		float pulse = ofSignedNoise(unique*0.4, ofGetElapsedTimef() * 0.9);
		float val;
		if( pulse < -0.2 ){
			val = 0.0;
		}else{
			val = ofMap(pulse, -0.2, 0.2, 0, 1, true); 
		}
		
		val *= (1.0-riseFallPct);

		ofSetColor(255, 255, 255, (1.0-riseFallPct)*155.0 + 100.0*val);
		glowImage.draw(pt.x, pt.y, 8, 8); 
		ofSetColor(255, 255, 255, 100.0 * val);
		glowImage.draw(pt.x, pt.y, 18, 18); 
		
		if( pulse > 0.75 ){
			float scaleAmnt = ofMap(pulse, 0.75, 0.80, 18, 48, true); 
			glowImage.draw(pt.x, pt.y, scaleAmnt, scaleAmnt); 
			
			if( pulse > 0.85 && ofGetElapsedTimef() - lastTime > ofRandom(0.4, 1.5) ){
				
				if( ofRandom(0, 200) > 100 ){
					globalSnd.play("HAIRY_1", ofRandom(0.2, 0.7), 1.0, ofRandom(-0.6, 0.6));
				}else{
					globalSnd.play("HAIRY_2", ofRandom(0.2, 0.7), 1.0, ofRandom(-0.6, 0.6));
				}
				lastTime = ofGetElapsedTimef();
			}
		}
		
		ofPopStyle();
	}

}

