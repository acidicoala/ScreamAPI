// Windows headers
#include <windows.h>
#include <shobjidl.h> 

// C++ std lib headers
#include <cassert>
#include <string>
#include <vector>
#include <fstream>

void showError(std::wstring message){
	MessageBox(NULL, message.c_str(), L"DLL Exports Reader Error", MB_ICONERROR | MB_OK);
}

// Adapted from: https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box
// I have removed most of the error checking, since this executable is not meant to be used in production.
std::wstring getDLLpath(){
	PWSTR pszFilePath;
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	IFileOpenDialog* pFileOpen;

	// Create the FileOpenDialog object.
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
					 IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	// Restrict the FileOpenDialog to dll files only 
	COMDLG_FILTERSPEC rgSpec[] =
	{
		{ L"Dynamic-Link Libraries", L"*.dll" }
	};
	pFileOpen->SetFileTypes(1, rgSpec);

	// Show the Open dialog box.
	if(FAILED(pFileOpen->Show(NULL))){
		// Quit the app if nothing was selected
		exit(0);
	}

	// Get the file name from the dialog box.
	IShellItem* pItem;
	pFileOpen->GetResult(&pItem);
	pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	// Copy the filename to string
	auto dllPath = std::wstring(pszFilePath);
	CoTaskMemFree(pszFilePath);
	pItem->Release();
	pFileOpen->Release();
	CoUninitialize();

	return dllPath;
}

// Adapted from: https://github.com/mborne/dll2def/blob/master/dll2def.cpp
std::vector<std::string> getExportFunctions(std::wstring dllPath){
	auto exportFunctions = std::vector<std::string>();

	HMODULE lib = LoadLibraryEx(dllPath.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
	if(!lib){
		showError(L"Can't open " + dllPath);
		exit(1);
	}
	assert(((PIMAGE_DOS_HEADER) lib)->e_magic == IMAGE_DOS_SIGNATURE);
	PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS) ((BYTE*) lib + ((PIMAGE_DOS_HEADER) lib)->e_lfanew);
	assert(header->Signature == IMAGE_NT_SIGNATURE);
	assert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
	PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY) (
		(BYTE*) lib + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
		);
	PVOID names = (BYTE*) lib + exports->AddressOfNames;

	// Iterate over the names and add them to the vector
	for(unsigned int i = 0; i < exports->NumberOfNames; i++){
		auto name = (char*) lib + ((DWORD*) names)[i];
		exportFunctions.emplace_back(name);
	}

	return exportFunctions;
}

std::vector<std::string> overrideFunctions{
	"EOS_Ecom_QueryOwnership",
	"EOS_Ecom_QueryEntitlements",
	"EOS_Ecom_GetEntitlementsCount",
	"EOS_Ecom_CopyEntitlementByIndex",
	"EOS_Ecom_Entitlement_Release",
	"EOS_Ecom_Checkout",
	// Achievements
	"EOS_Achievements_QueryDefinitions",
	"EOS_Achievements_GetAchievementDefinitionCount",
	"EOS_Achievements_CopyAchievementDefinitionV2ByIndex",
	"EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId",
	"EOS_Achievements_QueryPlayerAchievements",
	"EOS_Achievements_GetPlayerAchievementCount",
	"EOS_Achievements_CopyPlayerAchievementByIndex",
	"EOS_Achievements_CopyPlayerAchievementByAchievementId",
	"EOS_Achievements_UnlockAchievements",
	"EOS_Achievements_AddNotifyAchievementsUnlockedV2",
	"EOS_Achievements_RemoveNotifyAchievementsUnlocked"
	// Achievements (Deprecated)
	/*"EOS_Achievements_CopyAchievementDefinitionByIndex",
	"EOS_Achievements_GetUnlockedAchievementCount",
	"EOS_Achievements_CopyUnlockedAchievementByIndex",
	"EOS_Achievements_CopyUnlockedAchievementByAchievementId",
	"EOS_Achievements_AddNotifyAchievementsUnlocked"*/
};

enum class Architecture{ Win32, Win64 };
void generateHeader(std::wstring dllPath, Architecture arch, std::vector<std::string> functionNames){
	auto exportLinkDLL = arch == Architecture::Win64 ? "EOSSDK-Win64-Shipping_o" : "EOSSDK-Win32-Shipping_o";
	auto headerName = arch == Architecture::Win64 ? L"LinkerExports64.h" : L"LinkerExports32.h";

	// Generate header path
	std::wstring headerPath = dllPath.substr(0, dllPath.rfind('\\') + 1) + headerName;

	// Try opening the output file
	std::ofstream file;
	file.open(headerPath, std::ofstream::out | std::ofstream::trunc);
	if(!file.is_open()){
		showError(L"Error opening file: " + headerPath);
		exit(1);
	}

	// Add header guard
	file << "#pragma once\n\n";

	// Finally, output all the exports to the header file
	for(auto const& name : functionNames){
		// Iterate over overriden names and check if it is there
		for(const auto& overrideFunction : overrideFunctions){
			bool isOverriding;
#ifdef _WIN64
			isOverriding = name == overrideFunction;
#else
			isOverriding = name.find("_" + overrideFunction + "@") != std::string::npos;
#endif
			if(isOverriding)
				file << "// ";
		}

		file << "#pragma comment(linker, \"/export:" << name << "=" << exportLinkDLL << "." << name << "\")" << std::endl;
	}

	file.close();
}

/*
This small program is a development utility for the ScreamAPI project.
When you run it, it asks you to select a DLL file. It then reads the PE header
of the DLL and generates appropriate 32 or 64 bit linker export headers.
These header files are then saved in the same directory as the provided DLL.
To generate 32-bit exports, you must run the 32-bit version of this utility.
Similarly, run 64-bit version to generate 64-bit exports.
*/
int main(){
	auto dllPath = getDLLpath();
	auto functions = getExportFunctions(dllPath);

#ifdef _WIN64
	generateHeader(dllPath, Architecture::Win64, functions);
#else
	generateHeader(dllPath, Architecture::Win32, functions);
#endif
}