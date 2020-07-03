# ScreamAPI
#### A Legit DLC Unlocker for Epic Games
Welcome to the ScreamAPI repository.

### What is ScreamAPI?
ScreamAPI is a DLC unlocker for the games that are legitimately owned in your Epic Games account.
It attempts to fool the game that uses Epic Online Services Software Development Kit (EOSSDK)
into thinking that you own the desired DLCs. However, ScreamAPI does not modify the rest of the EOSSDK,
hence features like multiplayer, achievements, etc. remain fully functional.

### How do I install it?
There are several simple steps required to install and use the ScreamAPI.
1.	Determine whether the game is built for 32-bit or 64-bit architecture.
	There are several ways to do it which can be easily found on google.
	Since most modern games are built for 64-bit architecture, we will be
	dealing with 64-bit binaries in this example. However in case of 32-bit
	architecure you will be dealing with 32-bit version of the same binaries.
2.	Rename the original `EOSSDK-Win64-Shipping.dll` typically found in game's binary folder into 
	`EOSSDK-Win64-Shipping_o.dll` by adding 2 characters: `_o`.
3.	Download the latest DLL from [GitHub releases](https://github.com/acidicoala/ScreamAPI/releases/latest).
	The DLL architecture (Win32 vs Win64) must correspond to the one found in the first step.
4.	Place the downloaded dll file into the game's binary folder, alongside the original
	`EOSSDK-Win64-Shipping_o.dll`. This completes the installation process. Pretty simple, right?
	Now go launch the game the way you would normally launch it and check if all DLCs have been unlocked.

##### Optional: Adding configuration file
By default ScreamAPI will attempt to unlock all DLCs that game requests. However,
in case it fails to do so, you configure it's behaviour by adding a config file.
You can get it from [here](ScreamAPI/ScreamAPI.ini), and place it in the same folder
where you placed the dll. Be sure to name the config file as `ScreamAPI.ini`.
The description of each option is described in the config file itself
in the corresponding comment that starts with `;`. In the config file you can enable things like
logging and selective DLC ownership.

### Will I get banned for it?
Epic Games or a game publisher will obviously have the right to do so, as modifying game components
is a violation of Epic Games store [EULA](https://www.epicgames.com/store/en-US/eula). However, if you keep this to yourself,
don't publicise your usage of it and don't try messing with games that have Anti-Cheats
like BattleEye, Easy Anti-Cheat, etc. you should be fine. Either way, as mentioned in the project's
[license](LICENSE.txt) agreement of this project, I take no responisbility for the damages you might sustain.
All I can say is that I have tried and used it in several games and have not been banned
or punished in any shape or form.

### For developers

Check out the [wiki](https://github.com/acidicoala/ScreamAPI/wiki) section of this GitHub project
if you are interested in learning how this framework works or wish to contribute code to the project.

### Acknowledgements
ScreamAPI would not be possible without [nEOS](https://gitlab.com/infogram/neos) by **infogram**.
It served as a great educational resource of how EOS SDK works. This project borrows several ideas
from nEOS, so I extend my gratitude to infogram for making his EOS emulator publicly available.
If you want to learn in more detail how the entire EOS SDK works (as opposed to just DLC unlocking)
then I encourage you to check out the project's [Gitlab repository](https://gitlab.com/infogram/neos).

Additionally, ScreamAPI owes its existence to **CreamAPI** - a DLC unlocker for legitimately owned Steam games.
Cream API served as an insipration to me, for it was the first project that demonstrated 
that it is possible to unlock DLCs, while leaving the rest of the Steam integration intact.
Hence, the name of this project is a tribute to the CreamAPI.

### License
This software is licensed under [Zero Clause BSD](https://en.wikipedia.org/wiki/BSD_licenses#0-clause_license_(%22Zero_Clause_BSD%22))
license, terms of which are available in [LICENSE.txt](LICENSE.txt)

This software makes use of  [inih](https://github.com/benhoyt/inih)
by [benhoyt](https://github.com/benhoyt),
licensed under [BSD](https://github.com/benhoyt/inih/blob/master/LICENSE.txt).