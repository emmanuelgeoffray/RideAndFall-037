/*
 *  chargeAttractRod.cpp
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

#include "chargeAttractRod.h"

//------------------------------------------------------------------
chargeAttractRod::chargeAttractRod(){
	length = 100.0;
	drag = 0.98;
	targetAngle = 0;
	angleVel = 0;
	scale = 1.0;
	flip = false;
	thickness = 1.0;
	charges.assign(4, charge());
}

//------------------------------------------------------------------
void chargeAttractRod::setAngle(float ang){
	targetAngle = ang;
	angle		= ang;
}

//------------------------------------------------------------------
void chargeAttractRod::setFlip(bool bFlip){
	flip = bFlip;
}

//------------------------------------------------------------------
void chargeAttractRod::setDrag(float dragIn){
	drag = dragIn;
}

//------------------------------------------------------------------
void chargeAttractRod::setLength(float dist){
	length = dist;
}

//------------------------------------------------------------------
void chargeAttractRod::setForce(float force){
	for(int i = 0; i < charges.size(); i++){
		charges[i].myForce = 1.6 * ofMap(i, 0, charges.size(), force, force*0.4, true);
	}
}

//------------------------------------------------------------------
void chargeAttractRod::updateForce(){

	for(int i = 0; i < charges.size(); i++){
		charges[i].update();
		charges[i].prePt = getPercentAlongRod(charges[i].percent*0.97);
		charges[i].pt = getPercentAlongRod(charges[i].percent);
	}
}

//------------------------------------------------------------------
void chargeAttractRod::update(){

	if( length < 10 ){
		pts.clear();
		printf("RETURN - length == 0!!!!!\n");
		return;
	}	

	float numPts = length / 10.0;
	if( numPts <= 2 ){	
		printf("RETURN - numPts <= 2!!!!!\n");
		return;
	}
	
	if( numPts != pts.size() ){
		pts.clear();
		pts.assign( numPts, ofPoint() );
	}
	
	float sign = 1.0;
	if( flip ) sign = -1.0;		

	float segSize  = (float)1.0/numPts;
	float segSize2 = segSize*2.0;
	
	float px = pos.x;
	float py = pos.y;
	
	for(int i = 0; i < numPts; i++){
		if( i >= pts.size() )return;
		
		float pct   = (float)i/numPts;
		pct			= powf(pct, 2.0);
		
		pts[i].set(px, py, pos.z);
		
		if( i < numPts-1 ){
			px += sign*cos(2.0*angle*pct) * 10.0 * scale;
			py += sign*sin(2.0*angle*pct) * 10.0 * scale;
		}
													
//		if( percent >= pct && percent < pct + segSize ){
//			cx = ofMap(percent, pct, pct+segSize, preX, px, true);					
//					cy = ofMap(percent, pct, pct+segSize, preY, py, true);			
//		}		
		
	}

}

//------------------------------------------------------------------
ofPoint chargeAttractRod::getPercentAlongRod(float pct){
	
	int numPoints = pts.size();
	
	if( numPoints < 2 ){
		return ofPoint();
	} 
	
	pct = ofClamp(pct, 0, 1);

	//we assume even spacing! 
	//as thats the way we are making our lines

	int low = floor( pct * (float)numPoints );
	int hi  = low+1;

	if( hi >= numPoints || low >= numPoints ){
		return pts.back();
	}

	float pctBetween = ofMap(pct, low/(float)numPoints, hi/(float)numPoints, 0.0, 1.0, true);

	return ( pts[low]*(1.0-pctBetween) + pts[hi]*pctBetween );
}

//------------------------------------------------------------------
void chargeAttractRod::updateToTarget(float ourTargetAngle){
	angleVel *= drag;
	
	angleVel += ofAngleDifferenceRadians(angle, ourTargetAngle) * 0.01;
	
	angle += angleVel;
}

//------------------------------------------------------------------
void chargeAttractRod::updateToHome(){
	updateToTarget(targetAngle);
}

//------------------------------------------------------------------
void chargeAttractRod::draw(){
	if( pts.size() == 0 )return;

	ofPushStyle();
	
		ofNoFill();				
		ofSetColor(color.r, color.g, color.b, color.a);

		float sign = 1.0;
		if( flip ) sign = -1.0;
			
		ofSetLineWidth(2.0);
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < pts.size(); i++){
			if( i ==  0){
				glVertex3f(pts[i].x +( -1.0 * sign )*1000.0, pts[i].y, pts[i].z);
			}
			glVertex3f(pts[i].x, pts[i].y, pts[i].z);
		}
		glEnd();

		if( bRise ){
			ofSetColor(colorBall.r, colorBall.g, colorBall.b, colorBall.a*0.3);
		}else{
			simpleColor tmp = globalColors.getColor("lightRise");
			ofSetColor(tmp.r, tmp.g, tmp.b, tmp.a*0.3);
		}	
		if( thickness < 5 ){
			ofFill();
			for(int i = 0; i < charges.size(); i++){
				ofSetLineWidth(1.2 + i*2);
				ofLine(charges[i].pt.x, charges[i].pt.y, charges[i].prePt.x, charges[i].prePt.y);
			}
		}
		
		
//			
//			if( percent <= 1.0 ){
//				ofFill();
//				ofSetCircleResolution(8);
//				ofCircle(cx, cy, 4.0);
//			}
		
	ofPopStyle();
}
