/*
 *  hairyBeast.h
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

#pragma once
#include "ofMain.h"
#include "_projectGlobals.h"
#include "ofxChipmunk.h"
#include "imageCrossFadeElement.h"
#include "hairyStem.h"
#include "seqManagerElement.h"
#include "imageElement.h"
#include "imageParticleElement.h"

class hairyBeast : public baseElement{
	
	public:
	
		hairyBeast();
		void setup( ofPoint posIn, float scaleXY, float rotZ, float innerRadius, string image1, string image2 );
		void setTentaclePointer(vector <hairyStem> * ptr);
		void update();
		void setPersonPos(float x, float y, float z);
		void drawEye(float vx, float vy);
		void draw();
		
		imageElement pupil;
		imageElement eye;
		seqManagerElement blinkSeq;
		
		float lastTimeSoundPlayed;
		
		float timeNextBlink;

		float radius;
		ofVec2f vec;
		imageCrossFadeElement img;
		
		float eyeScale;
		float ogEyeScale;

		ofImage dustImg;
		
		//GHETTO - but makes things a lot faster
		//we have one set of tentacles which all the hairy dudes share		
		vector <hairyStem> * pVec;
		vector <imageParticleElement> dust;
		
};
