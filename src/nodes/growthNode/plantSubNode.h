/*
 *  plantSubNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 31/01/2010.
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
#include "imageElement.h"
#include "colorLoader.h"
#include "imageReferenceElement.h"
#include "ofxFutureUtils.h"
#include "leafElement.h"
#include "branch.h"
#include "seqManagerElement.h"
#include "plantSignPost.h"

class plantSubNode : public baseNode{

	public:
		plantSubNode();
	
		virtual void setup(ofPoint position, string nodeText, float minUpdateSpeed, float maxUpdateSpeed);
				
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements();
		virtual void fireFirstTimeStateReached(nodeState stateIn);
				
		static bool killLeaves(leafElement & A){
			return A.color.a <= 3.0;
		}
		
		//----------------
		virtual bool isKillable(){
			return (fallingLeaves.size() == 0);
		}

		//----------------
		virtual bool shouldPersonWait();
				
		//----------------
		virtual void update();
		virtual void draw();		
		
		float secondLevelScale;
		float globalScale;
		seqManagerElement img;
		
		
		vector <branch> branches;
		vector <simpleBranch> lines;
		vector <leafElement> fallingLeaves;
		
		float updateSpeed;
		
		int numUsed;
		float pct;
		float targetScale;
		
		ofTrueTypeFont ttf;
		vector <float> branchPct;
		vector <float> branchSpeed;
		
		vector <ofImage> leafs;
		
		plantSignPost signPost;		
		float signPostOffset;
		float signPostPct;
		
		
		protected:
			void makeSubBranch( branch & vec, float angleOffset, float scaleAmnt, int level);
			void makeBranch( branch & myBranch, float angle, int numJoints, float maxDist, float minDist, ofPoint rootPoint = ofPoint(),  simpleBranch * root = NULL );

};