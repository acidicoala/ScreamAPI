# Usage:
#   build.ps1 32 Debug
#   build.ps1 32 Release
#   build.ps1 64 Debug
#   build.ps1 64 Release

$arch = $args[0]
if ($arch -notmatch '^(32|64)$') {
    throw "Invalid architecute. Expected: '32' or '64'. Got: '$arch'"
}
$platform = If ($arch -eq '32') { 'Win32' } Else { 'x64' }

$config = $args[1]
if ($config -notmatch '^(Debug|Release|RelWithDebInfo)$') {
    throw "Invalid architecute. Expected: 'Debug' or 'Release' or 'RelWithDebInfo'. Got: '$config'"
}

Set-Location (Get-Item $PSScriptRoot)

$Env:SCREAM_API_VERSION = "2.0.1"
$Env:BUILD_DIR = "build\$arch"
if ($config -eq 'Debug') {
    $Env:VERSION_SUFFIX = "-debug"
}

cmake -G "Visual Studio 16 2019" -A $platform -B "$Env:BUILD_DIR"

cmake --build "$Env:BUILD_DIR" --config $config