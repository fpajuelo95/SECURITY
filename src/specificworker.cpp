

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
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{

}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{


	current = 0;
	innermodel=new InnerModel("/home/robocomp/robocomp/files/innermodel/simpleworld.xml");	
	timer.start(Period);	

	return true;
}

void SpecificWorker::compute()
{
  
try
  { 
    RoboCompDifferentialRobot::TBaseState bState;
    differentialrobot_proxy->getBaseState(bState);
    innermodel->updateTransformValues("base", bState.x, 0, bState.z, 0, bState.alpha, 0);

  
  switch(st)
   {
    case State::SEARCH:
	qDebug()<< "SEARCH";
	if(t.getid()==current){
	  gotopoint_proxy->stop();
	  gotopoint_proxy->go("base",t.getPose().x(),t.getPose().z(),0);

	  st=State::WAIT;
	}else
	  gotopoint_proxy->turn(0.3);

	break;
      
    case State::WAIT:
	qDebug()<< "WAIT";
	if(gotopoint_proxy->atTarget()==true)
	{
	  gotopoint_proxy->stop();
	  st=State::SEARCH;
	  current = (current+1)%4;
	}

	break;
// 	try
// 	{
// 		camera_proxy->getYImage(0,img, cState, bState);
// 		memcpy(image_gray.data, &img[0], m_width*m_height*sizeof(uchar));
// 		searchTags(image_gray);
// 	}
// 	catch(const Ice::Exception &e)
// 	{
// 		std::cout << "Error reading from Camera" << e << std::endl;
// 	}
  
   }
  }
  catch(const Ice::Exception &e)
  { std::cout << e << std::endl;};
}

void SpecificWorker::newAprilTag(const tagsList& tags)
{
  qDebug()<< tags[0].id;
  t.init(innermodel);
  t.copy (tags[0].tx,tags[0].tz,tags[0].id);
  
}








