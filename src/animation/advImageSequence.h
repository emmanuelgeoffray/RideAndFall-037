/*
 *  advImageSequence.h
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

#include "ofMain.h"
#include "ofxXmlSettings.h"

class seqSection{
	public:
		seqSection(){
			name = "";
			startFrame = 0;
			endFrame = 0;
			bLoop = false;
		}
		seqSection(string _name, int _start, int _end, bool _bLoop){
			name		= _name;
			startFrame	= _start;
			endFrame	= _end;
			bLoop		= _bLoop;
		}
		seqSection( const seqSection &in ){
			name		= in.name;
			startFrame	= in.startFrame;
			endFrame	= in.endFrame;
			bLoop		= in.bLoop;
		}
		
		bool bLoop;
		string name;
		int startFrame;
		int endFrame;
};

class imageTextureLoad : public ofImage{
	public:
//		//----------------------------------------------------------
//		bool loadImage(string fileName){
//			bool bLoadedOk = false;
//			bLoadedOk = loadImageIntoPixels(fileName, myPixels);
//		}
//		
//		//----------------------------------------------------------		
//		void drawAndLoad(float x, float y){
//			imageTextureLoad::drawAndLoad(x, y, width, height);
//		}
//
//		//----------------------------------------------------------		
//		void drawAndLoad(float x, float y, float w, float h){
//		
//			if( tex.getWidth() == 0.0 && myPixels.bAllocated){
//				tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
//				tex.loadData(myPixels.pixels, myPixels.width, myPixels.height, myPixels.glDataType);
//				width	= myPixels.width;
//				height	= myPixels.height;
//				bpp		= myPixels.bitsPerPixel;
//				type	= myPixels.ofImageType;
//				bUseTexture = true;
//			}
//			
//			ofImage::draw(x, y, w, h);
//		}
};

class advImageSequence{
    public:

		//------------------------------------------------
		advImageSequence();
		virtual ~advImageSequence();

		bool loadSeq(string directory, string ext);
		int getNumImages();

		imageTextureLoad * getPct(float pct);
		imageTextureLoad * getFrame(int which);
		
		bool setSection(string sectionName);
		bool setSection(int whichSection);

		ofDirectory dirList;
		vector <imageTextureLoad> seq;
		ofxXmlSettings xml;
		
		vector <seqSection> sections;
		seqSection currentSection;

		int whichSection;

		imageTextureLoad empty;
		int num;
	
};
