#pragma once

#include "NetMsg.h"
#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr {

	class NetModuleMessage : public NetMsg {
	public:
		NetMsgMacroDeclaration(NetModuleMessage);
		NetModuleMessage();
		virtual ~NetModuleMessage();
		virtual bool toStream(NetMessengerStreamBuffer& os) const;
		virtual bool fromStream(NetMessengerStreamBuffer& is);
		virtual void onMessageArrived();
		virtual std::string toString() const;
		
		std::string message;
	};

} // Namespace Aftr

#endif
