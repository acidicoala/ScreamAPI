# Example usage: test.ps1 32 Debug "D:\Epic\SonicMania" "ns:id1:id2"

$arch = $args[0]
$config = $args[1]
$game_dir = $args[2]
$app_ids = $args[3]

Set-Location (Get-Item $PSScriptRoot).Parent

Copy-Item "./build/$arch/$config/EOSSDK-Win$arch-Shipping.dll" -Destination "$game_dir"

Start-Process "com.epicgames.launcher://apps/${app_ids}?action=launch&silent=true"