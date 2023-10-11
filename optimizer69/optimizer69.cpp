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
 * Recursively delete files and directories within a specified path.
 *
 * @param pathToDelete The directory path to delete
 */
void deletePathRecursively(const path& pathToDelete) {
    if (exists(pathToDelete)) {
        for (const directory_entry& entry : directory_iterator(pathToDelete)) {
            try {
                if (is_regular_file(entry)) {
                    remove(entry);

                    cout << "Deleted a temp file: " << entry.path() << endl;
                } else if (is_directory(entry)) {
                    deletePathRecursively(entry.path());
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
    const path tempPath = temp_directory_path();

    for (string process : processesToKill) {
        killProcess(process);
    }

    try {
        deletePathRecursively(tempPath);

        cout << "Deleted temporary files and directories from: " << tempPath << endl;
    } catch (const filesystem_error& e) {
        cerr << "Error accessing the directory: " << e.what() << endl;
    }

    return 0;
}
