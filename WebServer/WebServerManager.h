#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <map>
#include "MedicalAnalysisSDK.h"

namespace SYY {
namespace Web {

	enum ProcessorType
	{
		BUAnalysis = 0,			// b³¬Í¼Æ¬·ÖÎö
	};

	class WebServerProcessor {
	public:
		virtual ErrorCode Process() = 0;
		virtual ProcessorType GetType() = 0;
	};

	class WebServerManager {
	public:
		ErrorCode Init();
		ErrorCode AddProcessor(ProcessorType type);
		ErrorCode DelProcessor(ProcessorType type);
		ErrorCode Execute();
		ErrorCode Release();

	public:
		WebServerManager();

	private:
		std::map<ProcessorType, WebServerProcessor*> m_tProcessors;
		boost::asio::io_service m_ioService;
		boost::asio::ip::tcp::acceptor *m_pAcceptor;
		int m_nPort;
	};

}
}

