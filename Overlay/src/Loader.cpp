#include "pch.h"
#include "Loader.h"
#include "Overlay.h"
#include <fstream>
#include <future>
#include <Config.h>

// Instructions on how to build libcurl on Windows can be found here:
// https://www.youtube.com/watch?reload=9&v=q_mXVZ6VJs4
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Normaliz.lib")
#define CURL_STATICLIB
#include "curl/curl.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"

namespace Loader{

#define CACHE_DIR ".ScreamApi_Cache"

/**
 * Initialize the loader and its dependencies.
 * @return true	if initialization was successfull
 *         false if there was an error in initialization
 */
bool init(){
	// Init curl first
#pragma warning(suppress: 26812) // Unscoped enum...
	CURLcode errorCode = curl_global_init(CURL_GLOBAL_ALL);
	if(errorCode != CURLE_OK){
		// Something went wrong
		Logger::error("Loader: Failed to initialize curl. Error code: %d", errorCode);
		return false;
	}

	// Create directory if it doesn't already exist 
	auto success = CreateDirectoryA(CACHE_DIR, NULL); // FIXME: Non-unicode function
	if(success || GetLastError() == ERROR_ALREADY_EXISTS){
		Logger::ovrly("Loader: Successfully initialized");
		return true;
	} else{
		Logger::error("Loader: Failed to create '%s' directory. Error code: %d", CACHE_DIR, GetLastError());
		return false;
	}
}

void shutdown(){
	curl_global_cleanup();

	if(!Config::CacheIcons()){
		if(!RemoveDirectoryA(CACHE_DIR))
			Logger::error("Failed to remove %s directory. Error code: %d", CACHE_DIR, GetLastError());
	}

	Logger::ovrly("Loader: Shutdown");
}

// Helper utility to generate icon path based on the AchievementID
std::string getIconPath(Overlay_Achievement& achievement){
	return CACHE_DIR"\\" + std::string(achievement.AchievementId) + ".png";
}

// Simple helper function to load an image into a DX11 texture with common settings
void loadIconTexture(Overlay_Achievement& achievement){
	static std::mutex loadIconMutex;
	{ // Code block for lock_guard destructor to release lock
		std::lock_guard<std::mutex> guard(loadIconMutex);

		Logger::ovrly("Loading icon texure for achievement: %s", achievement.AchievementId);
		auto iconPath = getIconPath(achievement);

		// Load from disk into a raw RGBA buffer
		int image_width = 0;
		int image_height = 0;
		auto* image_data = stbi_load(iconPath.c_str(), &image_width, &image_height, NULL, 4);
		if(image_data == NULL){
			Logger::error("Failed to load icon: %s. Failure reason: %s", iconPath.c_str(), stbi_failure_reason());
			return;
		}

		// Create texture
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		HRESULT result;
		ID3D11Texture2D* pTexture = nullptr;
		if(FAILED(result = Overlay::gD3D11Device->CreateTexture2D(&desc, nullptr, &pTexture))){
			Logger::error("Failed to load the texture. Error code: 0x%x", result);
			stbi_image_free(image_data);
			return;
		}

		// Update subresource
		D3D11_BOX box{};
		box.right = image_width;
		box.bottom = image_height;
		box.back = 1;
		Overlay::gContext->UpdateSubresource(pTexture, 0, &box, image_data, desc.Width * 4, desc.Width * 4 * desc.Height);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if(FAILED(result = Overlay::gD3D11Device->CreateShaderResourceView(pTexture, &srvDesc, &achievement.IconTexture))){
			Logger::error("Failed to create shader resource view. Error code: %x", result);
		}

		// Clean up the memory
		pTexture->Release();
		stbi_image_free(image_data);
	}
}

// Downloads the online icon into local cache folder
void downloadFile(const char* url, const char* filename){
	FILE* file_handle;
	CURL* curl_handle = curl_easy_init();

	Logger::ovrly("Downloading icon to: %s", filename);

	errno_t err = fopen_s(&file_handle, filename, "wb");
	if(!err && file_handle != NULL){
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, file_handle);
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_perform(curl_handle);
		fclose(file_handle);
	} else{
		Logger::error("Failed to open file for writing: %s", filename);
	}

	curl_easy_cleanup(curl_handle);
}

int getLocalFileSize(WIN32_FILE_ATTRIBUTE_DATA& fileInfo){
	LARGE_INTEGER size;
	size.HighPart = fileInfo.nFileSizeHigh;
	size.LowPart = fileInfo.nFileSizeLow;
	return (int)size.QuadPart;
}

// Downloads only the file headers and retuns the size of the file
int getOnlineFileSize(const char* url){
	CURL* curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1);
	curl_easy_setopt(curl_handle, CURLOPT_NOBODY, 1);
	curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_perform(curl_handle);
	curl_off_t contentLength;
	curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &contentLength);
	curl_easy_cleanup(curl_handle);

	return (int)contentLength;
}

void downloadIconIfNecessary(Overlay_Achievement& achievement){
	// Return if the icon URL is invalid
	if(std::string(achievement.UnlockedIconURL).rfind("http", 0) == std::string::npos) {
		Logger::ovrly("Ignoring invalid icon URL: %s", achievement.UnlockedIconURL);
		return;
	}

	auto iconPath = getIconPath(achievement);

	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	auto fileAttributes = GetFileAttributesExA(iconPath.c_str(), GetFileExInfoStandard, &fileInfo);
	if(fileAttributes){
		if(Config::ValidateIcons()){
			// File exists
			if(getLocalFileSize(fileInfo) == getOnlineFileSize(achievement.UnlockedIconURL)) {
				Logger::ovrly("Using cached icon: %s", iconPath.c_str());
			} else{
				// Download the file again if the local version is different from online one
				downloadFile(achievement.UnlockedIconURL, iconPath.c_str());
			}
		} else {
			Logger::ovrly("Using cached icon: %s", iconPath.c_str());
		}
	} else if(GetLastError() == ERROR_FILE_NOT_FOUND){
		// File doesn't exist
		downloadFile(achievement.UnlockedIconURL, iconPath.c_str());
	} else{
		// File exists, but we could not read it's attributes.
		Logger::error("Failed to read file attributes. Error code: %d", GetLastError());
		// TODO: Use FormatMessage to print user-friendly error message?
		return;
	}

	loadIconTexture(achievement);

	if(!Config::CacheIcons()){
		if(!DeleteFileA(iconPath.c_str()))
			Logger::error("Failed to remove %s file. Error code: %d", iconPath.c_str(), GetLastError());
	}
}
// Asynchronously downloads the icons and loads them into textures in order to keep UI responsive
void AsyncLoadIcons(){
	if(Config::LoadIcons() && init()){
		static std::vector<std::future<void>>asyncJobs;

		for(auto& achievement : *Overlay::achievements){
			asyncJobs.emplace_back(std::async(std::launch::async, downloadIconIfNecessary, std::ref(achievement)));
		}
		static auto awaitFuture = std::async(std::launch::async, [&](){
			for(auto& job : asyncJobs){
				// Asynchronously wait for all other async jobs to be completed
				job.wait();
			}
			// Cleanup afterwards
			asyncJobs.clear();
			shutdown();
		});
	}
}

}