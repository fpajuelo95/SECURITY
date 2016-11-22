/*
 *    Copyright (C) 2016 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
       \brief
       @author authorname
*/







#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>

class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);
	void newAprilTag(const tagsList &tags);


public slots:
	void compute(); 

private:
    enum class State {WAIT, SEARCH};

    struct Tag
  {
    QMutex m;
    QVec pose;
    QVec poseA;
    int id = 0;
    InnerModel* inner;
    
    int getid()
    {
      QMutexLocker lm(&m);
      return id;
    }
    
    void init(InnerModel* innerm)
    {
      inner = innerm;
    }
    
    void copy(float x, float z, int iden)
    {
      QMutexLocker lm(&m);
      id=iden;
      pose=inner->transform("world",QVec::vec3(x,0,z),"base");
      

    }
    QVec getPose()
    {
      QMutexLocker lm(&m);
      return pose;
    }
    
     bool cambiar()
    {
     QMutexLocker ml (&m);
     if((pose - poseA).norm2() >100){
	poseA = pose;
	return true;
     }
     return false;
     
   }
  };
    InnerModel* innermodel;
    int current = 0;
    State st = State::SEARCH;
    Tag t;


	
};

#endif

