// Project4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "cp2103-gpio.h"
#include "CP210xRuntimeDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int my_main(void);

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
			nRetCode = my_main();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}

int my_main(void)
{
	// printf("Hello %ls\n", theApp.m_lpCmdLine);

	LPWSTR *argv;
	int nArgs;

	argv = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == argv)
	{
		fprintf(stderr, "CommandLineToArgvW failed\n");
		return -1;
	}
	
	if (nArgs != 4) {
		fprintf(stderr, "Usage: %ls [COMx] [mask] [latch]\n", argv[0]);
		fprintf(stderr, "\nExamples:\n");
		fprintf(stderr, "  Set gpio 0 pin          : cp2103-gpio COM3 1 1\n");
		fprintf(stderr, "  Clear gpio 2 pin        : cp2103-gpio COM3 4 0\n");
		fprintf(stderr, "  Set all gpio pins       : cp2103-gpio COM3 0x0F 0x0F\n");
		fprintf(stderr, "  Clear all gpio pins     : cp2103-gpio COM3 15 0\n");
		fprintf(stderr, "  Set gpio 1,3 clear 0,2  : cp2103-gpio COM3 0x0F 0x0A\n");
		fprintf(stderr, "  Read all gpio pins      : cp2103-gpio COM3 0 0\n");
		fprintf(stderr, "\n");
		return -1;
	}

	// for (int i = 0; i < nArgs; i++) printf("argv[%d]: %ws\n", i, argv[i]);

	// DWORD ComPortNo = wcstol(argv[1], NULL, 10);
	LPWSTR ComPortStr = argv[1];
	WORD Mask = (WORD)wcstol(argv[2], NULL, 0);
	WORD Latch = (WORD)wcstol(argv[3], NULL, 0);

	LocalFree(argv);

	//Format our string for the COM port desired
	CString comString;
	comString.Format(_T("\\\\.\\%s"), ComPortStr);

	//Open a handle the the device specified
	HANDLE hDevice = CreateFile(comString, GENERIC_READ | GENERIC_WRITE, 0,	0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	0);

	//If the handle is valid then it opened
	if (hDevice != INVALID_HANDLE_VALUE)
	{
		BYTE bPartNum;

		//Get the current part number
		if (CP210xRT_GetPartNumber(hDevice, &bPartNum) == CP210x_SUCCESS)
		{
			printf("Chip    : CP21%02d\n", bPartNum);

			#if 0
			CP210x_PRODUCT_STRING productString;
			BYTE productStringLength;

			CP210x_SERIAL_STRING serialString;
			BYTE serialStringLength;

			if (CP210xRT_GetDeviceProductString(hDevice, productString, &productStringLength) == CP210x_SUCCESS)
			{
				printf("productString:%s\n", productString);
			}

			if (CP210xRT_GetDeviceSerialNumber(hDevice, serialString, &serialStringLength) == CP210x_SUCCESS)
			{
				printf("serialString:%s\n", serialString);
			}
			#endif
	
			if (Mask != 0)
			{
				if (CP210xRT_WriteLatch(hDevice, Mask, Latch) != CP210x_SUCCESS)
				{
					fprintf(stderr, "Error: CP210xRT_WriteLatch() failed\n");
					return -1;
				}

				fprintf(stderr, "Written : 0x%02X (Mask:0x%02X)\n", Latch, Mask);
			}

			if (CP210xRT_ReadLatch(hDevice, &Latch) != CP210x_SUCCESS)
			{
				fprintf(stderr, "Error: CP210xRT_ReadLatch() failed\n");
				return -1;
			}	

			printf("Read    : 0x%02X\n", Latch);
		}
		else
		{
			fprintf(stderr, "Error: GetPartNumber() failed on %ls port\n", ComPortStr);
			fprintf(stderr, "       Probably wrong COM port selected\n");
		}

		//Close the handle to the device
		CloseHandle(hDevice);
	}
	else
	{
		fprintf(stderr, "Error: Could not open %ls port!\n", ComPortStr);
	}

	return 0;
}