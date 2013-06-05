/*
 *  seqManager.h
 *  openFrameworks
 *
 *  Created by theo on 9/19/09.
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

#include "advImageSequence.h"

class seqManager{

	public:
		seqManager(){
			frameRate	= 30.0;
			seq			= NULL;
			pct			= 0.0;
			bRev		= false;
		}
		
		void setup( advImageSequence * imgSeq, float fps ){
			seq = imgSeq;
			pct = 0.0;
			frameRate = fps;
			
			if( imgSeq->getNumImages() ){
				width = imgSeq->getFrame(0)->width;
				height = imgSeq->getFrame(0)->height;
			}
			
			seq->setSection(0);
			currentSection = seq->currentSection;
			whichSection = 0;
			frameCounter = currentSection.startFrame;
			timeSince = ofGetElapsedTimef();
			bPlay = false;
			bDone = false;
			numSections = seq->sections.size();
		}
		
		void setAnchorPercent(float xPct, float yPct){
			anchor.set(xPct, yPct);
		}
		
		bool lastSection(){
			return (numSections > 0 && (whichSection == numSections-1) );
		}
		
		//CHECK ME FOR REVERSE!!!!!!
		float getPercent(){
			if(  currentSection.endFrame == currentSection.startFrame ) return 0;
			
			float percent = 0.0;
			percent = (float)(float) ( frameCounter - currentSection.startFrame) / fabs ( (float)( currentSection.endFrame - currentSection.startFrame) );
			percent = ofClamp(percent, 0, 1);
			
			if(bRev) percent = 1.0 - percent;
						
			return percent;
		}
		
		void play(){
			bPlay = true;
		}
		
		void stop(){
			bPlay = false;
		}
		
		void setSection(string sectionName){
			if( seq->setSection(sectionName) ){
				whichSection = seq->whichSection;			
				startNewSection();				
			}
			
		}
		
		void next(){
			if( whichSection < numSections-1 )whichSection++;
			else return;
			
			startNewSection();
		}
		
		void prev(){
			if( whichSection > 0 && numSections > 0)numSections--;
			else return;
			
			startNewSection();
		}
		
		void restart(){
			if( numSections > 0)whichSection = 0;
			else return;
			
			startNewSection();
		}

		void restartAndWait(){
			restart();
			stop();
		}		
		void startNewSection(){
			currentSection = seq->sections[whichSection];
			
			frameCounter = currentSection.startFrame;
			
			if( currentSection.endFrame < currentSection.startFrame ){
				bRev = true;
			}else{
				bRev = false;
			}
			
			timeSince = ofGetElapsedTimef();
			bDone = false;
		}
		
		bool isDone(){
			return bDone;
		}
		
		void setFrameRate(float fps){
			frameRate = fps;
		}
		
		void update(){
			if(seq == NULL)return;
			
			bool bUpdate = false;
			
			if( bRev ){
				if( frameCounter > currentSection.endFrame )bUpdate = true;
			}else{
				if( frameCounter < currentSection.endFrame )bUpdate = true;
			}
			
			if( bPlay && bUpdate ){
				
				float timeDelta = ofGetElapsedTimef() - timeSince;
				
				if( bRev )frameCounter -= timeDelta * frameRate;
				else frameCounter += timeDelta * frameRate;
				
				//printf("frameCounter %f\n", frameCounter);
				
				if( ( bRev && frameCounter <= currentSection.endFrame) || ( !bRev && frameCounter >= currentSection.endFrame ) ){
					if( currentSection.bLoop ){
						frameCounter = currentSection.startFrame;
					}else{
						frameCounter = currentSection.endFrame;
					}
					bDone = true;
				}
						
			}
			timeSince = ofGetElapsedTimef();
		}
		
		imageTextureLoad * getCurrentFrame(){
			if( seq == NULL )return NULL;
			imageTextureLoad * img = seq->getFrame((int)frameCounter);
			img->setAnchorPercent(anchor.x, anchor.y);
			return img;
		}

		void draw(float x, float y, float w, float h){
			if(seq == NULL)return;
			imageTextureLoad * img = seq->getFrame((int)frameCounter);
			img->setAnchorPercent(anchor.x, anchor.y);
			img->draw(x, y, w, h);
		}
		
		void draw(float x, float y){
			if(seq == NULL)return;
			imageTextureLoad * img = seq->getFrame((int)frameCounter);
			img->setAnchorPercent(anchor.x, anchor.y);
			img->draw(x, y);
		}
		
		ofPoint anchor;
		
		float frameRate;
		seqSection currentSection;
		
		float frameCounter;
		float pct;
		float timeSince;
		advImageSequence * seq;
		
		float width, height;
		
		int whichSection;
		int numSections;
		
		bool bDone;
		bool bRev;

		bool bEndThisLoop;
		bool bPlay;
		
};