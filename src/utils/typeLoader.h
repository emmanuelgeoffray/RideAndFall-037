/*
 *  typeLoader.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 16/02/2010.
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

typedef std::pair<string, ofTrueTypeFont> typePair;

class typeLoader{
	public:
	
	//-----------------
	void loadFont(string filename, int size, string uniqueKey){			
		fonts.insert(typePair(uniqueKey, ofTrueTypeFont()) );
		fonts[uniqueKey].loadFont(filename, size);
	}

	ofTrueTypeFont * getFontPtr(string uniqueKey){
		return &fonts[uniqueKey];
	}

	protected:
		map <string, ofTrueTypeFont> fonts;

};