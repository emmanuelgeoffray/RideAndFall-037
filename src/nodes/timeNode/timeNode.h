/*
 *  timeNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 18/02/2010.
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
#include "seqManagerElement.h"
#include "textArmElement.h"

class timeNode : public baseNode{

	public:
		timeNode();
		
		virtual void setup(ofPoint position);
		virtual vector <baseElement *> getElements();		
		virtual void update();
		virtual void draw();	
		
		//---------------- 
		bool shouldPersonWait();
		
		int waitFrame;
				
		seqManagerElement seqRise;
		seqManagerElement seqFall;

		textArmElement testArm[2];
		
		vector <string> timeStrings;
		
//		imageElement textBox;
		
		bool bEndPlayed;
		bool bLeft;
};