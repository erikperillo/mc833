#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include "net.h"
#include "protocol.h"

using namespace std;

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 7532
#define MAX_NUM_THREADS 3

//mutex for free_thread array
std::mutex free_thread_mtx;
//indicates which threads are free to be used
bool free_thread[MAX_NUM_THREADS];

void error(const std::string& msg, int ret=1)
{
	perror(msg.c_str());
	exit(ret);
}

/*
The worker thread.
Interacts with each client, providing necessary services
*/
void serviceLoop(int id, int sock)
{
	NetMessage msg;
	NetAddr src;

	while(true)
	{
		//receiving message from client
		msg = recv(sock);
		if(msg.getErrCode() < 0)
			error("recv");	
		if(msg.getErrCode() == 0)
		{
			cout << "[" << id << "] connection ended." << endl;	
			break;
		}

		//displaying message
		src = msg.getSrcAddr();
		cout << "[" << id << "]"
			<< "[" << src.getIp() << ":" << src.getPort() << "]"
			<< " " << msg.getContent() << endl;
	}	

	close(sock);
	//marking itself as free at the end
	free_thread_mtx.lock();
	free_thread[id] = true;
	free_thread_mtx.unlock();
}

/*
Main server loop.
Receives incoming connections and dispatches them to service loops.
*/
void serverLoop()
{
	NetAddr addr(SERVER_NAME, SERVER_PORT);
	NetAddr conn;
	std::thread threads[MAX_NUM_THREADS];
	int sock;
	int conn_sock;
	int i;

	//marking every thread as free
	for(i=0; i<MAX_NUM_THREADS; i++)
		free_thread[i] = true;

	if(addr.getErrCode() < 0)
		error("addr");	

	//getting socket for TCP connection
	sock = getSocket(SOCK_STREAM);	
	if(sock < 0)
		error("getSocket");
	
	//binding and listening for incoming connections
	if(bind(sock, addr) < 0)
		error("bind");
	if(listen(sock, MAX_NUM_THREADS) < 0)
		error("listen");

	cout << "started server" << endl;
	//main loop
	while(true)
	{
		//getting new connection
		conn = accept(sock);	
		conn_sock = conn.getErrCode();
		if(conn_sock < 0)
			error("accept");

		free_thread_mtx.lock();
		for(i=0; i<MAX_NUM_THREADS; i++)
			if(free_thread[i])	
			{
				if(threads[i].joinable())
					threads[i].join();

				cout << "<new connection on " 
					<< conn.getIp() << ":" << conn.getPort() << ">" << endl;
				threads[i] = std::thread(serviceLoop, i, conn_sock);
				free_thread[i] = false;	
				break;
			}
		free_thread_mtx.unlock();

		if(i == MAX_NUM_THREADS)
			close(conn_sock);
	}

	//waiting for threads to finish
	//for(i=0; i<MAX_NUM_THREADS; i++)
	//	threads[i].join();	
}

int main()
{
	serverLoop();
	return 0;
}
