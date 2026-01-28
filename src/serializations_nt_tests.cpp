#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "TestClass.h"
#include "DeserializeData.h"

int server();
int client();

int main(int argc, char **argv)
{
	// Spécificité Windowsienne
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	std::cout << "Client/server? ";
	std::string choice;
	std::cin >> choice;

	int r;
	if (choice == "s")
		r = server();
	else
		r = client();

	// Libération Windows socket
	WSACleanup();

	return r;
}

int server()
{

	// Création d'une socket
	SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockServer == INVALID_SOCKET)
	{
		// Erreur de création de socket
		std::cerr << "failed to create socket: " << WSAGetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET; //< IPv4

	// Conversion d'une IP textuelle vers la représentation sockaddr
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

	addr.sin_port = htons(47117); //< Attention, l'API socket veut du big endian

	int addrlen = sizeof(addr);
	if (bind(sockServer, reinterpret_cast<const sockaddr*>(&addr), addrlen) == SOCKET_ERROR)
	{
		std::cerr << "failed to bind socket: " << WSAGetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	if (listen(sockServer, 10) == SOCKET_ERROR)
	{
		std::cerr << "failed to listen socket: " << WSAGetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	// À partir d'ici, notre socket peut accepter des connexions
	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;

	int clientAddrSize = sizeof(clientAddr);

	std::cout << "Waiting for clients" << std::endl;

	std::vector<SOCKET> clients;

	while (true)
	{
		std::vector<WSAPOLLFD> pollFds;

		WSAPOLLFD& serverPollFd = pollFds.emplace_back();
		serverPollFd.fd = sockServer;
		serverPollFd.events = POLLIN;
		serverPollFd.revents = 0;

		for (SOCKET client : clients)
		{
			WSAPOLLFD& clientPollFd = pollFds.emplace_back();
			clientPollFd.fd = client;
			clientPollFd.events = POLLIN;
			clientPollFd.revents = 0;
		}

		if (WSAPoll(&pollFds[0], pollFds.size(), 10) > 0)
		{
			for (WSAPOLLFD& pollFd : pollFds)
			{
				if (pollFd.revents == 0)
					continue;

				if (pollFd.fd == sockServer)
				{
					SOCKET sockClient = accept(sockServer, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);

					char clientAddrTxt[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &clientAddr.sin_addr, &clientAddrTxt[0], INET_ADDRSTRLEN);
					std::cout << "Client connected from " << clientAddrTxt << ":" << ntohs(clientAddr.sin_port) << std::endl;

					clients.push_back(sockClient);
				}
				else
				{
					// char data[1024];
					std::vector<std::uint8_t> data(1024);
					int dataLength = recv(pollFd.fd, reinterpret_cast<char*>(data.data()), 1024, 0);
				    data.resize(dataLength);
					if (dataLength <= 0)
					{
						// Déconnexion
						if (dataLength == SOCKET_ERROR)
							std::cerr << "failed to receive data from socket: " << WSAGetLastError() << std::endl;

						auto it = std::find(clients.begin(), clients.end(), pollFd.fd);
						assert(it != clients.end());
						clients.erase(it);
						continue;
					}
				    
                    DeserializeData deserializeData(data);
				    TestClass testClass = TestClass::Deserialize(deserializeData);
					// std::string message(data, dataLength);
					std::cout << "received data of length: " << dataLength << std::endl;

					for (SOCKET client : clients)
					{
						if (client == pollFd.fd)
							continue;

						// Gestion et/ou envoi du message au serveur
						// if (send(client, &message[0], message.size(), 0) == SOCKET_ERROR)
						// 	std::cerr << "failed to send data: " << WSAGetLastError() << std::endl;
					}
				}
			}
		}
	}

	// Destruction d'une socket
	closesocket(sockServer);

	return EXIT_SUCCESS;
}

int client()
{
	// Création d'une socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		// Erreur de création de socket
		std::cerr << "failed to create socket: " << WSAGetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET; //< IPv4

	// Conversion d'une IP textuelle vers la représentation sockaddr
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	addr.sin_port = htons(47117); //< Attention, l'API socket veut du big endian

	if (connect(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
	{
		std::cerr << "failed to connect: " << WSAGetLastError() << std::endl;
		return EXIT_FAILURE;
	}
    
    std::vector<std::uint8_t> message;
    TestClass testClass(21);

    testClass.Serialize(message);
    
    // Gestion et/ou envoi du message au serveur
    if (send(sock, reinterpret_cast<const char*>(message.data()), static_cast<int>(message.size()), 0) == SOCKET_ERROR)
    {
        std::cerr << "failed to connect: " << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }

	closesocket(sock);

	return EXIT_SUCCESS;
}
