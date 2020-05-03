#include "WOWayPointAbstract.h"
#include "FontsWayPoints.h"
#include "GLViewFonts.h"
#include "NetModuleMessage.h"
#include <vector>
#include <iostream>

namespace Aftr
{

WOWP1* WOWP1::New(const WayPointParametersBase& params, float radius )
{
   WOWP1* ptr = new WOWP1(params, radius);
   ptr->onCreate();
   return ptr;
}

WOWP1::WOWP1(const WayPointParametersBase& params, float radius
             ) : WOWayPointSpherical( params, radius ), IFace( this )
{
}

WOWP1::~WOWP1()
{
}

void WOWP1::onTrigger()
{
	this->timesTriggered += 1;
	if (timesTriggered % 10 == 1 && timesTriggered % 100 != 11) {
		std::cout << "WOWP1 waypoint Triggered for the " << timesTriggered << "st time!" << std::endl << std::endl;
	}
	else if (timesTriggered % 10 == 2 && timesTriggered % 100 != 12) {
		std::cout << "WOWP1 waypoint Triggered for the " << timesTriggered << "nd time!" << std::endl << std::endl;
	}
	else if (timesTriggered % 10 == 3 && timesTriggered % 100 != 13) {
		std::cout << "WOWP1 waypoint Triggered for the " << timesTriggered << "rd time!" << std::endl << std::endl;
	}
	else {
		std::cout << "WOWP1 waypoint Triggered for the " << timesTriggered << "th time!" << std::endl << std::endl;
	}
   float x = rand() % 100;
   if ((int)x % 2 == 0) {
	   x *= -1;
   }
   float y = rand() % 100;
   if ((int)y % 2 == 0) {
	   y *= -1;
   }
   Vector newPosition = Vector(x, y, rand() % 50);
   this->setPosition(newPosition);

   int times = this->timesTriggered;
   std::string message = "Total Targets Hit: " + std::to_string(times);
   ((GLViewFonts*)ManagerGLView::getGLView())->updateMessage(message);
   NetModuleMessage netMessage;
   netMessage.message = message;
   ((GLViewFonts*)ManagerGLView::getGLView())->getClient()->sendNetMsgSynchronousTCP(netMessage);

}

} //namespace Aftr