/*
 *  textArmElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 05/02/2010.
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
 
#include "textArmElement.h"

//---------------------------------------------------------------------------------------------------
textArmElement::textArmElement(){
	ttf			= NULL;
	textAngle	= 0;
	state		= TEXT_ARM_HIDDEN;
	pct			= 0.0;
}

//---------------------------------------------------------------------------------------------------
void textArmElement::setupType(ofTrueTypeFont * ttfPtr, simpleColor backColor, simpleColor textColor, bool flip, ofPoint anchor){
	ttf			= ttfPtr;
	bgColor		= backColor;
	typeColor	= textColor;
	
	textBlock.setup(ttf, "Test Text, Some Text!", NULL, anchor, flip);
	textBlock.setBackgroundColor(bgColor);
	textBlock.setTypeColor(typeColor);
}

//---------------------------------------------------------------------------------------------------
void textArmElement::setupArm(float startAngle, float angleRange, float length, float textAngleOffset, simpleColor lineColorIn){
	arm.clear();
	arm.assign(2, angleLine());

	arm[0].init();
	arm[0].setRoot(ofPoint());	
	arm[0].setAndStoreAngle(startAngle);
	arm[0].setLength(length / 2 );
	arm[0].setAngleRange(angleRange);
	arm[0].targetDrag = 0.82;
	
	arm[1].init();	
	arm[1].setRoot(&arm[0].tip);
	arm[1].setAndStoreAngle(startAngle);
	arm[1].setLength(length / 2 );
	arm[1].setAngleRange(angleRange);
	arm[1].targetDrag = 0.87;

	textAngle = textAngleOffset;
	lineColor = lineColorIn;
	
}

//---------------------------------------------------------------------------------------------------
textArmState textArmElement::getState(){
	return state;
}

//---------------------------------------------------------------------------------------------------
void textArmElement::show(string text, float durSeconds){
	if( state == TEXT_ARM_HIDDEN ){	
		textBlock.setText(text);
		duration = durSeconds;
		state    = TEXT_ARM_IN;
		pct		 = 0.0;
		showTill = ofGetElapsedTimef() + duration;
	}
}

//---------------------------------------------------------------------------------------------------
void textArmElement::hide(){
	if( state != TEXT_ARM_HIDDEN ){
		state = TEXT_ARM_OUT;
	}
}

//---------------------------------------------------------------------------------------------------
bool textArmElement::isBusy(){
	return (state != TEXT_ARM_HIDDEN);
}

//---------------------------------------------------------------------------------------------------
void textArmElement::update(float rate){

	if( state == TEXT_ARM_IN ){
		pct += rate;
		if( pct >= 1.0 ){
			pct			= 1.0;
			state		= TEXT_ARM_SHOWING;
			showTill	= ofGetElapsedTimef() + duration;
		}
	}
	else if( state == TEXT_ARM_SHOWING && showTill < ofGetElapsedTimef() ){
		state = TEXT_ARM_OUT;
	}
	else if( state == TEXT_ARM_OUT ){
		pct -= rate;
		if( pct <= 0.0 ){
			pct = 0.0;
			state = TEXT_ARM_HIDDEN;
		}
	}	
	
	for(int i = 0; i < arm.size(); i++){
		arm[i].update(1.37);
		arm[i].scale = pct;
	}
	
	textBlock.setScale(pct, pct, 1.0);
	
}

//---------------------------------------------------------------------------------------------------
void textArmElement::draw(){

	ofPushStyle();
		ofEnableAlphaBlending();
		
		if( state != TEXT_ARM_HIDDEN && arm.size() ){
			ofPushMatrix();
				
				ofTranslate(pos.x, pos.y, pos.z);
				
				ofSetLineWidth(3.0);
				
				ofSetColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
				for(int i = 0; i < arm.size(); i++){
					ofLine(arm[i].root.x*arm[i].scale, arm[i].root.y*arm[i].scale, arm[i].tip.x*arm[i].scale, arm[i].tip.y*arm[i].scale);
				}

				ofSetColor(lineColor.r*0.7, lineColor.g*0.7, lineColor.b*0.7, lineColor.a);
				for(int i = 0; i < arm.size(); i++){
					ofCircle(arm[i].root.x*arm[i].scale, arm[i].root.y*arm[i].scale, arm[i].scale * 4.5);					
				}				
				
				textBlock.setRotation(0, 0, textAngle);
				textBlock.setPosition(arm.back().tip.x*arm.back().scale, arm.back().tip.y*arm.back().scale, 0);
				textBlock.draw();
			
			ofPopMatrix();
		}
		
	ofPopStyle();

}





