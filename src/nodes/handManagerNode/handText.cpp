/*
 *  handText.cpp
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

#include "handText.h"

//--------------------------------------------------------------------------------
handText::handText(){
	ttf = NULL;
}

void handText::loadImage(string path, ofPoint anchorPercent, ofPoint rectPercent, bool bRightText){
	handTest.loadImage(path);
	handTest.setAnchorPercent(anchorPercent.x, anchorPercent.y);
	
	bAttachRight = bRightText;
	anchor		= anchorPercent;
	rectAnchor	= rectPercent;
}

//--------------------------------------------------------------------------------
void handText::setColor( simpleColor color ){
	textColor = color;
}

//--------------------------------------------------------------------------------
void handText::setTypeFace(ofTrueTypeFont * ttfPtr){
	ttf = ttfPtr;
}

//--------------------------------------------------------------------------------
void handText::setPosition(float x, float y, float z){
	pos.set(x, y, z);
}	

//--------------------------------------------------------------------------------
void handText::setText(string textIn){
	currentText = textIn;
}	

//--------------------------------------------------------------------------------
void handText::draw(){
		if( ttf == NULL ){
			return;
		}
		
		ofPushStyle();
		
			ofEnableAlphaBlending();
			ofSetColor(255, 255, 255);
						
			float handW =  handTest.width*scale.x;
			float handH =  handTest.height*scale.y;
									
			float sW = ttf->stringWidth(currentText);
			float sH = ttf->stringHeight(currentText) + ttf->stringHeight("L");
			
			float shiftX = ((sW*1.3)-sW)/2;
			
			sW *= 1.3;
			sH *= 1.4;

			float offsetTexH = 4.5;//ttf->stringWidth("L");
			
			ofSetColor(0xc9d3d9);
			ofSetRectMode(OF_RECTMODE_CORNER);
			
								//316 *  (0.785 - 0.0)  
			float boxTopLeftX  = handW * (rectAnchor.x - anchor.x);
			float boxTopLeftY  = handH * (rectAnchor.y - anchor.y);
			
			if( bAttachRight ){
				boxTopLeftX -= sW;
			}
						
			ofRect(pos.x + boxTopLeftX, pos.y + boxTopLeftY, sW, sH);
//			ofRect(pos.x + handW*(1.0-0.253), pos.y - handH*(1.0-0.280), sW, sH);

			ofSetColor(255, 255, 255);
			handTest.draw(pos.x, pos.y, handW, handH);

			ofSetColor(textColor.r, textColor.g, textColor.b, textColor.a);

			ttf->drawString(currentText, pos.x+shiftX+boxTopLeftX, pos.y+shiftX+offsetTexH+boxTopLeftY);
			
			//ttf->drawString(currentText, pos.x + shiftX + handW*(rectAnchor.x) - 2.0*anchor.x*sW,  pos.y + shiftX + offsetTexH - handH*(rectAnchor.y));

//			ttf->drawString(currentText, pos.x + 204, -94 + pos.y);

		
		ofPopStyle();

}