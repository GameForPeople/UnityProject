#include "Server.h"

static std::vector<CUserData> userData;	//床傾球拭辞亀 紫遂背醤馬澗汽, 政煽 汽戚斗亜 位蟹弦生檎 嬢胸惟 馬形壱!!, 穿蝕生稽 識情背辞 袷 床切!ぞぞぞぞぞぞぞぞ
static bool isSaveOn{ false };	// 煽舌 食採 毒舘 (適虞拭 税背, 舛左 痕井 推短 閤聖 獣 true稽 痕井 / 瓜戚 疑奄鉢 琶推蒸聖牛)

DWORD WINAPI SaveUserDate(LPVOID arg);

int main(int argc, char * argv[])
{
	char* retIPChar;
	retIPChar = new char[20]; // IPv4亜 20 char左陥 適析 宋嬢亀 蒸製.
	GetExternalIP(retIPChar);

	printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n");
	printf("＝ IOCP Server  - Bridge Unity Project          \n");
	printf("＝                                ver 0.1 180815\n");
	printf("＝\n");
	printf("＝    IP Address : %s \n", retIPChar);
	printf("＝    Server Port : %d \n", SERVER_PORT);
	printf("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝\n\n");

	delete []retIPChar;

#pragma region [Load UserData]

	std::ifstream inFile("UserData.txt", std::ios::in);
	std::string ID;
	
	int PW, winCount, loseCount, Money;
	int userDataCount{};

	inFile >> userDataCount;
	userData.reserve(userDataCount);

	for (int i = 0; i < userDataCount; i++) {
		inFile >> ID >> PW >> winCount >> loseCount >> Money;

		userData.emplace_back(ID, PW, winCount, loseCount, Money);
	}

	inFile.close();

	std::cout << "  - UserData Load Complete! " << std::endl << std::endl;


	for (auto i : userData) 
	{
		std::cout << i.GetID() << " " << i.GetPW() << " " << i.GetWinCount() << " " << i.GetLoseCount() << "  " << i.GetMoney() << std::endl;
	}

#pragma endregion

	userData.emplace_back((std::string)"abcd", 1541, 0, 100, 3000 );

	isSaveOn = true;

	for (auto i : userData)
	{
		std::cout << i.GetID() << " " << i.GetPW() << " " << i.GetWinCount() << " " << i.GetLoseCount() << "  " << i.GetMoney() << std::endl;
	}


#pragma region [ 制紗 段奄鉢 貢 脊窒径 刃戟 匂闘 持失 ]
	//制紗 段奄鉢
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// 脊窒径 刃戟 匂闘 持失
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	/*
		CreateIoCompletionPort澗 砧亜走 蝕拝聖 敗!
			1. 脊窒径 刃戟 匂闘 持失
			2. 社掴引 脊窒径 刃戟 匂闘 尻衣 (IO舌帖人 IOCP尻衣)

		1腰属 昔切葵,  IOCP人 尻衣拝 輩級, 持失獣澗 INVALID_HANDLE_VALUE研 昔切稽 角沿
		2腰属 昔切葵,  IOCP 輩級, 湛 持失獣澗 NULL
		3腰属 昔切葵, IO刃戟獣 角嬢哀 葵, 紫遂切亜 角奄壱 粛精 葵 角沿
		4腰属 昔切葵, 廃腰拭 疑拙拝 置企 什傾球 鯵呪, 0 角奄檎 覗稽室什 収切稽 切疑 走舛喫
	*/

	if (hcp == NULL) 
	{ 
		printf("脊窒径 刃戟 匂闘 持失");
		return 1; 
	}

	// CPU 鯵呪 溌昔
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// CPU 鯵呪 == 12鯵 * 2鯵税 拙穣切 什傾球 持失
	// IO拙穣戚 刃戟吉 板, 刃戟吉 IO拭 企廃 坦軒研 呪楳拝 什傾球 熱聖 姥失廃陥.
	// 析鋼旋生稽 什傾球 熱税 滴奄澗 覗稽室辞 鯵呪税 2壕 舛亀研 拝雁廃陥.
	HANDLE hThread;  
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i) 
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

#pragma endregion

#pragma region [ 社掴 持失 貢, 郊昔球, 軒充 ]
	
	//Socket()
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) err_quit((char *)"socket()");

	//bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);
	int retVal = bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit((char *)"bind()");

	// Listen()!
	retVal = listen(listenSocket, SOMAXCONN);
	if(retVal == SOCKET_ERROR) err_quit((char *)"listen()");

#pragma endregion

	printf("  - Dedi server activated!\n\n");

#pragma region [ Thread Run! Accept and Save UserData ]
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int addrLength;
	DWORD recvBytes, flags;

	HANDLE hSaveUserDataThread;
	hSaveUserDataThread = CreateThread(NULL, 0, SaveUserDate, NULL, 0, NULL);
	CloseHandle(hSaveUserDataThread);

	while (7) {
		//accept()
		addrLength = sizeof(clientAddr);
		clientSocket = accept(listenSocket, (SOCKADDR *)&clientAddr, &addrLength);
		if (clientSocket == INVALID_SOCKET)
		{
			err_display((char *)"accept()");
			break;
		}

		// 適虞戚情闘 辞獄拭 羨紗(Accept) 敗聖 硝顕
		printf("[TCP 辞獄] 適虞戚情闘 羨紗 : IP 爽社 =%s, Port 腰硲 = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		
		// 社掴引 脊窒径 刃戟 匂闘 尻衣
		CreateIoCompletionPort((HANDLE)clientSocket, hcp, clientSocket, 0);
		
		// 社掴 舛左 姥繕端 拝雁
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL) break;
		
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = clientSocket;
		ptr->recvBytes = ptr->sendBytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUF_SIZE;

		// 搾疑奄 脊窒径税 獣拙
		flags = 0;
		retVal = WSARecv(
			clientSocket, // 適虞戚情闘 社掴
			&ptr->wsabuf, // 石聖 汽戚斗 獄遁税 匂昔斗
			1,			 // 汽戚斗 脊径 獄遁税 鯵呪
			&recvBytes,  // recv 衣引 石精 郊戚闘 呪, IOCP拭辞澗 搾疑奄 号縦生稽 紫遂馬走 省生糠稽 nullPtr研 角移亀 巷号
			&flags,		 // recv拭 紫遂吃 巴掘益
			&ptr->overlapped, // overlapped姥繕端税 匂昔斗
			NULL			// IOCP拭辞澗 紫遂馬走 省生糠稽 NULL, nullptr角移亀 巷号
		);

		if (retVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				err_display((char *)"WSARecv()");
			}

			continue;
		}
	}
#pragma endregion

#pragma region [ plzDoNotQuit! ]
	char plzDoNotQuit{};
	std::cin >> plzDoNotQuit;
	
	
	WSACleanup;
	return 0;
#pragma endregion
}

DWORD WINAPI SaveUserDate(LPVOID arg) {
	while (7) {
		Sleep(10000);

		if (isSaveOn) {
			isSaveOn = false;

			Sleep(2000);
			std::ofstream outFile("UserData.txt", std::ios::out);

			outFile << userData.size() << std::endl;

			for (auto i : userData) {
				outFile << " " << i.GetID()
					<< " " << i.GetPW()
					<< " " << i.GetWinCount()
					<< " " << i.GetLoseCount()
					<< " " << i.GetMoney()
					<< std::endl;
			}
			outFile.close();

			std::cout << "[ System(Server Core) - UserDataSave ]" << std::endl;
			isSaveOn = false;
			Sleep(2000);
		}
	}
	return 0;
}