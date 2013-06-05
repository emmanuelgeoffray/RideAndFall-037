/*
 *  advadvImageSequence.cpp
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

#include "advImageSequence.h"

//------------------------------------------------
advImageSequence::advImageSequence(){
    num = 0;
    seq.clear();
	whichSection = 0;
}

//------------------------------------------------
advImageSequence::~advImageSequence(){
    seq.clear();
    dirList.reset();
}

//------------------------------------------------
bool advImageSequence::loadSeq(string directory, string ext){
    seq.clear();
    dirList.reset();
	sections.clear();
    dirList.allowExt(ext);
    num = dirList.listDir(directory);
    if( num <= 0 )return false;
	
	if( directory[directory.length()-1] != '/'){
		directory += "/";
	}
	
	if( !xml.loadFile(directory + "_info.xml") ){
		return false;
	}

	int numTags = xml.getNumTags("section");
	for(int i = 0; i < numTags; i++){
		string secName = xml.getValue("section:name", "none", i);
		int startFrame = xml.getValue("section:start", 0, i);
		int endFrame   = xml.getValue("section:end", 0, i);
		bool bLoop	   = xml.getValue("section:loop", false, i);
		
		printf("name: %s = %i %i \n", secName.c_str(), startFrame, endFrame);
		
		sections.push_back( seqSection( secName, startFrame, endFrame, bLoop ) );
	}
	
	if( numTags > 0)currentSection = sections[0];

    seq.assign(num, imageTextureLoad());

    for(int i = 0; i < num; i++){
		seq[i].loadImage(dirList.getPath(i));
    }

	whichSection = 0;

    return true;
}

bool advImageSequence::setSection(string sectionName){
	for(int i = 0; i < sections.size(); i++){
		if( sections[i].name == sectionName){
			currentSection = sections[i];
			whichSection = i;
			return true;
		}
	}
	return false;
}

bool advImageSequence::setSection(int _whichSection){
	if( _whichSection >= 0 && _whichSection < sections.size() ){
		currentSection = sections[_whichSection];
		whichSection = _whichSection;
		return true;
	}
	return false;
}

//---------------------------------------------
int advImageSequence::getNumImages(){
   return seq.size();
}

//----------------------------------------------
imageTextureLoad * advImageSequence::getPct(float pct){
    pct = ofClamp(pct, 0, 1);

    if( seq.size() > 0){
        float numImages = seq.size();
		
		int which = ofMap(pct, 0, 1, currentSection.startFrame, currentSection.endFrame);
		ofClamp(which, 0, MAX(0,numImages-1) );
		
        //printf("returning %i \n", which);
        return &seq[which];
    }
    return &empty;
}


//----------------------------------------------
imageTextureLoad * advImageSequence::getFrame(int which){
    if( which >= 0 && which < seq.size()){
        //printf("returning %i \n", which);
        return &seq[which];
    }
    return &empty;
}

