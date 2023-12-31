#pragma once
#include <Windows.h>

//#include "D:/GithubReps/UmiDrive-Runtime/main.h"

class DriveRuntime
{
private:
	HMODULE hDl;
public:
	struct FileDesc
	{
		int ID;
		long long int Size;
	};
public:
	using dir_callback =					void (*)(const char* name, unsigned long long length, char isDir,void * data);
	static int (*LoadDrive)					(const char* token, int l1, const char* email, int l2);
	static void (*UnloadDrive)				(int ID);
	static void (*GetCurrDirFiles)			(int driveID, void* data, dir_callback cb);
	static FileDesc (*LoadFile)					(int driveID, const char* path, int l);
	static long long int (*ReadLoadedFile)	(int driveID, int fileID, void* buf, long long unsigned int size);
	static void (*CloseFile)				(int driveID, int fileID);
private:
	DriveRuntime()
	{
		hDl = LoadLibrary(R"(D:\GithubReps\UmiDrive-Runtime\main.dll)");
		if (hDl == NULL)
		{
			MessageBox(NULL, "Failed to load main.dll", "Error", MB_OK);
			return;
		}
		LoadDrive = (decltype(LoadDrive))GetProcAddress(hDl, "LoadDrive");
		if (LoadDrive == NULL)
		{
			MessageBox(NULL, "Failed to load LoadDrive", "Error", MB_OK);
			return;
		}
		UnloadDrive = (decltype(UnloadDrive))GetProcAddress(hDl, "UnloadDrive");
		if (UnloadDrive == NULL)
		{
			MessageBox(NULL, "Failed to load UnloadDrive", "Error", MB_OK);
			return;
		}
		LoadFile = (decltype(LoadFile))GetProcAddress(hDl, "LoadFile");
		if (LoadFile == NULL)
		{
			MessageBox(NULL, "Failed to load LoadFile", "Error", MB_OK);
			return;
		}
		CloseFile = (decltype(CloseFile))GetProcAddress(hDl, "CloseFile");
		if (CloseFile == NULL)
		{
			MessageBox(NULL, "Failed to load CloseFile", "Error", MB_OK);
			return;
		}
		ReadLoadedFile = (decltype(ReadLoadedFile))GetProcAddress(hDl, "ReadLoadedFile");
		if (ReadLoadedFile == NULL)
		{
			MessageBox(NULL, "Failed to load ReadLoadedFile", "Error", MB_OK);
			return;
		}
		GetCurrDirFiles = (decltype(GetCurrDirFiles))GetProcAddress(hDl, "GetCurrDirFiles");
		if (GetCurrDirFiles == NULL)
		{
			MessageBox(NULL, "Failed to load GetCurrDirFiles", "Error", MB_OK);
			return;
		}
	}
	~DriveRuntime()
	{
		FreeLibrary(hDl);
	}
private:
	static DriveRuntime instance;
};

DriveRuntime DriveRuntime::instance;

int (*DriveRuntime::LoadDrive) (const char* token, int l1,const char* email, int l2);
void (*DriveRuntime::UnloadDrive) (int ID);
void (*DriveRuntime::GetCurrDirFiles) (int driveID, void* data, dir_callback cb);
DriveRuntime::FileDesc (*DriveRuntime::LoadFile) (int driveID,const char* path, int l);
long long int (*DriveRuntime::ReadLoadedFile) (int driveID, int fileID, void* buf, long long unsigned int size);
void (*DriveRuntime::CloseFile) (int driveID, int fileID);