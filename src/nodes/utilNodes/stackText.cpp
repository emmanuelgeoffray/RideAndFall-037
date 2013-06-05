/*
 *  stackText.cpp
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

#include "stackText.h"


//<name>red</name>
//<name>liteBlue</name>
//<name>midBlue</name>
//<name>darkBlue</name>
//<name>darkFall</name>
//<name>lightRise</name>

//--------------------------------------

void stackText::setup(ofPoint position, vector <riseFallString> riseFallText, bool bFlipOrder, bool bBig ){
	setPosition(position.x, position.y, position.z);
	
	isFlipping = false;
	bFlip = bFlipOrder;
	visible = true;
	
	if( riseFallText.size() == 0 ){
		return;
	}
	
	state = TEXT_STACK_INACTIVE;		
	pctActive = 0.0;
	
	combos.clear();
	combos.push_back(colorCombo("darkFall", "lightRise"));
	combos.push_back(colorCombo("red", "lightRise"));
	combos.push_back(colorCombo("liteBlue", "darkFall"));
	combos.push_back(colorCombo("lightRise", "red"));
	
	
//	combos.push_back(colorCombo("liteBlue", "midBlue"));
	
	vector <string> fonts;
	if( bBig ){
		fonts.push_back("DIN_REG_XL");
		fonts.push_back("DIN_REG_L");
		fonts.push_back("DIN_REG_M");
	}else{
		fonts.push_back("DIN_REG_L");
		fonts.push_back("DIN_REG_M");
		fonts.push_back("DIN_REG_S");	
	}
	
	phrases = riseFallText;
	
	vector <riseFallString> phrasesRev = phrases;
	reverse(phrasesRev.begin(), phrasesRev.end());
	
//	if( bFlip ){
//		reverse(phrases.begin(), phrases.end());
//	}
	
	blocks.clear();
	blocks.assign(phrases.size(), textFlipElement());

	ofPoint p;
	int randColor = (int)( ofRandom(0,  100) > 50 );
	
	if( !bRise ){
		 randColor += 2; 
	}
	
	for(int i = 0; i < blocks.size(); i++){
		
		simpleColor bgColor;
		simpleColor fgColor;
		
		bgColor = globalColors.getColor(combos[randColor].bgName);
		fgColor = globalColors.getColor(combos[randColor].fgName);
		
		float value = 1.0;//ofRandom(0.6, 1.0);
		bgColor.r *= value;
		bgColor.g *= value;
		bgColor.b *= value;
		
		string text;
		if( bFlip ){
			text = phrases[i].f;
			phrases[i].r = phrasesRev[i].r;
		}else{
			text = phrases[i].r;
			phrases[i].f = phrasesRev[i].f;			
		}

		int flip = bFlip?1:0;
		
		ofTrueTypeFont * ttf;
		if( text.length() < 8 ){
			ttf = globalType.getFontPtr(fonts[0]);
		}else if( text.length() < 12 ){
			ttf = globalType.getFontPtr(fonts[1]);
		}else{
			ttf = globalType.getFontPtr(fonts[2]);
		}
	
		p.z = 1; 
		
		blocks[i].setup(ttf, text);
		blocks[i].setBackgroundColor(bgColor);
		blocks[i].setTypeColor(fgColor);
		blocks[i].setPosition(p.x, p.y, p.z);
		blocks[i].visible = true;
		blocks[i].flipSide = bRise;
		
		p.x = ofSignedNoise(0.5, (float)i * 0.7) * blocks[i].width * 0.5;
		
		if( bFlip ){
			p.y -= blocks[i].height * 1.31;
		}else{
			p.y += blocks[i].height * 1.31;
		}
	}
		
	height = p.y;

	if( bFlip ){
		
		pos.y += fabs(height);
//		
//		for(int i = 0; i > blocks.size(); i++){
//			blocks[i].pos.y -= fabs(height);
//		}
		
	}
}


//--------------------------------------

//If you want to return this element (So that all drawing happens inside this class's draw() do this
//otherwise return your elements vector so that each element is depth sorted and drawn seperately
vector <baseElement *> stackText::getElements(){
	elements.clear();
	elements.push_back(this);

	return elements;
} 
		
//--------------------------------------
bool stackText::isActive(){
	return (state == TEXT_STACK_ACTIVE);
}

//--------------------------------------
void stackText::activate(){
	state		= TEXT_STACK_ACTIVATING;
	pctActive	= 0.0;
}

//--------------------------------------
void stackText::deactivate(){
	state		= TEXT_STACK_DEACTIVATING;
	pctActive	= 1.0;
}
		
//--------------------------------------
void stackText::update(float rate){
	
	//NOTE - MOVE THIS TO TIME PER TEXT BLOCK - so that long text takes longer.

	if( state == TEXT_STACK_ACTIVATING ){
		if( pctActive < 1.0 ){
		
			float prePercent = pctActive;
			pctActive += rate;
			
			if( (int)(pctActive*(float)blocks.size()) - (int)(prePercent*(float)blocks.size()) >= 1.0 ){
				globalSnd.play("TEXT_STACK_REVEAL", ofRandom(0.8, 0.9), 1.0, ofMap(pos.x, -ofGetWidth()/2, ofGetWidth()/2, -1.0, 1.0, true));
			}
			
			if( pctActive >= 1.0 ){
				state = TEXT_STACK_ACTIVE;
				pctActive = 1.0;
			}
		}
	}
	if( state == TEXT_STACK_DEACTIVATING ){
		if( pctActive > 0.0 ){
			pctActive -= rate*1.5;
			if( pctActive <= 0.0 ){
				state = TEXT_STACK_DEACTIVE;
				pctActive = 0.0;
			}
		}
	}	


	
		bool bPlayFlipSound = false;

		for(int i = 0; i < blocks.size(); i++){
		
			if( bRise ){
				if( blocks[i].flipSide == 0 && !blocks[i].isFlipping() ){
					if( i < blocks.size()-1 ){
						if( blocks[i+1].flipSide == 1 ){
							blocks[i].setTextFlip(phrases[i].r);
							bPlayFlipSound = true;
						}
					}else{
						blocks[i].setTextFlip(phrases[i].r);
						bPlayFlipSound = true;		
						isFlipping = false;				
					}
				}
			}
			
			if( !bRise ){
				if( blocks[i].flipSide == 1 && !blocks[i].isFlipping() ){
					if( i > 0 ){
						if( blocks[i-1].flipSide == 0 ){
							blocks[i].setTextFlip(phrases[i].f);
							bPlayFlipSound = true;							
						}
					}else{
						blocks[i].setTextFlip(phrases[i].f);
						bPlayFlipSound = true;
						isFlipping = false;										
					}
				}
			}
		
			blocks[i].update(0.19);
		}

		if( state != TEXT_STACK_DEACTIVE && fabs(myPerson.pos.y - (pos.y-height/2)) < 400 ){
			if( bPlayFlipSound ){
				globalSnd.play("TEXT_STACK_FLIP", ofRandom(0.8, 0.9), 1.0, ofMap(pos.x, -ofGetWidth()/2, ofGetWidth()/2, -1.0, 1.0, true));
			}
		}
		
	
}

//--------------------------------------
void stackText::draw(){
	
	if( state == TEXT_STACK_DEACTIVE || state == TEXT_STACK_DEACTIVE || !visible ) return;
	
	int numDrawn = (float)blocks.size() * ofClamp(pctActive, 0, 1);			
	if( numDrawn < 1 )return;
	
	float sign = 1.0;
	if(bFlip){
		sign = -1.0;
	}
	
	ofPushStyle();
		
		ofPushMatrix();
			ofTranslate(pos.x, pos.y, pos.z);
			
			if( bRise ){
				globalColors.setColor("darkFall");
			}else{
				globalColors.setColor("lightRise");
			}
			
			ofSetCurveResolution(12);

			ofNoFill();
						
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, (short)0x00FF);
			
			ofBeginShape();
			for(int i = 0; i < numDrawn; i++){
				if( i == 0 ){
					ofCurveVertex(blocks[i].pos.x, blocks[i].pos.y-blocks[i].height*2.0*sign);
					ofCurveVertex(blocks[i].pos.x, blocks[i].pos.y-blocks[i].height*2.0*sign);					
				}

				ofCurveVertex(blocks[i].pos.x, blocks[i].pos.y);
				if( i % 2 == 0 ){
					ofCurveVertex(blocks[i].pos.x+blocks[i].width*0.65, blocks[i].pos.y);				
				}else{
					ofCurveVertex(blocks[i].pos.x-blocks[i].width*0.65, blocks[i].pos.y);					
				}
				
				if( i == (numDrawn-1) ){
					
					ofCurveVertex(blocks[i].pos.x, blocks[i].pos.y+blocks[i].height*0.8*sign);
					
					if( numDrawn == blocks.size() ){
						ofCurveVertex(blocks[i].pos.x, blocks[i].pos.y+blocks[i].height*3.0*sign);					
						ofCurveVertex(blocks[i].pos.x, blocks[i].pos.y+blocks[i].height*3.0*sign);	
					}else{
						ofCurveVertex(blocks[i+1].pos.x, blocks[i+1].pos.y);	
					}
				}
			}
			ofEndShape(false);
			
			glDisable(GL_LINE_STIPPLE);
			
			ofFill();
			for(int i = 0; i < numDrawn; i++){
				blocks[i].draw();
			}
			
		ofPopMatrix();
	
	ofPopStyle();
}	


