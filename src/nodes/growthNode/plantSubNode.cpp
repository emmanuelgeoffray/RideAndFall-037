/*
 *  plantSubNode.cpp
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

#include "plantSubNode.h"


//-----------------------------------
plantSubNode::plantSubNode(){
	minBoundsY = -1000;
	maxBoundsY = 1000;
}

//-----------------------------------
void plantSubNode::setup(ofPoint position, string nodeText, float minUpdateSpeed, float maxUpdateSpeed){
	globalScale = 0.0;
	
	setupName("risePlant");
	setPosition(position.x, position.y, position.z);
	
	//THIS IS SO I CAN SYNC UP THE OTHER NODE TO US
	//addNodeEvent(NODE_ACTIVATING_RISE, nodeName, NODE_ACTIVATING_RISE);	

	lines.assign(200, simpleBranch());
	leafs.assign(4, ofImage());
	
	string baseFolder = "assets/nodes/node05-plant/both/";
	
	leafs[0].loadImage(baseFolder + "textures/leaf02.png");
	leafs[1].loadImage(baseFolder + "textures/leaf03.png");
	leafs[2].loadImage(baseFolder + "textures/leaf04.png");
	leafs[3].loadImage(baseFolder + "textures/leaf05.png");

	img.loadSequence(globalSeqLoader.getSequencePtr("PLANT_SEQ"), ofPoint(0.5, 0.5), true);
	img.setPosition(pos.x, pos.y, pos.z);
	img.setScale(1.4, 1.4, 1.0);
	img.setRotation(0, 0, ofRandom(-90, 90));

	for(int i = 0; i < leafs.size(); i++){
		leafs[i].setAnchorPercent(0.5, 1.0);
	}
	
	//PLANT TEXT 
	ofTrueTypeFont * ttf = globalType.getFontPtr("DIN_REG_M");

	float ang		= ofRandom(0, TWO_PI);
	float rad		= img.width * 0.3 * img.scale.x;
	ofPoint signPos = pos + ofPoint( cos(ang), sin(ang) ) * rad ;
	
	ofPoint attach(0.0, 0.5);
	if( ang > HALF_PI && ang < TWO_PI * 0.75){
		attach.set(1.0, 0.5);
	}

	signPost.setup(ttf, signPos, (ang*RAD_TO_DEG) + ofRandom(-20, 20), ofRandom(90, 170), attach);
	signPost.setText(nodeText);
	signPost.setBackgroundColor( globalColors.getColor("darkFall"));
	signPost.setTextColor( globalColors.getColor("liteBlue"));	
	signPostPct = 0.0;
	signPostOffset = ofRandom(-1, 0);
	
	numUsed = 0;
	int numRoots = 3;	
	targetScale = 1.0;
	
	float angleOffset = ofRandom(-60, 120);
		
	branchPct.assign(numRoots, 0.0);
	branchSpeed.assign(numRoots, 0.04);
	branches.assign(numRoots, branch());
	for(int i = 0; i < numRoots; i++){

		float jointLength = ofRandom(70, 150);
			
		float ang		= angleOffset + ofMap(i, 0, numRoots, 45, 45 + 360.0);
		float rad		= img.width * 0.4 * img.scale.x;
		ofPoint rootPos = pos + ofPoint( cos(ang*DEG_TO_RAD), sin(ang*DEG_TO_RAD) ) * rad;
		
		makeBranch(branches[i], ang, ofRandom(2, 3), jointLength, jointLength*0.8, rootPos);
		branches[i].setLevel(0);
		
		branchSpeed[i] = ofRandom(minUpdateSpeed, maxUpdateSpeed);
	
		//yes ridiculous I know!
		makeSubBranch(branches[i], -70, 1.6, 1);
		makeSubBranch(branches[i].children.back(), 70, 1.3, 2);
		makeSubBranch(branches[i].children.back().children.back(), -40, 1.3, 3);
	}

	killState = NODE_KILLABLE;
	state	  = NODE_INACTIVE;
}

//-----------------------------------
void plantSubNode::makeSubBranch( branch & vec, float angleOff, float scaleAmnt, int level ){
	
	vec.children.assign(1, branch());
	for(int k = 0; k < 1; k++){
		float angle = vec.lines.back().getAngle() - angleOff;
		float len   = vec.lines.back().targetLength * scaleAmnt;
		makeBranch(vec.children[k], angle, 1, len, len, ofPoint(), &( vec.lines.back() ) );
		vec.children[k].setLevel(level);
	}	
	
}

//-----------------------------------
void plantSubNode::makeBranch( branch & myBranch, float angle, int numJoints, float minJLength, float maxJLength, ofPoint rootPoint,  simpleBranch * root ){
		
	myBranch.lines.assign(numJoints, simpleBranch());
	
	int i = 0;
	for(int j = 0; j < numJoints; j++){
	
		myBranch.lines[i].init();
														
		if( j == 0 ){
			myBranch.lines[i].setFirst();
			
			if( root != NULL ){
				myBranch.lines[i].setRoot(&root->tip);
			}else{
				myBranch.lines[i].setRoot(rootPoint);
			}
			
			myBranch.lines[i].setAndStoreAngle( angle );
			
		}else{
			myBranch.lines[i].setRoot(&myBranch.lines[i-1].tip);
			myBranch.lines[i].setAndStoreAngle(myBranch.lines[i-1].getAngle() + 45.0 * ( i%2==0?-1.0:1.0) );
		}

			myBranch.lines[i].setAngleRange(ofMap(j, 0, numJoints, 20, 10));
			myBranch.lines[i].setLength( ofMap(j, 0, numJoints, maxJLength, minJLength ) );
			myBranch.lines[i].circleSize = ofRandom(4, 12);
			
			int numLeafs = ofRandom(3, 8);
			int numRoot = 0;
			
			if( myBranch.level == 0 ){
				numRoot   = ofRandom(2, 4.9);
				numLeafs += numRoot;
			}	
			
			for(int p  = 0; p < numLeafs; p++){
			
				float ang = ofRandom(-120 -90 , 120 -90 );
				float imgScale = ofRandom(0.6, 1.2) * ofMap(j, 0, numJoints, 1.2, 0.4);
				
				int whichImage = ofRandom(0, 3.99);
	
				leafElement colorLeaf;
				colorLeaf.setImageReference( leafs[whichImage], true);
				colorLeaf.offsetAngle = ang + ofRandom(8, 15);
				colorLeaf.setLeafBrightness(ofRandom(0.6, 1.0));
				colorLeaf.setScale(imgScale, imgScale, 1);
				
				if( numRoot > 0 && p >= numLeafs-numRoot){
					colorLeaf.type = 1;
				}else{
					colorLeaf.type = 0;
				}

				myBranch.lines[i].leafs.push_back( colorLeaf );					
			}

			i++;
		}
	
}

//-----------------------------------

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> plantSubNode::getElements(){
	vector <baseElement *> elementList;
	elementList.push_back(this);
	for(int i = 0; i < fallingLeaves.size(); i++){
		elementList.push_back(&fallingLeaves[i]);
	}
	return elementList;
} 

//-----------------------------------
void plantSubNode::fireFirstTimeStateReached(nodeState stateIn){
	state = stateIn;
	
	if( state == NODE_ACTIVATING_RISE ){
		img.restart();		
		img.play();
	}
	
	if( state == NODE_ACTIVATING_FALL ){
		for(int i = 0; i < branches.size(); i++){
			branches[i].killLeaves();
			branches[i].releaseLeaves(fallingLeaves);
			img.restart();
			img.play();				
		}
	}
	
	processNodeEvents();
}

//---------------- 
bool plantSubNode::shouldPersonWait(){
	return (state == NODE_ACTIVATING || state == NODE_ACTIVATING_FALL || state == NODE_ACTIVATING_RISE );
}

//-----------------------------------
void plantSubNode::update(){
	img.update();
	
	if( branchPct.size() ){
		signPost.update(0.65, ofClamp(signPostOffset + signPostPct, 0, 1) );
	}
	
	float timeNow =ofGetElapsedTimef();
	
	for(int i = 0; i < fallingLeaves.size(); i++){
		
		fallingLeaves[i].vel *= fallingLeaves[i].drag;		
		fallingLeaves[i].vel.y -= 0.58;
		fallingLeaves[i].vel.x = 0.5 * ofSignedNoise((float)i*0.5, ofGetElapsedTimef() * 0.3 );

		fallingLeaves[i].rot.z = ofLerpDegrees(fallingLeaves[i].rot.z, ofSignedNoise(fallingLeaves[i].pos.x * 0.2, ofGetElapsedTimef() * 0.14) * 20.0, 0.17);		
		fallingLeaves[i].pos += fallingLeaves[i].vel;
		
		if( fallingLeaves[i].timeToLive - timeNow < 1.5 ){
			fallingLeaves[i].scale.x *= 0.95;
			fallingLeaves[i].scale.y *= 0.95;			
		}
		
		if( fallingLeaves[i].brightness > 0.5 ){
			fallingLeaves[i].brightness *= 0.992;
		}		
								
		if( timeNow > fallingLeaves[i].timeToLive ){
			fallingLeaves[i].color.a *= 0.8;							
		}
	}
	
	fallingLeaves.erase(fallingLeaves.begin(), partition(fallingLeaves.begin(), fallingLeaves.end(), plantSubNode::killLeaves));
	
	for(int k = 0; k < branches.size(); k++){
		branches[k].update(3.6, branchPct[k]);		
	}	
	
	if( state == NODE_ACTIVATING_RISE ){
	
		for(int k = 0; k < branches.size(); k++){
			branches[k].growLeaves();
		}
		
		bool bDone = true;
		for(int i = 0; i < branchPct.size(); i++){
			branchPct[i] += branchSpeed[i];
			if( branchPct[i] < targetScale ){
				bDone = false;
				break;
			}else{
				branchPct[i] = targetScale;
			}
		}
		
		if( bDone ){
			targetScale += 1.0;
			fireFirstTimeStateReached(NODE_RISE);
		}
		
	}
	
	if( branchPct.size() && branchPct.back() > 0.9 ){
		signPostPct += branchSpeed.back()*2;
	}	
	
	if( state == NODE_ACTIVATING_FALL ){
		bool bDone = true;
		
		for(int k = 0; k < branches.size(); k++){
			if( branches[k].leavesDead() ){
				bDone = false;
			}
		}
		
		if( bDone ){
			fireFirstTimeStateReached(NODE_FALL);
		}
	}

}


//-----------------------------------
void plantSubNode::draw(){
	ofPushStyle();

		signPost.draw();			
		img.draw();
		
		for(int k = 0; k < branches.size(); k++){
			branches[k].draw();
		}

		//falling leaves are returned in get elements so they are drawn outside of this draw call

	ofPopStyle();			
			
}		

		