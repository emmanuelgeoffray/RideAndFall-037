/*
 *  titleNode.h
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
#include "textBlockElement.h"
#include "textBlockTrackingElement.h"
#include "imageParticleElement.h"
#include "imageElement.h"
#include "imageReferenceElement.h"
#include "seqManagerElement.h"

class titleNode : public baseNode{

	public:
		virtual void setup(ofPoint position);
		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();
						
		virtual void setState(nodeState stateIn);		
		virtual void update();
		virtual void draw();
		
		bool introAllDone();
		void taskCompleted();
		
		bool holdPerson(){
			return ( bReleasePerson == false );
		}
		
		void forceNext();

		//----------------
		void checkState(float targetX, float targetY, float targetZ);
		bool shouldPersonWait();
		
		virtual void updateAngle(float angleIn);
				
		virtual float timeInMode();
		
		ofTrueTypeFont ttf;	
		
		simpleColor typeColor, typeColor2,	bgColor, bgColor2;		
		float pct;
		
		ofImage titleImg;
		
		bool needToAdvance;
		
		vector <ofPoint> positions;
		vector <string> stepString;
		
		int mode;
		int completedMode;
		
		float timeModeStarted;
		float timeToShowCheck;
		float completedTime;
		
		float fadeInPct;
		float magTrackingFade;
		float magTrackingAngle;
		
		seqManagerElement seqMan;

		imageParticleElement titleImageParticle;
		imageElement magTrack;		
		imageElement mag;
		imageElement textBox;
		
		
		float redLinePosY;
		float redLineMinX;
		float redLineMaxX;
		
		ofImage handImg;
		imageReferenceElement handLeft;
		imageReferenceElement handRight;		
		
		textBlockElement textBlock;
		
	protected:		
		bool bReleasePerson;	
		virtual void nextStep();

};