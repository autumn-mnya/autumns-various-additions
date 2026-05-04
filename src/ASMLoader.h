#pragma once

extern bool setting_enable_asm_loader;

void ApplyPatches(const char* patchFile);
void UnpatchMemory();