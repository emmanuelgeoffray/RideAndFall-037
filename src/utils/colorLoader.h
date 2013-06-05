/*
 *  colorLoader.h
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
#include "ofxXmlSettings.h"
#include "ofMain.h"

class simpleColor{

	public:

	simpleColor(){
		r = 255.0;
		g = 255.0;
		b = 255.0;
		a = 255.0;
		colorName = "NONE";
	}	
	
	void set( float R, float G, float B){
		r = R;
		g = G;
		b = B;
	}

	void set( float R, float G, float B, float A){
		r = R;
		g = G;
		b = B;
		a = A;
	}

	void modulateRGB(float pct){
		r *= pct;
		g *= pct;
		b *= pct;
	}
	
	void modulateRGBA(float pct){
		r *= pct;
		g *= pct;
		b *= pct;
		a *= pct;
	}

	void modulateAlpha(float pct){
		a *= pct;
	}

	void set( float V ){
		r = V;
		g = V;
		b = V;
	}
	
	void setOpacity(float A){
		a = A;
	}
			
	string colorName;
	float r, g, b, a;
	
};

class colorGroup{
	public:
	
	simpleColor getColor(string colorName){
		for(int i = 0; i < colors.size(); i++){
			if( colorName == colors[i].colorName ){
				return colors[i];
			}	
		}
		return simpleColor();
	}
	
	bool setColor(string colorName){
		for(int i = 0; i < colors.size(); i++){
			if( colorName == colors[i].colorName ){
				ofSetColor( colors[i].r, colors[i].g, colors[i].b, colors[i].a);
				return true;
			}	
		}
		return false;
	}
	
	vector <simpleColor> colors;
};

class colorLoader{

	public:
	
		static void loadColors(string xmlfile, vector <simpleColor> &colors){
			ofxXmlSettings xml;
			bool success = xml.loadFile(xmlfile);
			
			if(success){
				int numColors = xml.getNumTags("color");
				if( numColors > 0 ){
					
					for(int i = 0; i < numColors; i++){
						if( xml.pushTag("color", i) ){
							
							colors.push_back(simpleColor());
							
							colors.back().colorName		= xml.getValue("name", "color-"+ofToString(i));
							colors.back().r				= xml.getValue("r", 255.0);
							colors.back().g				= xml.getValue("g", 255.0);
							colors.back().b				= xml.getValue("b", 255.0);
							colors.back().a				= xml.getValue("a", 255.0);
							
							xml.popTag();
						}
					}
				}
			}
			
			//xml.loadFile();
		}
		
};