# Usage:
# PS ..\ScreamAPI> .\build.ps1 64 Debug

Set-Location (Get-Item $PSScriptRoot)

. ./KoalaBox/build.ps1 ScreamAPI @args

Build-Project
