#include <v8.h>
#include <node.h>
#include <nan.h>
#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using Nan::To;
using v8::Local;
using v8::Number;
using v8::Object;

int getNum()
{
    int hello = 5;
    return hello;
}

void writeLog(char* logTxt)
{
    using namespace std;
    ofstream OutFile;
    char *logDir = "E:\\test";
    char logPath[100];
    strcpy(logPath, logDir);
    const char *logFileName = strcat(logPath, "clipboard_log.txt");
    OutFile.open(logFileName, std::ios::out | std::ios::app); 
    if (!OutFile.is_open())
        return;

    time_t t = time(0);
    char date[32] = {NULL};
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&t));

    OutFile << date << " : " << logTxt;
    OutFile << std::endl;
    OutFile.close();
}

NAN_METHOD(sum)
{
    Nan::HandleScope scope;
    uint32_t sum = 0;
    for (int i = 0; i < info.Length(); i++)
    {
        sum += To<int64_t>(info[i]).FromJust();
    }
    sum += getNum();
    info.GetReturnValue().Set(Nan::New(sum));
}

NAN_METHOD(copy)
{
    Nan::HandleScope scope;
    HWND hWnd = NULL;
    HANDLE hHandle;
    OpenClipboard(hWnd); 
    struct stat buffer;
    EmptyClipboard();                          
    hHandle = GlobalAlloc(GMEM_FIXED, 1000);  
    char *pData = (char *)GlobalLock(hHandle);
    char* hello = "helloworld";
    strcpy(pData, hello);
    SetClipboardData(CF_TEXT, hHandle); 
    GlobalUnlock(hHandle);             
    CloseClipboard();                
}

void init(Local<Object> exports)
{
    Nan::HandleScope scope;
    Nan::SetMethod(exports, "sum", sum);
    Nan::SetMethod(exports, "copy", copy);
}

NODE_MODULE(memwatch, init);