/*
 *  splashScreen.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 21/02/2010.
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

#include "splashScreen.h"



void splashScreen::setup(){
	splashImage.loadImage("assets/title/backgrounds/title.jpg");
	
	buttonLoading.loadImage("assets/title/buttons/loadingButton.png");
	buttonLaunch.loadImage("assets/title/buttons/launchButton.png");
	
	buttonLoading.setAnchorPercent(0.5, 0.5);
	buttonLaunch.setAnchorPercent(0.5, 0.5);
	
	buttonPos.set(248, 620);
	
	bWidth  = buttonLoading.width;
	bHeight = buttonLoading.height;
	
	state = SPLASH_NONE;
}

void splashScreen::update(){

}

void splashScreen::draw(){

	ofSetColor(255, 255, 255, 255);
	splashImage.draw(0,0);
	
	if( state >= SPLASH_SHOW_BUTTON ){
	
		ofEnableAlphaBlending();
		
		if( state <= SPLASH_LAUNCH_FLASH ){
			if( state == SPLASH_LAUNCH_FLASH ){
				ofSetColor(100, 100, 100, 255);
			}
			
			buttonLaunch.draw(buttonPos.x, buttonPos.y);
		}else{
		
			ofSetColor(255, 255, 255, 255);
			buttonLoading.draw(buttonPos.x, buttonPos.y);
		
		}
	}
	
}

bool splashScreen::checkHit(float x, float y){
	if( x > buttonPos.x - bWidth*0.5 && x < buttonPos.x + bWidth*0.5 && y > buttonPos.y - bHeight*0.5 && y < buttonPos.y + bHeight*0.5 ){
		return true;
	}
	return false;
}

bool splashScreen::checkPress(float x, float y){
	if( state == SPLASH_NONE )return false;
	
	if( checkHit(x, y) ){
		state = SPLASH_LAUNCH_FLASH;
		return true;
	}
	return false;
}

bool splashScreen::checkRelease(float x, float y){
	if( state == SPLASH_NONE )return false;

	if( checkHit(x, y) ){
		state = SPLASH_LAUNCH_READY;
		return true;
	}else{
		state = SPLASH_SHOW_BUTTON;
	}
	return false;
}