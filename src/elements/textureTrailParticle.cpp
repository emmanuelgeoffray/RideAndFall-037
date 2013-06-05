/*
 *  textureTrailParticle.cpp
 *  ofxFernCameraDemo
 *
 *  Created by theo on 01/02/2010.
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


#include "textureTrailParticle.h"

textureTrailParticle::textureTrailParticle(){
	imgPtr = NULL;
}

void textureTrailParticle::init(ofPoint position, float size, ofPoint velocity, ofPoint gravity){
	pos		= position;
	rad		 = size;
	vel		 = velocity;
	gra		 = gravity;
	drag	 = 0.978;
	maxN	 = 20;
	unique   = ofRandomuf();
	prevPoint = pos;
	startPoint = pos;
	trail.clear();
	visible = true;
}

void textureTrailParticle::update(float forceMag, float timeFactor, ofPoint attractor, float angleRange){
	vel *= drag;
	
//	float xforce = ofSignedNoise((float)ofGetFrameNum() * timeFactor, 1000, unique * 100.0);
//	float yforce = ofSignedNoise((float)ofGetFrameNum() * timeFactor, -1000, unique * 100.0);
//	float zforce = 0;//ofSignedNoise((float)ofGetFrameNum() * timeFactor, 33, unique * 100.0);
//	
//	float speed			= vel.length();

	//We do it by angle so as not to have the fish want to do an instant change of direction.

//	float speed		 = sqrt(vel.x*vel.x + vel.y*vel.y);

	//TODO: clean this shit up - maybe apply force via change in angle with limit to amount of change. 
	
	ofVec3f vec	 = attractor - pos;
	float tAngle     = atan2f(vec.y, vec.x);
	
	float angleShift = (DEG_TO_RAD * angleRange) * ofSignedNoise( ofGetElapsedTimef() * timeFactor , unique * 100.0 );
	float angle		 = tAngle + angleShift;	
				
	ofVec3f force( cos(angle), sin(angle), 0.0);
	force.normalize();

	vel += force * forceMag;
	pos += vel;			

	ofVec3f dist = pos - prevPoint;
	
	if( dist.lengthSquared() >= 0.25 ){
		updateTrail(prevPoint, pos+vel, 1, 20);	
		prevPoint = pos;			
	}

}

void textureTrailParticle::updateTrail(ofPoint & prevPoint, ofPoint currentPoint, int numPer, int maxNum){

	//TODO: optimize! 
	//CHECK THIS TOO!
	for(int i = 0; i < numPer; i++){
		float pct = (float)i/numPer;
		texSpineJoint jt;
		jt.pos = currentPoint*pct + prevPoint*(1.0-pct);
		
		trail.push_back(jt);
	}
	
	maxN = maxNum;
	
	if( trail.size() > maxNum ){
		trail.erase(trail.begin(), trail.begin()+(trail.size()-maxNum));
	}

	if( trail.size() > 3 ){
				
		ofVec3f delta;
		ofVec3f norm;
						
		for(int i = 0; i < trail.size(); i++){
			
			if( i == 0 ){
				delta = trail[1].pos - trail[0].pos;
			}else if( i == trail.size()-1 ){
				delta = trail[trail.size()-1].pos - trail[trail.size()-2].pos;
			}
			else{
				delta = trail[i+1].pos - trail[i-1].pos;
			}
				
			trail[i].pct =  (float)i/trail.size();
			delta.normalize();

			trail[i].norm.set(  -delta.y,  delta.x, 0.0 );
		}
	}
	
	
	
}

void textureTrailParticle::draw(){
	if( !visible || imgPtr == NULL )return;

	ofPushStyle();
	ofEnableAlphaBlending();			
			
	if( trail.size() > 3 ){
	
		ofPushMatrix();
		
		ofSetColor(color.r, color.g, color.b, color.a);
		
		ofTexture & ref = imgPtr->getTextureReference();
		ref.bind();
		
		ofPoint curPos = pos;
		ofPoint texCoord1;
		ofPoint texCoord2;
		
		//TODO: last point is a angle - leads to clipped texture.
		
		glBegin(GL_TRIANGLE_STRIP);
			for(int k = 0; k < trail.size(); k++){
				int i = (trail.size()-1)-k;
				
				float realPct = ( (float)trail.size()/maxN );		
						
				texCoord1 = ref.getCoordFromPercent(0.02, 0.02 + (1.0-trail[i].pct*realPct) * 0.98 );
				texCoord2 = ref.getCoordFromPercent(0.98,  0.02 + (1.0-trail[i].pct*realPct) * 0.98 );
								
				glTexCoord2f( texCoord1.x,  texCoord1.y);
				glVertex3fv( (shift + curPos - trail[i].norm * scale.x).getPtr());
				
				glTexCoord2f(texCoord2.x, texCoord2.y);						
				glVertex3fv( (shift + curPos + trail[i].norm * scale.x).getPtr());
				
				curPos -= ofPoint( trail[i].norm.y, -trail[i].norm.x ) * scale.y * realPct;
			}
		glEnd();
		
		ref.unbind();				

		ofPopMatrix();
	}
	
	ofPopStyle();
}
