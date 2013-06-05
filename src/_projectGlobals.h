/*
 *  _projectGlobals.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 02/02/2010.
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

#include "colorLoader.h"
#include "easySoundPlayer.h"
#include "myPersonElement.h"
#include "typeLoader.h"
#include "imageSequenceLoader.h"

extern easySoundPlayer globalSnd;
extern colorGroup globalColors;
extern imageSequenceLoader globalSeqLoader;
extern myPersonElement myPerson;
extern typeLoader globalType;
extern float riseFallPct;
extern bool bRise;
extern ofPoint camPos;
extern float durationSameDirection;
extern float percentOver;
extern ofPoint personScreenPos;
extern bool bLoadFullTextures;
extern bool bLoadRegTextures;

//
//<name>red</name>
//<name>liteBlue</name>
//<name>midBlue</name>
//<name>darkBlue</name>
//<name>darkFall</name>
//<name>lightRise</name>