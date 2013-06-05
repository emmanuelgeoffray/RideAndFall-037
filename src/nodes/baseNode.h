/*
 *  baseNode.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 29/01/2010.
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
#include "baseElement.h"
#include "_projectGlobals.h"

typedef enum{
	NODE_INACTIVE,
	NODE_ACTIVATING,
	NODE_ACTIVE,
	NODE_ACTIVATING_RISE,
	NODE_RISE,
	NODE_ACTIVATING_FALL,	
	NODE_FALL,
	NODE_DEACTIVATING,
	NODE_DEACTIVE
}nodeState;

typedef enum{
	NODE_UNKILLABLE,
	NODE_CHECKKIDS_KILL,
	NODE_KILLABLE
}nodeKillState;

typedef struct{
	string nodeName;
	nodeState nodeCommand;
}eventStruct;

typedef struct{
	string targetName;
	nodeState targetNodeCommand;
	nodeState triggerWhenAtState;
}nodeSendEvents;

extern ofEvent <eventStruct> globalEventStruct;

class baseNode : public baseElement{

	public:
	
		baseNode(){
			state		= NODE_INACTIVE;
			killState	= NODE_UNKILLABLE;
		}
		
		virtual void setupName(string nameStr){
			nodeName = nameStr;
			ofAddListener(globalEventStruct,this,&baseNode::eventsIn);
		}
		
		virtual ~baseNode(){
		
		}
		
		static string nodeStateToStr(nodeState stateIn){
			switch(stateIn){
				case NODE_INACTIVE:
					return "NODE_INACTIVE";
				case NODE_ACTIVATING:
					return "NODE_ACTIVATING";
				case NODE_ACTIVE:
					return "NODE_ACTIVE";
				case NODE_DEACTIVATING:
					return "NODE_DEACTIVATING";
				case NODE_DEACTIVE:
					return "NODE_DEACTIVE";
				default:
					return "NO_STATE";
				break;
			}
		}
		
		//----------------
		virtual void checkState(float targetX, float targetY, float targetZ){
			
		}

		//----------------
		virtual bool shouldPersonWait(){
			return false;
		}
		
		//This should be implemented with more complexity in the derrived nodes
		//ie if a node has particles it should check that they are not on screen etc
		//----------------
		virtual bool isKillable(){
			return (killState != NODE_UNKILLABLE);
		}

		//----------------
		virtual void setState(nodeState stateIn){
			if( state != stateIn){
				state = stateIn;
				fireFirstTimeStateReached(state);
			}
		}
				
		//----------------
		virtual void fireFirstTimeStateReached(nodeState stateIn){
			state = stateIn;
			processNodeEvents();
		}

		//----------------		
		virtual void processNodeEvents(){
			for(int i = 0; i < nodeEvents.size(); i++){
				if( state == nodeEvents[i].triggerWhenAtState ){
					eventStruct tmp;
					tmp.nodeName = nodeEvents[i].targetName;
					tmp.nodeCommand = nodeEvents[i].targetNodeCommand;
					ofNotifyEvent(globalEventStruct, tmp, this);
				}
			}
		}
		
		//----------------		
		virtual void addNodeEvent( nodeState triggerWhenAtState, string targetName, nodeState targetState ){
			nodeSendEvents tmp;
			tmp.triggerWhenAtState	=	triggerWhenAtState;
			tmp.targetName			=	targetName;
			tmp.targetNodeCommand	=	targetState;
				
			nodeEvents.push_back( tmp );
		}
		
		//----------------		
		virtual void eventsIn(eventStruct & data){
			printf("event %s - %s\n", data.nodeName.c_str(), baseNode::nodeStateToStr(data.nodeCommand).c_str());
			if( data.nodeName == nodeName ){
				setState(data.nodeCommand);
			}
		}

		//----------------		
		//If you want to return this element (So that all drawing happens inside this class's draw() do this
		//otherwise return your elements vector so that each element is depth sorted and drawn seperately
		virtual vector <baseElement *> getElements(){
			vector <baseElement *> elementsList;
			elementsList.push_back(this);
			return elementsList;
		} 

		//----------------		
		nodeState getState(){
			return state;
		}

		//----------------		
		virtual void update(){
			
		}
		
		//----------------		
		virtual void draw(){
			
		}			
				
		string nodeName;
		nodeState state;
		nodeKillState killState;
		
		vector <nodeSendEvents> nodeEvents;
		
		vector <baseElement *> elements;
		
};