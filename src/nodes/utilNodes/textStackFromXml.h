/*
 *  textStackFromXml.h
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
#include "ofxXmlSettings.h"
#include "textStackUtils.h"

class textStackFromXml{
	public:
	
	static vector<riseFallString> loadFromXml(string xmlPath, string sectionName){
	
			vector<riseFallString> vec;
			
			vector <string> riseStr;
			vector <string> fallStr;
			
			ofxXmlSettings xml;
			if( xml.loadFile(xmlPath) ){
				
				if( xml.pushTag(sectionName, 0) ){
				
					int numTags = xml.getNumTags("riseLine");
					for(int i = 0; i < numTags; i++){
						riseStr.push_back(xml.getValue("riseLine", "", i) );
					}
					
					numTags = xml.getNumTags("fallLine");
					for(int i = 0; i < numTags; i++){
						fallStr.push_back(xml.getValue("fallLine", "", i) );
					}
										
					xml.popTag();
				}
			}
			
			int numTotal = MAX(riseStr.size(), fallStr.size());
			
			if( numTotal > 0 ){
				string r, f;
				for(int i = 0; i < numTotal; i++){
					if( i < riseStr.size() ){
						r = riseStr[i];
					}else{
						r = "";
					}
					if( i < fallStr.size() ){
						f = fallStr[i];
					}else{
						f = "";
					}
					
					vec.push_back(riseFallString(r, f));
				}
			}
		
		return vec;
	}

};