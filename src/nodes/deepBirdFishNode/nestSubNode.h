/*
 *  nestSubNode.h
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
#include "magnetElement.h"

class nestSubNode : public baseNode{

	public:
		virtual void setup(ofPoint position, float minRadius, float maxRadius, string tex1Path, string tex2Path);
		virtual void update(float minDist, float maxDist, float nearScale, float farScale);
		virtual void draw();	
		
		void checkState(float targetX, float targetY, float targetZ);
		
		imageElement bg;
		bool introFinished;
		
		float prePct;
		float pct;
		
		ofImage textureImage;
		ofImage textureImage2;
		
		bool bBird;
		bool bChanged;
				
		float attractPct;
		float timeReleased;
		
		float scaleFactor;

		vector <magnetElement> magnets;
};