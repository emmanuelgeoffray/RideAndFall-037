/*
 *  simpleSceneGraph.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 27/01/2010.
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

#include "baseElement.h"


static ofPoint cameraPos;

static bool sortDistFromCamXYZ(baseElement * A, baseElement * B){
	
	float distA = (A->pos.x-cameraPos.x)*(A->pos.x-cameraPos.x) + (A->pos.y-cameraPos.y)*(A->pos.y-cameraPos.y) + (A->pos.z-cameraPos.z)*(A->pos.z-cameraPos.z); 
	float distB = (B->pos.x-cameraPos.x)*(B->pos.x-cameraPos.x) + (B->pos.y-cameraPos.y)*(B->pos.y-cameraPos.y) + (B->pos.z-cameraPos.z)*(B->pos.z-cameraPos.z); 

	return (distA > distB);
	
}

static bool sortDistFromCamZ(baseElement * A, baseElement * B){
	
	float distA = (cameraPos.z-A->pos.z);
	float distB = (cameraPos.z-B->pos.z); 

	return (distA > distB);
	
}

class simpleSceneGraph{
	
	public:
		
		simpleSceneGraph(){
			numSkipped = 0;
			numDrawn   = 0;
		}
		
		void updateCameraPos(ofPoint cameraPosition){
			cameraPos = cameraPosition;
		}

		void addElements(vector <baseElement *> elementList){
			for(int i = 0; i < elementList.size(); i++){
				addElement(elementList[i]);
			}
		}
		
		void addElement(baseElement * element){
			vector <baseElement *> tmp = element->getElements();
			for(int i = 0; i < tmp.size(); i++){
				elements.push_back(tmp[i]);
			}
		}
		
		void clearElements(){
			elements.clear();
		}

		void sortZ(){
			sort(elements.begin(), elements.end(), sortDistFromCamZ);
		}
		
		void sortXYZ(){
			sort(elements.begin(), elements.end(), sortDistFromCamXYZ);
		}

		void renderWithClipping(ofRectangle clipRect, float minZ, float maxZ, float camDist, float fov){
			
			//at this point we are depth sorted
			//we just want to skip elements that are offscreen
			
			float minX = clipRect.x;
			float maxX = clipRect.x + clipRect.width;
			float minY = clipRect.y;
			float maxY = clipRect.y + clipRect.height;
			
			float camCenterX = (minX + maxX) / 2;
			float camCenterY = (minY + maxY) / 2;
					
			ofPoint pos; 
			int skipCount = 0;
			
			//maxZ reprsents the cameraPlane - so anything past that will be behind the camera
			//minZ represents a boundry by which points further back than it should be checked in screen coordinates
			
			for(int i = 0; i < elements.size(); i++){
				pos = elements[i]->pos;
				
				if( pos.z < minZ ){
					float angleX = (pos.x-camCenterX) / (camDist + pos.z);
					pos.x = camCenterX + atan(angleX) * camDist;
					
					float angleY = atan( (pos.y-camCenterY) / (camDist + pos.z ) );
					pos.y = camCenterY + atan(angleY) * camDist;
				}
				
				if( pos.x+elements[i]->maxBoundsX < minX || pos.y+elements[i]->maxBoundsY  < minY || pos.x+elements[i]->minBoundsX > maxX || pos.y+elements[i]->minBoundsY > maxY || pos.z > (maxZ+camDist) ){
					skipCount++;
					continue;
				}
				elements[i]->draw();
			}
			
			numSkipped  = skipCount;
			numDrawn	= elements.size() - numSkipped;
			
			//printf("skipping %i objects \n", skipCount);
			
		}
		
		void render(){
			for(int i = 0; i < elements.size(); i++){
				elements[i]->draw();
			}
		}
		
		vector <baseElement *> elements;
		int numDrawn;
		int numSkipped;
};


