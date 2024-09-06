#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "ASMLoader.h"

#include "cave_story.h"
#include "Main.h"
#include "ModSettings.h"

#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>

#define MAX_LINE_LENGTH 500

bool setting_enable_asm_loader = true;

struct PatchData
{
    uintptr_t address;
    std::vector<unsigned char> patchBytes;
    std::vector<unsigned char> originalBytes;
};

std::vector<PatchData> patches;

void ApplyPatches(const char* patchFile)
{
    char path[MAX_PATH];
    sprintf(path, "%s\\%s.txt", gPatchesPath, patchFile);

    FILE* file = fopen(path, "r");
    if (!file) {
        std::cerr << "Failed to open the patch file." << std::endl;
        return;
    }

    char line[MAX_LINE_LENGTH];

    uintptr_t address = 0;
    PatchData currentPatch;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '0' && line[1] == 'x') {
            // New function address found, process the previous bytes
            if (!currentPatch.patchBytes.empty()) {
                // Store the original bytes
                SIZE_T bytesRead;
                currentPatch.originalBytes.resize(currentPatch.patchBytes.size());
                ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(currentPatch.address),
                    currentPatch.originalBytes.data(), currentPatch.originalBytes.size(), &bytesRead);

                // Print the original bytes to the console in hex format
                printf("Original bytes at address 0x%08lX: ", currentPatch.address);
                for (const auto& byte : currentPatch.originalBytes) {
                    printf("%02X ", byte);
                }
                printf("\n");

                // Write the patch bytes to the current process
                WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(currentPatch.address),
                    currentPatch.patchBytes.data(), currentPatch.patchBytes.size(), nullptr);

                // Print the patch bytes to the console in hex format
                printf("Patching address 0x%08lX with bytes: ", currentPatch.address);
                for (const auto& byte : currentPatch.patchBytes) {
                    printf("%02X ", byte);
                }
                printf("\n");

                patches.push_back(std::move(currentPatch));
            }

            // Get the new function address
            sscanf(line, "%lx", &address);

            // Reset the current patch
            currentPatch = {};
            currentPatch.address = address;
        }
        else {
            // Parse line as hex bytes and store them
            unsigned int value;
            char hexByte[3];
            hexByte[2] = '\0';

            // Read bytes using sscanf
            for (int i = 0; i < MAX_LINE_LENGTH; i += 3) {
                if (sscanf(line + i, "%2s", hexByte) == 1) {
                    value = strtol(hexByte, nullptr, 16);
                    currentPatch.patchBytes.push_back(static_cast<unsigned char>(value));
                }
                else {
                    break;
                }
            }
        }
    }

    fclose(file);

    // Write the patch bytes for the last address found
    if (!currentPatch.patchBytes.empty()) {
        // Store the original bytes
        SIZE_T bytesRead;
        currentPatch.originalBytes.resize(currentPatch.patchBytes.size());
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(currentPatch.address),
            currentPatch.originalBytes.data(), currentPatch.originalBytes.size(), &bytesRead);

        // Print the original bytes to the console in hex format
        printf("Original bytes at address 0x%08lX: ", currentPatch.address);
        for (const auto& byte : currentPatch.originalBytes) {
            printf("%02X ", byte);
        }
        printf("\n");

        // Write the patch bytes to the current process
        WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(currentPatch.address),
            currentPatch.patchBytes.data(), currentPatch.patchBytes.size(), nullptr);

        // Print the patch bytes to the console in hex format
        printf("Patching address 0x%08lX with bytes: ", currentPatch.address);
        for (const auto& byte : currentPatch.patchBytes) {
            printf("%02X ", byte);
        }
        printf("\n");

        patches.push_back(std::move(currentPatch));
    }
}


void UnpatchMemory()
{
    for (auto& patch : patches)
    {
        SIZE_T bytesWritten;
        SIZE_T patchSize = patch.originalBytes.size();

        // Restore the original bytes
        if (!WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(patch.address),
            patch.originalBytes.data(), patchSize, &bytesWritten))
        {
            DWORD lastError = GetLastError();
            std::cerr << "Failed to restore original bytes at address 0x" << std::hex << patch.address << ". Error code: " << lastError << std::endl;
        }
        else
        {
            printf("Unpatching address 0x%08lX\n", patch.address);

            // Print the original bytes to the console in hex format
            printf("Original bytes at address 0x%08lX: ", patch.address);
            for (const auto& byte : patch.originalBytes) {
                printf("%02X ", byte);
            }
            printf("\n");

            // Print the patch bytes to the console in hex format
            printf("Patch bytes at address 0x%08lX: ", patch.address);
            for (const auto& byte : patch.patchBytes) {
                printf("%02X ", byte);
            }
            printf("\n");
        }
    }

    patches.clear();
}