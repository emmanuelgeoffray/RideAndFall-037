/*
 *  trailGroupManager.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 28/01/2010.
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

#include "ofMain.h"
#include "baseTrailParticle.h"
#include "srcDstTrailParticle.h"

class trailGroupManager{
	public:

		void setup(int numParticles, ofPoint spawnPoint, ofPoint targetPoint ){
			
			particles.assign(numParticles, srcDstTrailParticle());
			for(int i = 0; i < particles.size(); i++){
				particles[i].init( spawnPoint, targetPoint, 10, ofVec3f(ofRandom(10, -10), ofRandom(10, -10), ofRandom(10, -10)), ofVec3f(0,0,0) );
			}
			
		}
		
		void update(float forceMag){
			for(int i = 0; i < particles.size(); i++){
				particles[i].update(forceMag, 0.1, 0.75);
			}		
		}
		
		vector <baseElement *> getElements(){
			vector <baseElement *> elements;
			for(int i = 0; i < particles.size(); i++){
				elements.push_back(&particles[i]);
			}
			return elements;
		} 
		
		vector <srcDstTrailParticle> particles;
	
};
