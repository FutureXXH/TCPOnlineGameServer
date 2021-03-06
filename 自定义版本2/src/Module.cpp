#include "../include/ModuleManager.h"



int LuaModule::LuaLog(lua_State *L)
{
	int num = lua_gettop(L);
	if(!lua_isstring(L,1))
	{
		lua_pushinteger(L,-1);
		return -1;
	}
	string LogLevel = lua_tostring(L,1);
	

	if(!lua_isstring(L,2))
	{
		lua_pushinteger(L,-1);
		return -1;
	}
	string Loginfo = lua_tostring(L,2);

	if(LogLevel == "INFO")
	Log(INFO,Loginfo);
	else if(LogLevel == "WARNING")
	Log(WARNING,Loginfo);
	else if(LogLevel == "ERROR")
	Log(ERROR,Loginfo);

       return 1;
}


int LuaModule::LuaRegMessage(lua_State *L)
{
 int num = lua_gettop(L);
//获取第一个参数
if(!lua_isinteger(L,1))
{
	lua_pushinteger(L,-1);
	return -1;
}

int MessageID = lua_tointeger(L,1);
//获取第二个参数
if(!lua_isinteger(L,2))
{
	lua_pushinteger(L,-1);
	return -1;
}
int moduleID = lua_tointeger(L,2);

__ModuleManager->RegisterMessage(MessageID,moduleID);

}







 int LuaModule::LuaSendMessage(lua_State *L)
{
int num = lua_gettop(L);
//获取第一个参数
if(!lua_isinteger(L,1))
{
	lua_pushinteger(L,-1);
	return -1;
}

Message* m = __ModuleManager->GetMessageObj();
m->MessageID = lua_tointeger(L,1);



size_t strlen = 0;
//获取第二个参数
if(!lua_isstring(L,2))
{
	lua_pushinteger(L,-1);
	return -1;
}
const char* str = lua_tolstring(L,2,&strlen);



m->writeData(str,strlen);
m->dataSize = strlen;
//获取第三个参数
if(!lua_isinteger(L,3))
{
	lua_pushinteger(L,-1);
	return -1;
}
m->srcModuleID = lua_tointeger(L,3);

//获取第四个参数
if(!lua_isinteger(L,3))
{
	lua_pushinteger(L,-1);
	return -1;
}
m->aimModuleID = lua_tointeger(L,4);

__ModuleManager->pushDataMessageQueue(m);

}




int LuaModule::LuaLoadModule(lua_State *L)
{
int num = lua_gettop(L);
//获取第一个参数
if(!lua_isinteger(L,1))
{
	lua_pushinteger(L,-1);
	return -1;
}
int ID = lua_tointeger(L,1);

size_t strlen = 0;
//获取第二个参数
if(!lua_isstring(L,2))
{
	lua_pushinteger(L,-1);
	return -1;
}


Message* m = __ModuleManager->GetMessageObj();
m->MessageID = 0;
m->srcModuleID = LoadLuaModule;


string str = lua_tolstring(L,2,&strlen);


m->writeData(ID);
m->writeData(str.c_str(),strlen);



__ModuleManager->pushDataMessageQueue(m);

}

int LuaModule::LuaCloseModule(lua_State *L)
{
   int num = lua_gettop(L);
    //获取第一个参数
   if(!lua_isinteger(L,1))
   {
	lua_pushinteger(L,-1);
	return -1;
   }
   int ID = lua_tointeger(L,1);

    Message* m = __ModuleManager->GetMessageObj();
    m->MessageID = 0;
    m->srcModuleID = DeleteModule;


   m->writeData(ID);
   __ModuleManager->pushDataMessageQueue(m);


}

int LuaModule::LuaGetTimeStamp(lua_State *L)
{
 

 lua_pushinteger(L,GetTimeStamp());
 return 1;


}





bool LuaModule::ReloadLua()
{
	//重新打开lua文件
    if(!OpenLuaFile())
	{
		ModuleState = MODULE_CLOSING;
	}
	//执行初始函数
	Init();
   
   
}


LuaModule::LuaModule(int setID,string FileName)
{
   luaFileName = "./LuaModules/" + FileName;
   ID = setID;
    //初始化lua虚拟机
    InitLua();
	//打开lua文件
    if(!OpenLuaFile())
	{
		
      auto m = __ModuleManager->GetMessageObj();
       if(m == nullptr)return;
         this->ModuleState = MODULE_ERROR;
        m->MessageID= 0;
	    m->writeData(ID);
       m->srcModuleID = DeleteModule;
       __ModuleManager->pushDataMessageQueue(m);
	}
}


void LuaModule::InitLua()
{
   luaPtr = luaL_newstate();
   luaL_openlibs(luaPtr);

    //注册C++函数进LUA
   luaL_Reg lualibs[] = {
    {"SendMessage",LuaSendMessage},
	{"RegMessage",LuaRegMessage},
	{"LoadNewModule",LuaLoadModule},
	{"CloseModule",LuaCloseModule},
	{"GetTime",LuaGetTimeStamp},
	{"Log",LuaLog},
    {NULL,NULL}
   };

   luaL_newlib(luaPtr,lualibs);
   lua_setglobal(luaPtr,"ServerLuaLib");


  

}

bool LuaModule::OpenLuaFile()
{
     
	 int isok = luaL_dofile(luaPtr,luaFileName.c_str());
	 if(isok == 1)
	 {
		 Log(ERROR,"模块ID"+str(ID) + "  Lua文件运行失败!");
		 return false;
	 }
	 return true;
}


 void LuaModule::update()
 {
    

	lua_getglobal(luaPtr,"OnUpdate");
	int iserr = lua_pcall(luaPtr,0,0,0);
	if(iserr)
	{
		Log(ERROR,lua_tostring(luaPtr,-1));
	}
 }

 void LuaModule::Init()
 {

     
	lua_getglobal(luaPtr,"OnInit");
	lua_pushinteger(luaPtr,ID);
	int iserr = lua_pcall(luaPtr,1,0,0);
	if(iserr)
	{
		Log(ERROR,lua_tostring(luaPtr,-1));
	}
 }

 void LuaModule::parseMessage(Message* messagePtr)
 {
 
	
	 lua_getglobal(luaPtr,"OnParseMessage");
	 lua_pushinteger(luaPtr,messagePtr->MessageID);
     lua_pushlstring(luaPtr,messagePtr->data,messagePtr->dataSize);
	 lua_pushinteger(luaPtr,messagePtr->srcModuleID);

	int iserr = lua_pcall(luaPtr,3,0,0);
	if(iserr)
	{
		Log(ERROR,lua_tostring(luaPtr,-1));
	}
 }

void LuaModule::Exit() 
{
  	lua_getglobal(luaPtr,"OnExit");
	int iserr = lua_pcall(luaPtr,0,0,0);
	if(iserr)
	{
		Log(ERROR,lua_tostring(luaPtr,-1));
	}
}

LuaModule::~LuaModule()
{
  CloseLua();
}

void LuaModule::CloseLua()
{
   lua_close(luaPtr);
}


//========================================================================================


ModuleBase::~ModuleBase()
{
    //释放消息对象数据
	delete[] MessageMemoryPtr;
	
}
ModuleBase::ModuleBase()
{
	//预开辟消息数据内存
	MessageMemoryPtr = new Message[MessageObjPoolSize];
	for (int i = 0; i < MessageObjPoolSize; i++)
	{
		MessageObjPool.push(&MessageMemoryPtr[i]);
	}
}


int64 ModuleBase::GetTimeStamp()
{
  return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void ModuleBase::parseQueue()
{
	Message* p = nullptr;
	p = MessageQueue.pop();
	//时间分配调度 防止一直处理消息 默认20毫秒
	long long startTime = GetTimeStamp();
	while (p != nullptr&&(GetTimeStamp() - startTime < 20))
	{

		parseMessage(p);
        
		//放回对象池中
	    PushMessageObj(p);
		p = MessageQueue.pop();
	}

}

uint32 ModuleBase::getQueueSize()
{
	return MessageQueue.size();
}

bool ModuleBase::pushMessage(Message* m)
{
	if (MessageQueue.size() >= MaxQueueSize)return false;
    //从对象池取出
    Message* cpm  = GetMessageObj();
	*cpm = *m;
	MessageQueue.push(cpm);
	return true;
}

bool ModuleBase::RegisterMessageID(int32 ID)
{
	__ModuleManager->RegisterMessage(ID,this->ID);
	return true;
}

Message* ModuleBase::GetMessageObj()
{
    if(MessageObjPool.size() == 0)return nullptr;
	auto p = MessageObjPool.pop();
	p->reset();
	return p;
}


bool ModuleBase::PushMessageObj(Message* obj)
{
     MessageObjPool.push(obj);
	return true;
}




