#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include "Manchester.h"

#define IP_PROTOCOL boost::asio::ip::tcp::v4()
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 555

#define WIDTH 800
#define HEIGHT 600
#define RESIZEABLE true

using boost::asio::ip::tcp;

bool runningThreads = true;
bool click_send = false;
std::string message;

std::string targetHost = DEFAULT_HOST;
std::string targetPort = std::to_string(DEFAULT_PORT);
int listenPort = DEFAULT_PORT;

nana::form* formaggio;
// Ponteiros para textos da interface
nana::textbox* sent_binary;
nana::textbox* sent_encoded;
nana::textbox* recv_encoded;
nana::textbox* recv_binary;
nana::textbox* recv_decoded;

tcp::socket* serverSocket;

// Função antiga.
/*
void sendMessage(std::string message) {
	try {
		sent_binary->reset(binaryString(message));

		char* encoded = manchesterEncode(message);
		int length = message.length() + 1;

		sent_encoded->reset(binaryString(encoded, 2 * length));

		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);

		// Encontra o(s) endpoint(s) correspondente(s) ao host e porta especificados.
		tcp::resolver::query query(targetHost, targetPort);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		// Cria o socket do cliente e conecta no primeiro endpoint que funcionar
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		socket.send(boost::asio::buffer(encoded, 2 * length));

		delete[] encoded;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
*/

// Função antiga.
/*
void client() {
	while (runningThreads) {
		if (click_send) {
			sendMessage(message);
			click_send = false;
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
	std::cout << "ended client" << std::endl;
}
*/

// Função antiga.
/*
void server() {
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), listenPort));

		while (true) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			boost::asio::streambuf buffer;
			boost::system::error_code error;

			while (boost::asio::read(socket, buffer, error)) {
				size_t bytes = boost::asio::buffer_size(buffer.data());
				const char* encoded = boost::asio::buffer_cast<const char*>(buffer.data());
				recv_encoded->reset(binaryString(encoded, bytes));

				char* decoded = manchesterDecode(encoded, bytes);
				recv_binary->reset(binaryString(decoded));
				recv_decoded->reset(decoded);

				if (error) {
					std::cout << "status: " << error.message() << std::endl;
					break;
				}

				delete[] decoded;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	std::cout << "ended server" << std::endl;
}
*/

void server(boost::asio::io_service& io_service) {
	try {
		tcp::acceptor acceptor(io_service, tcp::endpoint(IP_PROTOCOL, listenPort));

		while (runningThreads) {
			tcp::socket socket(io_service);
			serverSocket = &socket;

			acceptor.accept(socket);

			boost::asio::streambuf buffer;
			boost::system::error_code error;

			while (boost::asio::read(socket, buffer, error)) {
				size_t bytes = boost::asio::buffer_size(buffer.data());
				const char* encoded = boost::asio::buffer_cast<const char*>(buffer.data());
				recv_encoded->reset(binaryString(encoded, bytes));

				char* decoded = manchesterDecode(encoded, bytes);
				recv_binary->reset(binaryString(decoded));
				recv_decoded->reset(decoded);

				if (error) break; // Sai do loop quando encontra um EOF.

				delete[] decoded;
			}
		}
	}
	catch (std::exception& e) {
		// Esse catch está em branco intencioalmente.
		// std::cerr << e.what() << std::endl;
	}
}

// Funções para servidor assíncrono (eu buguei na parte do async_read).
/*
void handleRead(const boost::system::error_code& error, boost::asio::streambuf& buffer) {
	size_t bytes = boost::asio::buffer_size(buffer.data());
	const char* encoded = boost::asio::buffer_cast<const char*>(buffer.data());
	recv_encoded->reset(binaryString(encoded, bytes));

	char* decoded = manchesterDecode(encoded, bytes);
	recv_binary->reset(binaryString(decoded));
	recv_decoded->reset(decoded);

	if (error) {
		std::cout << "status: " << error.message() << std::endl;
	}

	delete[] decoded;
}

void startAccept(tcp::acceptor&);

void server2(boost::asio::io_service& io_service) {
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), PORT_NUMBER));

	startAccept(acceptor);

	io_service.run();

	std::cout << "ended server" << std::endl;
}

void handleAccept(const boost::system::error_code& error, boost::shared_ptr< tcp::socket > socket, tcp::acceptor& acceptor) {
	if (error) {
		std::cout << "Error accepting connection: " << error.message() << std::endl;
		return;
	}

	std::cout << "Doing things with socket..." << std::endl;

	// Fazer async_read aqui.

	std::cout << "Done using socket, ready for another connection." << std::endl;
	startAccept(acceptor);
}

void startAccept(tcp::acceptor& acceptor) {
	boost::shared_ptr< tcp::socket > socket(new tcp::socket(acceptor.get_io_service()));
	
	std::cout << "Waiting on connection" << std::endl;
	acceptor.async_accept(*socket, boost::bind(handleAccept, boost::asio::placeholders::error, socket, boost::ref(acceptor)));
}
*/

void client(boost::asio::io_service& io_service) {
	while (runningThreads) {
		if (click_send) {
			try {
				sent_binary->reset(binaryString(message));

				char* encoded = manchesterEncode(message);
				int length = message.length() + 1;

				sent_encoded->reset(binaryString(encoded, 2 * length));

				tcp::resolver resolver(io_service);

				// Encontra o(s) endpoint(s) correspondente(s) ao host e porta especificados.
				tcp::resolver::query query(targetHost, targetPort);
				tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

				// Cria o socket do cliente e conecta no primeiro endpoint que funcionar
				tcp::socket socket(io_service);
				boost::asio::connect(socket, endpoint_iterator);

				socket.send(boost::asio::buffer(encoded, 2 * length));

				delete[] encoded;
			}
			catch (std::exception& e) {
				nana::msgbox mb (*formaggio, "Falha no envio");
				mb << L"Não foi possível se conectar ao o host destino na porta especificada.";
				mb.show();
			}
			click_send = false;
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
}

void gui(boost::asio::io_service& io_service) {
	using namespace nana;

	form fm{ nana::rectangle(200, 200, WIDTH, HEIGHT), nana::appearance(true, true, false, false, false, false, RESIZEABLE) };
	fm.caption("Manchester Code");
	formaggio = &fm;

	label lbl_host{ fm, "Host:" };
	textbox txt_host{ fm , targetHost };
	txt_host.multi_lines(false);

	label lbl_port{ fm, "Port:" };
	textbox txt_port{ fm , targetPort };
	txt_port.multi_lines(false);

	button btn_changeport{ fm, "Alterar porta" };
	btn_changeport.events().click([=,&fm] {
		inputbox::integer port("Nova porta", listenPort, 0, 65535, 1);

		inputbox inbox(fm, "Selecione a nova porta.", "Nova porta");
		
		inbox.verify([&port] (window handle) {
			listenPort = port.value();
			return true;
		});

		if (inbox.show(port)) {
			auto p = port.value();
		}
	});

	label lbl_sent_uncoded{ fm, "Digite a mensagem aqui:" };
	textbox txt_sent_uncoded{ fm };
	txt_sent_uncoded.line_wrapped(true);

	label lbl_sent_binary{ fm, "Mensagem ASCII:" };
	textbox txt_sent_binary{ fm };
	txt_sent_binary.editable(false);
	txt_sent_binary.line_wrapped(true);
	sent_binary = &txt_sent_binary;

	label lbl_sent_encoded{ fm, "Mensagem codificada:" };
	textbox txt_sent_encoded{ fm };
	txt_sent_encoded.editable(false);
	txt_sent_encoded.line_wrapped(true);
	sent_encoded = &txt_sent_encoded;

	label lbl_recv_encoded{ fm, "Mensagem recebida codificada:" };
	textbox txt_recv_encoded{ fm };
	txt_recv_encoded.editable(false);
	txt_recv_encoded.line_wrapped(true);
	recv_encoded = &txt_recv_encoded;

	label lbl_recv_binary{ fm, "Mensagem recebida ASCII:" };
	textbox txt_recv_binary{ fm };
	txt_recv_binary.editable(false);
	txt_recv_binary.line_wrapped(true);
	recv_binary = &txt_recv_binary;

	label lbl_recv_decoded{ fm, "Mensagem recebida decodificada:" };
	textbox txt_recv_decoded{ fm };
	txt_recv_decoded.editable(false);
	txt_recv_decoded.line_wrapped(true);
	recv_decoded = &txt_recv_decoded;

	button btn_send{ fm, "Enviar" };
	btn_send.events().click([&] {
		txt_host.getline(0, targetHost);
		txt_port.getline(0, targetPort);
		txt_sent_uncoded.getline(0, message);
		click_send = true;
	});

	fm.div(
		"vert"
		"<<weight=20><weight=40 vert<><lbl_host weight=20><>><weight=250 vert<><txt_host weight=20><>><weight=20><weight=40 vert<><lbl_port weight=20><>><weight=40 vert<><txt_port weight=20><>><><weight=80 vert <><btn_changeport weight=20><>><weight=20>>" // Barra de cima
		"<weight=80%" // Corpo da janela
			"<weight=20><vert<weight=20><lbl_sent_uncoded weight=20><txt_sent_uncoded><weight=20><lbl_sent_binary weight=20><txt_sent_binary><weight=20><lbl_sent_encoded weight=20><txt_sent_encoded><weight=20>>" // Metade da esquerda
			"<weight=20><vert<weight=20><lbl_recv_encoded weight=20><txt_recv_encoded><weight=20><lbl_recv_binary weight=20><txt_recv_binary><weight=20><lbl_recv_decoded weight=20><txt_recv_decoded><weight=20>>" // Metade da direita
			"<weight=20>>"
		"<<><vert<><btn_send weight=20><>><>>" // Barra de baixo
	);

	fm["lbl_host"] << lbl_host;
	fm["txt_host"] << txt_host;
	fm["lbl_port"] << lbl_port;
	fm["txt_port"] << txt_port;
	fm["btn_changeport"] << btn_changeport;
	fm["lbl_sent_uncoded"] << lbl_sent_uncoded;
	fm["txt_sent_uncoded"] << txt_sent_uncoded;
	fm["lbl_sent_binary"] << lbl_sent_binary;
	fm["txt_sent_binary"] << txt_sent_binary;
	fm["lbl_sent_encoded"] << lbl_sent_encoded;
	fm["txt_sent_encoded"] << txt_sent_encoded;
	fm["lbl_recv_encoded"] << lbl_recv_encoded;
	fm["txt_recv_encoded"] << txt_recv_encoded;
	fm["lbl_recv_binary"] << lbl_recv_binary;
	fm["txt_recv_binary"] << txt_recv_binary;
	fm["lbl_recv_decoded"] << lbl_recv_decoded;
	fm["txt_recv_decoded"] << txt_recv_decoded;
	fm["btn_send"] << btn_send;
	fm.collocate();

	fm.show();

	exec();

	// Chuncho para terminar todas as threads
	runningThreads = false;

	char* encoded = manchesterEncode("");
	int length = message.length() + 1;

	tcp::resolver resolver(io_service);
	tcp::resolver::query query("localhost", std::to_string(listenPort));
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	socket.send(boost::asio::buffer(encoded, 2 * length));

	delete[] encoded;
}

int main() {
	boost::asio::io_service io_service;

	boost::thread clientThread(client, boost::ref(io_service));
	boost::thread serverThread(server, boost::ref(io_service));
	boost::thread guiThread(gui, boost::ref(io_service));

	clientThread.join();
	serverThread.join();
	guiThread.join();

	return 0;
}