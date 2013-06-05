/*
 *  textStackUtils.h
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

class colorCombo{
	public:
	
	string bgName;
	string fgName;
	
	colorCombo(string bg, string fg){
		bgName = bg;
		fgName = fg;
	} 
	
};

class riseFallString{
	public:
		riseFallString(){}
		riseFallString(string riseStr, string fallStr){
			r = riseStr;
			f = fallStr;
		}
		
	string r;
	string f;
};


typedef enum{
	TEXT_STACK_INACTIVE,
	TEXT_STACK_ACTIVATING,
	TEXT_STACK_ACTIVE,
	TEXT_STACK_DEACTIVATING,
	TEXT_STACK_DEACTIVE
}stackTextState;