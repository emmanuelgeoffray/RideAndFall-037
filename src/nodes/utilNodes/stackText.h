/*
 *  stackNode.h
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

#pragma once
#include "ofMain.h"
#include "baseNode.h"
#include "textFlipElement.h"

#include "textStackUtils.h"

class stackText : public baseElement{

	public:
		virtual void setup(ofPoint position,  vector <riseFallString> riseFallText, bool bFlip, bool bBig = false);
		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();
		
		bool isActive();
		void activate();
		void deactivate();
		
		bool textDone(){
			return (TEXT_STACK_ACTIVE || TEXT_STACK_DEACTIVE);
		}
		
		void show(){
			visible = true;
		}
		
		void hide(){
			visible = false;
		}
		
		stackTextState getState(){
			return state;
		}
		
		
		bool bFlip;
		virtual void update(float rate = 0.04);
		virtual void draw();
		
		float width;
		float height;
		
		bool isFlipping;
				
		simpleColor typeColor, typeColor2,	bgColor, bgColor2;		
		float pct;
		
		vector <baseElement *> elements;		
		vector <riseFallString> phrases;
		vector <textFlipElement> blocks;
		vector <colorCombo> combos;
		
	protected:
		float pctActive;
		stackTextState state;
		
};