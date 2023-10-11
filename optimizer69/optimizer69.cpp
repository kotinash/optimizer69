#include <iostream>
#include <Windows.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

string processesToKill[15] = {
    "Salad.exe",
    "GoogleCrashHandler.exe",
    "GoogleCrashHandler64.exe",
    "vmmem", // Windows sandbox thingy that uses ~400 mb while idling
    "igfxHK.exe",
    "igfxEM.exe",
    "ApntEx.exe", // Dell PCs
    "SecurityHealthService.exe",
    "igfxCUIService.exe",
    "RuntimeBroker.exe",
    "dllhost.exe",
    "WmsSelfHealingSvc.exe",
    "WmsSessionAgent.exe",
    "WmsSvc.exe",
    "taskhostw.exe"
};

/**
 * Kill a specific process by its name
 *
 * @param process The name of the process to be killed
 */
void killProcess(string process) {
    system(("taskkill /f /im " + process).c_str());
}

/**
 * Delete temporary files from a specified directory.
 *
 * @param path The directory path where temporary files are located
 */
void deleteTempFiles(const string& path) {
    for (const auto& entry : directory_iterator(path)) {
        if (is_regular_file(entry)) {
            try {
                remove(entry);

                cout << "Deleted a temp file " << entry << endl;
            } catch (...) {
                cout << "Failed to delete a temp file " << entry << endl;
            }
        }
    }
}

/**
 * Main function to kill processes and delete temporary files
 */
int main() {
    for (string process : processesToKill) {
        killProcess(process);
    }

    try {
        const string path = temp_directory_path().string();
        deleteTempFiles(path);
    } catch (const filesystem_error& e) {
        cerr << "Error accessing the directory: " << e.what() << endl;
    }

    return 0;
}
