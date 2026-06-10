#include "SystemMonitor.h"
#include <iostream>
#include <windows.h>

using namespace std;

// Display Windows system information
void SystemMonitor::displaySystemInfo() {
    SYSTEM_INFO sysInfo;
    MEMORYSTATUSEX memoryStatus;

    // Get CPU/system architecture information
    GetSystemInfo(&sysInfo);

    // Prepare memory structure
    memoryStatus.dwLength = sizeof(memoryStatus);

    // Get memory/RAM information
    GlobalMemoryStatusEx(&memoryStatus);

    cout << "\n===== System Monitor =====\n";

    // Number of CPU cores/logical processors
    cout << "Processor Count: " << sysInfo.dwNumberOfProcessors << endl;

    // RAM usage percentage
    cout << "Memory Usage: " << memoryStatus.dwMemoryLoad << "%\n";

    // Total physical RAM in GB
    cout << "Total RAM: "
         << memoryStatus.ullTotalPhys / (1024 * 1024 * 1024)
         << " GB\n";

    // Available physical RAM in GB
    cout << "Available RAM: "
         << memoryStatus.ullAvailPhys / (1024 * 1024 * 1024)
         << " GB\n";

    // Disk space variables
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

    // Get disk space for C drive
    if (GetDiskFreeSpaceEx(
            "C:\\",
            &freeBytesAvailable,
            &totalNumberOfBytes,
            &totalNumberOfFreeBytes)) {

        cout << "Total Disk Space C: "
             << totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024)
             << " GB\n";

        cout << "Free Disk Space C: "
             << totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024)
             << " GB\n";
    }
    else {
        cout << "Could not read disk information.\n";
    }
}