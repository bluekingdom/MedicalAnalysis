
#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <functional>

#include "WebServerManager.h"

namespace SYY {
namespace Web {

	using boost::asio::ip::tcp;

	WebServerManager::WebServerManager()
		:m_pAcceptor(nullptr)
	{ }

	void process_client(std::shared_ptr<tcp::socket> client)
	{
		time_t now = time(0);
		std::shared_ptr<std::string> message(new std::string(ctime(&now)));

		auto callback = [=](const boost::system::error_code& err, size_t size)
		{
			if ((int)size == message->length())
				std::cout << "write completed" << std::endl;
		};

		client->async_send(boost::asio::buffer(*message), callback);
	}

	typedef std::function<void(const boost::system::error_code&)> accept_callback;
	void start_accept(tcp::acceptor& server)
	{
		std::shared_ptr<tcp::socket> client(new tcp::socket(server.get_io_service()));
		accept_callback callback = [&server, client](const boost::system::error_code& error)
		{
			if (!error)
				process_client(client);

			start_accept(server);
		};

		server.async_accept(*client, callback);
	}

	SYY::ErrorCode WebServerManager::Init()
	{
		m_nPort = 3200;

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode WebServerManager::Execute()
	{
		m_pAcceptor = new tcp::acceptor(m_ioService, tcp::endpoint(tcp::v4(), m_nPort));

		start_accept(*m_pAcceptor);
		m_ioService.run();

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode WebServerManager::Release()
	{
		if (m_pAcceptor)
		{
			delete m_pAcceptor;
		}
		m_pAcceptor = nullptr;

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode WebServerManager::AddProcessor(ProcessorType type)
	{

		return SYY_NO_ERROR;
	}

	SYY::ErrorCode WebServerManager::DelProcessor(ProcessorType type)
	{

		return SYY_NO_ERROR;
	}


}
}
