/*
 *  leafElement.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 11/02/2010.
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

#include "leafElement.h"

//----------------------------------------------------
leafElement::leafElement(){
	drawPct		= 0.0;
	offsetAngle = 0.0;
	shrink = 1.0;
	type = 0;
	brightness = 1.0;
	scaleAmnt = 1.0;
}
	
//----------------------------------------------------
void leafElement::setLeafBrightness(float valIn){
	brightness = valIn;
}	

//----------------------------------------------------
void leafElement::setScaleAmount(float scaleIn){
	scaleAmnt = scaleIn;
}	


//----------------------------------------------------
void leafElement::draw(){
	if( !visible ){
		//printf("warning not visible\n");
		return;
	}
	ofPushStyle();
	//ofSetColor(255, 255, 255);
	
		if( hasAlpha ){
			ofEnableAlphaBlending();
		}
				
		ofSetColor(color.r * brightness, color.g * brightness, color.b * brightness, color.a);

		ofPushMatrix();
			ofTranslate(pos.x, pos.y, pos.z);
			if( rot.x != 0.0 )ofRotate(rot.x, 1, 0, 0);
			if( rot.y != 0.0 )ofRotate(rot.y, 0, 1, 0);
			if( rot.z != 0.0 )ofRotate(rot.z, 0, 0, 1);
			
			if( flip ){
				img->draw(0, 0, (float)img->width * scale.x * shrink * scaleAmnt, (float)img->height * -scale.y * shrink * scaleAmnt);
			}else{
				img->draw(0, 0, (float)img->width * scale.x * shrink * scaleAmnt, (float)img->height * scale.y * shrink * scaleAmnt);				
			}
			
		ofPopMatrix();
	
	ofPopStyle();
	
}