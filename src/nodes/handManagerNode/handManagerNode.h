/*
 *  handManagerNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 29/01/2010.
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
#include "textStackFromXml.h"
#include "handTextManager.h"

class handManagerNode : public baseNode{

	public:
	
		virtual void setup(ofPoint position);
		
		void updateCamToScreenCoords();
		bool doPinPrick();
		bool doFanBlow();

		virtual void update();
		virtual void draw();	
		
		float lastTimeBurst;

		seqManagerElement seqFan;		
		seqManagerElement seqPin;
		
		ofPoint myPersonScreenPosFan;		
		ofPoint myPersonScreenPosPin;		

		ofPoint storePersonPosPin;						
		ofPoint storePersonPosFan;
		ofPoint handStorePoint;
		bool needToCalcStringPos;


};