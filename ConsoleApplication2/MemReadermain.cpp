#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

//BitConverter.toInt32
template <typename IntegerType>
IntegerType bitsToInt(IntegerType& result, const unsigned char* bits, bool little_endian = true)
{
	result = 0;
	if (little_endian)
		for (int n = sizeof(result); n >= 0; n--)
			result = (result << 8) + bits[n];
	else
		for (unsigned n = 0; n < sizeof(result); n++)
			result = (result << 8) + bits[n];
	return result;
}
//End of BitConverter Sample

using namespace std;

DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;

	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}

	}

	CloseHandle(processesSnapshot);
	return 0;

} //this function

int main(int args, char* argv[])
{
	LPCVOID MemoryAddress = (LPCVOID)0x004B0C44;
		//Memory Location

	unsigned char *buffer = new unsigned char[4]; 
		//Storage For Readed Memory

	DWORD ProcessID = FindProcessId(L"th12.exe");
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ, false, ProcessID);
	
		//PROCESS_ALL_ACCESS

	while (true)
	{

		ReadProcessMemory(hProcess, MemoryAddress, buffer, 4, 0);
		//Read the Process


		int output = 0;
		bitsToInt(output, buffer);

		//Touhou score support
		int score = output * 10;
	
		printf("Touhou Score : ");
		printf("%d\n\n", score);
		printf("*** DEBUG ***\n");
		printf("Data from remote Process : ");
		printf("%d\n\n", output);
		printf("**************************\n");
		Sleep(100);

	}

	system("pause");

}