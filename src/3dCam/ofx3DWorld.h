/*
 *  ofx3DWorld.h
 *  vectorMathExample
 *
 *  Created by theo on 16/12/2009.
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

class ofx3DWorld{
	public:
	
	ofx3DWorld(){
		origin = ofVec3f(0,0,0);
		upVec.set(0, 1, 0);
		scale.set(1.0, 1.0, 1.0);
	}

	void setOrigin(ofPoint originRelativeToOF){
		origin = originRelativeToOF;
	}
	
	void setUpVector(ofPoint up){
		upVec = up;
	}
	
	void begin(){
		glPushMatrix();
		glEnable(GL_DEPTH_TEST);
	}
	
	void setScale( float x, float y, float z ){
		scale.set(x, y, z);
	}

	void ourCameraSetup(float eyeX, float eyeY, float eyeZ,  float w, float h, float fov){

		float halfFov, theTan, screenFov, aspect;
		screenFov 		= fov;

		halfFov 		= PI * screenFov / 360.0;
		theTan 			= tanf(halfFov);
		float dist 		= eyeY / theTan;
		float nearDist 	= dist / 10.0;	// near / far clip plane
		float farDist 	= dist * 10000.0;
		aspect 			= (float)w/(float)h;

		camDist = dist;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(screenFov, aspect, nearDist, farDist);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		gluLookAt(eyeX, eyeY, dist+eyeZ, eyeX, eyeY, 0.0, upVec.x, upVec.y, upVec.z);
		

		//glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
		glTranslatef(origin.x, origin.y, origin.z);       // shift origin up to upper-left corner.
		
		glRotatef(camRot.x, 1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
		glRotatef(camRot.y, 0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)		
		glRotatef(camRot.z, 0.0,0.0,1.0);  //rotate our camera on the z-axis ff
		
		//glScalef(1.0/scale.x, 1.0/scale.y, 1.0/scale.z);
		glTranslatef(-camPos.x, -camPos.y, -camPos.z);
		
	}
	
	void end(){
		glDisable(GL_DEPTH_TEST);
		glPopMatrix();
	}
	
	void drawDebugGrid(){
		glBegin(GL_LINES);
		
		float dist = 6000;
		int numSteps = 100;
				
		glLineWidth(4.0);
		glColor3f(0.0, 0.0, 0.0);
		
		glVertex3f(-dist/2, 0, 0);
		glVertex3f(dist/2, 0, 0);

		glVertex3f(0, -dist/2, 0);
		glVertex3f(0, dist/2, 0);
		
		glVertex3f(0,0,-dist/2);
		glVertex3f(0,0,dist/2);
		
		glLineWidth(1.0);
		glColor4f(0.3, 0.3, 0.3, 0.2);
//		

		float stepDist = (float)dist / (float)numSteps;

		for(int i = -numSteps/2; i < numSteps/2; i++){
			glVertex3f(stepDist * i, 0, -dist/2);
			glVertex3f(stepDist * i, 0, dist/2);
		}

		for(int i = -numSteps/2; i < numSteps/2; i++){
			glVertex3f(-dist/2, 0, stepDist * i);
			glVertex3f(dist/2, 0, stepDist * i);
		}

//		
		
		glEnd();
	}

	float camDist;
	ofPoint cameraLook;
	ofPoint scale;
	ofPoint camPos;
	ofPoint camRot;

	ofPoint origin;
	ofPoint upVec;

};
