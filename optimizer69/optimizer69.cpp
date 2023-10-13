#include <iostream>
#include <Windows.h>
#include <filesystem>

using namespace std;
using namespace filesystem;

#define tryIgnore(function) \
        try {               \
            function;       \
        } catch (...) {}    \

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
 * Recursively delete files and directories within a specified path.
 *
 * @param pathToDelete The directory path to delete
 */
void deleteDirectory(const path& pathToDelete) {
    if (exists(pathToDelete)) {
        for (const directory_entry& entry : directory_iterator(pathToDelete)) {
            try {
                if (is_regular_file(entry)) {
                    remove(entry);

                    cout << "Deleted file: " << entry.path() << endl;
                } else if (is_directory(entry)) {
                    deleteDirectory(entry.path());
                }
            } catch (...) {
                cout << "Failed to delete: " << entry.path() << endl;
            }
        }

        remove(pathToDelete);
    }
}

/**
 * Main function. Nothing special
 */
int main() {
    cout << "This utility will purge your temp folder, prefetch logs, recycle bin and clean up your ram. Starting in 5 seconds" << endl;

    Sleep(5000);

    for (string process : processesToKill) {
        killProcess(process);
    }

    const path tempPath = temp_directory_path();

    tryIgnore(deleteDirectory(tempPath));
    tryIgnore(deleteDirectory("C:\\Windows\\Temp"));
    tryIgnore(deleteDirectory("C:\\Windows\\Prefetch"));
    tryIgnore(deleteDirectory("C:\\$RECYCLE.BIN"));

    return 0;
}
