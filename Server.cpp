#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> 
#include<iostream>
#include<string>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll  

#define BUF_SIZE 10086
//using namespace std;  //直接用std好像会出现bug
using std::cout;

int main() {
	//初始化dll
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字  
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

	//绑定套接字  
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充  
	sockAddr.sin_family = PF_INET;  //使用IPv4地址  
	sockAddr.sin_addr.s_addr = inet_addr("192.168.2.104");  //回送IP地址 
	sockAddr.sin_port = htons(1234);  //端口  
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//进入监听状态  
	listen(servSock, 20);

	//接收客户端请求  
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	char buffer[BUF_SIZE] = { 0 };  //缓冲区  
	while (1) {
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //接收客户端发来的数据  
		send(clntSock, buffer, strLen, 0);  //将数据原样返回  

		closesocket(clntSock);  //关闭套接字  
		memset(buffer, 0, BUF_SIZE);  //重置缓冲区  
	}

	//关闭套接字  
	closesocket(servSock);

	//终止 DLL 的使用  
	WSACleanup();

	return 0;
}