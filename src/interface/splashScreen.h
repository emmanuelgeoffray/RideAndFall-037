/*
 *  splashScreen.h
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

#pragma once
#include "ofMain.h"

typedef enum{
	SPLASH_NONE,
	SPLASH_SHOW_BUTTON,
	SPLASH_LAUNCH_FLASH,
	SPLASH_LAUNCH_READY,
	SPLASH_QUIT,
	SPLASH_FINISHED
}splashState;

class splashScreen{
	public:
		void setup();
		void update();
		void draw();
		
		void setButtonVisible(){
			state = SPLASH_SHOW_BUTTON;
		}
		
		bool checkPress(float x, float y);
		bool checkRelease(float x, float y);
		
		splashState state;
		
		ofImage splashImage;
		ofImage buttonLoading;
		ofImage buttonLaunch;
		
		float bWidth;
		float bHeight;
		
		ofPoint buttonPos;
		
		protected:
			bool checkHit(float x, float y);
};