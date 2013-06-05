/*
 *  magnetElement.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 12/02/2010.
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
#include "imageReferenceElement.h"
#include "ofMain.h"
#include "colorLoader.h"
#include "ofxFutureUtils.h"

class magnetElement : public baseElement{
	public:
		
		magnetElement(){
			angleVel = 0;
			angle	 = 0;
			width    = 32;
			height   = 32;
			
			stretch = 1.0;
			
			drag = ofRandom(0.95,0.98);
			
			tex = NULL;
		}
		
		void setPosition(float x, float y, float z){
			pos.set(x, y, z);
			ogPos = pos;
		}
		
		void updatePosViaStretch(float stretch, ofPoint & relPoint){
			pos = ogPos * stretch + relPoint * (1.0-stretch);
		}
		
		void setTexturePtr(ofTexture * ptr){
			tex = ptr;
		}
		
		void update(float targetAngle){
			if( tex == NULL)return;
			
			float angleDelta = ofAngleDifferenceDegrees(angle, targetAngle);
			
			angleVel *= drag;
			angleVel += angleDelta * 0.08;
			
			angle += angleVel;
			
			pt1.set(0.5, 0);
			pt2.set(0.0, 0.5);			
			pt3.set(0.75, 0.75);
						
			pt1.rotate(angle);
			pt2.rotate(angle);
			pt3.rotate(angle);
			
			tex0 = tex->getCoordFromPercent(0.5, 0.5);
			tex1 = tex->getCoordFromPercent(pt1.x*0.5 + 0.5, pt1.y*0.5 + 0.5);
			tex2 = tex->getCoordFromPercent(pt2.x*0.5 + 0.5, pt2.y*0.5 + 0.5);
			tex3 = tex->getCoordFromPercent(pt3.x*0.5 + 0.5, pt3.y*0.5 + 0.5);			
			
			wStretch = width * stretch;
			hStretch = height * stretch;
		}
		
		void draw(){
			if( tex == NULL)return;
			
//
//			if( stretch > 2.4 ){
//				ofSetColor(color.r * 1.4, color.g * 1.4, color.b * 1.4, color.a * 0.6);
//				
//				float sAmnt = 1.2* stretch-2.0;
//				
//				glTexCoord2fv(tex0.v);
//				glVertex3f(pos.x, pos.y, pos.z);
//				glTexCoord2fv(tex1.v);
//				glVertex3f(pos.x+pt1.x*wStretch*sAmnt, pos.y+pt1.y*hStretch*sAmnt, pos.z);
//				glTexCoord2fv(tex2.v);
//				glVertex3f(pos.x+pt2.x*wStretch*sAmnt, pos.y+pt2.y*hStretch*sAmnt, pos.z);
//				glTexCoord2fv(tex1.v);
//				glVertex3f(pos.x+pt1.x*wStretch*sAmnt, pos.y+pt1.y*hStretch*sAmnt, pos.z);
//				glTexCoord2fv(tex2.v);
//				glVertex3f(pos.x+pt2.x*wStretch*sAmnt, pos.y+pt2.y*hStretch*sAmnt, pos.z);
//				glTexCoord2fv(tex3.v);
//				glVertex3f(pos.x+pt3.x*wStretch*sAmnt, pos.y+pt3.y*hStretch*sAmnt, pos.z);
//			}
			
			ofSetColor(color.r, color.g, color.b, color.a);			
		
	//		glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2fv(tex0.getPtr());
				glVertex3f(pos.x, pos.y, pos.z);
				glTexCoord2fv(tex1.getPtr());
				glVertex3f(pos.x+pt1.x*wStretch, pos.y+pt1.y*hStretch, pos.z);
				glTexCoord2fv(tex2.getPtr());
				glVertex3f(pos.x+pt2.x*wStretch, pos.y+pt2.y*hStretch, pos.z);
				glTexCoord2fv(tex1.getPtr());
				glVertex3f(pos.x+pt1.x*wStretch, pos.y+pt1.y*hStretch, pos.z);
				glTexCoord2fv(tex2.getPtr());
				glVertex3f(pos.x+pt2.x*wStretch, pos.y+pt2.y*hStretch, pos.z);
				glTexCoord2fv(tex3.getPtr());
				glVertex3f(pos.x+pt3.x*wStretch, pos.y+pt3.y*hStretch, pos.z);
	//		glEnd();	
			
		}
		
		ofPoint tex0, tex1, tex2, tex3;
		ofPoint ogPos;
		
		float wStretch;
		float hStretch;
		float stretch;
		float drag;
		simpleColor color;
		
		ofVec2f pt1, pt2, pt3;
		ofTexture * tex;
		
		float width;
		float height;
		
		float angleVel;
		float angle;
};
