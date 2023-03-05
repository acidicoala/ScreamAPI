# Copies EGS addon python scripts into the scream api directory
# and launches mitmweb with the addon.

Set-Location (Get-Item $PSScriptRoot)

$ScreamApiDir = "$env:LocalAppData/acidicoala/koalageddon/unlockers/ScreamAPI/"

Copy-Item -Path "./*.py" -Destination $ScreamApiDir

$UpstreamParams = "--mode upstream:127.0.0.1:8888 --ssl-insecure"

Start-Process "$ScreamApiDir/mitmproxy.exe" -ArgumentList "-s '$ScreamApiDir/egs_addon.py' --listen-port 9999 "
