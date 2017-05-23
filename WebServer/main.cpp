#include <iostream>
#include "WebServerManager.h"

#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <boost/asio.hpp>

using namespace SYY;
using namespace SYY::Web;

using boost::asio::ip::tcp;
using namespace std;

int test_wsMgr(int argc, char** argv)
{
	WebServerManager wsMgr;
	ErrorCode code = SYY_NO_ERROR;

	code = wsMgr.Init();
	if (code != SYY_NO_ERROR)
	{
		std::cout << "init error\n";
		return -1;
	}

	wsMgr.Execute();

	wsMgr.Release();

	return 0;
}

void process_client(shared_ptr<tcp::socket> client)
{
	time_t now = time(0);
	shared_ptr<string> message(new string(ctime(&now)));

	auto callback = [=](const boost::system::error_code& err, size_t size)
	{
		if ((int)size == message->length())
			cout << "write completed" << endl;
	};

	client->async_send(boost::asio::buffer(*message), callback);
}

typedef function<void(const boost::system::error_code&)> accept_callback;
void start_accept(tcp::acceptor& server)
{
	shared_ptr<tcp::socket> client(new tcp::socket(server.get_io_service()));
	accept_callback callback = [&server, client](const boost::system::error_code& error)
	{
		if (!error)
			process_client(client);

		start_accept(server);
	};

	server.async_accept(*client, callback);
}

int test_server(int argc, char** argv)
{
	try
	{
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3200));
		start_accept(acceptor);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

int main(int argc, char** argv)
{
	test_wsMgr(argc, argv);
	//test_server(argc, argv);

	return 0;
}