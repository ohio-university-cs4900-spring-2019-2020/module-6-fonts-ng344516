#include "NetModuleMessage.h"
#ifdef AFTR_CONFIG_USE_BOOST

#include <iostream>
#include <sstream>
#include <string>
#include "AftrManagers.h"
#include "WO.h"
#include "GLView.h"
#include "WorldContainer.h"
#include "GLViewFonts.h"


using namespace Aftr;

NetMsgMacroDefinition(NetModuleMessage);

NetModuleMessage::NetModuleMessage() {

}

NetModuleMessage::~NetModuleMessage() {

}

bool NetModuleMessage::toStream(NetMessengerStreamBuffer& os) const {
	os << message;
	return true;
}

bool NetModuleMessage::fromStream(NetMessengerStreamBuffer& is) {
	is >> message;
	return true;
}

void NetModuleMessage::onMessageArrived() {
	((GLViewFonts*)ManagerGLView::getGLView())->updateMessage(message);
	std::cout << this->toString() << std::endl;

}

std::string NetModuleMessage::toString() const {
	std::stringstream ss;
	ss << message;
	return ss.str();
}

#endif
