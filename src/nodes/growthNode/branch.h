/*
 *  branch.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 11/02/2010.
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
#include "leafElement.h"
#include "angleLine.h"
#include "ofMain.h"
#include "_projectGlobals.h"
#include "ofxFutureUtils.h"

class simpleBranch : public angleLine{
	public:
	
	void update(float rate, float leafHealth){
		angleLine::update(rate);
		
		float leafColor = ofMap(leafHealth, 0.2, 0.8, 80, 255, true);
		for(int k = 0; k < leafs.size(); k++){
			if( leafs[k].type == 1){
				leafs[k].setPosition(root.x, root.y, root.z);
			}else{
				leafs[k].setPosition(tip.x, tip.y, tip.z);
			}
			leafs[k].setRotation(0, 0, leafs[k].offsetAngle + angle * RAD_TO_DEG);
			leafs[k].color.set(leafColor);
		}
	}
	
	void drawLeafs(float scaleAmnt = 1.0){
		for(int k = 0; k < leafs.size(); k++){
			leafs[k].scaleAmnt = scaleAmnt;			
			leafs[k].draw();
		}
		for(int k = 0; k < rootLeafs.size(); k++){
			rootLeafs[k].scaleAmnt = scaleAmnt;
			rootLeafs[k].draw();
		}
	}
	
	float circleSize;
	
	vector <leafElement> rootLeafs; 
	vector <leafElement> leafs; 
};	

typedef enum{
	LEAF_NORMAL,
	LEAF_DYING,
	LEAF_GROWING
}leafState;

class branch{
	public:
		branch();
		
		void setLevel(int levelIn);		
		float getRootAngle();
				
		void update(float factor, float drawPctIn);		
		void updateAngle(float amnt);
		void killLeaves();
		void growLeaves();
		bool leavesDead();
		
		void releaseLeaves(vector <leafElement> & fallingLeaves);
		void draw();
		
		int level;
		leafState ourLeafState;
		
		float leafHealthPct;
		
		float drawPct;
		float targetScale;
		
		vector <simpleBranch> lines;
		vector <branch> children;
};