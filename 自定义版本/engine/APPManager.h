#pragma once
#ifndef __APPMANAGER_H
#define __APPMANAGER_H

#include "IDefine.h"

#include "TaskManager.h"
#include <thread>
#include <concurrent_queue.h>
#include <unordered_map>


using namespace std;





class APPManager
{
public:

	int SafeCode;
	int threadnum = 0;
	
	//unordered_map<SOCKET, TCPClient*>* ClientMap;

public:
	bool InitManager( Concurrency::concurrent_queue<task*> * tasks);
	void ProcessService();
	void Response(task & Task);

};



#endif // !__APPMANAGER_H


