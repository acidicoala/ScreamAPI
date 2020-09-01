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

std::vector<std::future<void>>asyncJobs;
std::future<void> awaitFuture;

/**
 * Initialize the loader and its dependencies.
 * @return true	if initialization was successfull
 *         false if there was an error in initialization
 */
bool init(){
	Logger::ovrly("Loader init");

	curl_global_init(CURL_GLOBAL_ALL);

	// Create directory if it doesn't exist already
	auto success = CreateDirectoryA(CACHE_DIR, NULL); // FIXME: Non-unicode function
	if(success || GetLastError() == ERROR_ALREADY_EXISTS){
		return true;
	} else{
		Logger::error("Failed to create '%s' directory. Error code: %d", CACHE_DIR, GetLastError());
		return false;
	}
}

void shutdown(){
	Logger::ovrly("Loader shutdown");
	curl_global_cleanup();

	if(!Config::CacheIcons()){
		if(!RemoveDirectoryA(CACHE_DIR))
			Logger::error("Failed to remove %s directory. Error code: %d", CACHE_DIR, GetLastError());
	}
}

// Helper utility to generate icon path based on the AchievementID
std::string getIconPath(Overlay_Achievement& achievement){
	return CACHE_DIR"\\" + std::string(achievement.AchievementId) + ".png";
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

	return (int) contentLength;
}

// Simple helper function to load an image into a DX11 texture with common settings
void loadIconTexture(Overlay_Achievement& achievement){
	Logger::debug(__func__);

	auto iconPath = getIconPath(achievement);

	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(iconPath.c_str(), &image_width, &image_height, NULL, 4);
	if(image_data == NULL)
		return;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	Overlay::pD3D11Device->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
#pragma warning(suppress: 6387)
	Overlay::pD3D11Device->CreateShaderResourceView(pTexture, &srvDesc, &achievement.IconTexture);
	pTexture->Release();

	stbi_image_free(image_data);
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
	return (int) size.QuadPart;
}

void downloadIconIfNecessary(Overlay_Achievement& achievement){
	auto iconPath = getIconPath(achievement);

	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	auto fileAttributes = GetFileAttributesExA(iconPath.c_str(), GetFileExInfoStandard, &fileInfo);
	if(fileAttributes){
		// File exists
		if(getLocalFileSize(fileInfo) == getOnlineFileSize(achievement.UnlockedIconURL)){
			Logger::ovrly("Using cached icon: %s", iconPath.c_str());
		} else{
			// Download the file again if the local version is different from online one
			downloadFile(achievement.UnlockedIconURL, iconPath.c_str());
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
// Asynchronously downloads the icons and loads them into textures
void asyncLoadIcons(Achievements& achievements){
	if(init()){
		for(auto& achievement : achievements){
			asyncJobs.emplace_back(std::async(std::launch::async, downloadIconIfNecessary, std::ref(achievement)));
		}
		awaitFuture = std::async(std::launch::async, [&] (){
			for(auto& job : asyncJobs){
				// Asynchronously wait for all other async jobs to be completed
				job.wait();
			}
			asyncJobs.clear();
			Loader::shutdown();
		});
	}
}

}