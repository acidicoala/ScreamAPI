# 🐨 ScreamAPI 😱

**Legit DLC Unlocker for Epic Online Services**

📥 [Download the latest release](https://github.com/acidicoala/ScreamAPI/releases/latest)

💬 [Official forum topic](https://cs.rin.ru/forum/viewtopic.php?p=2161197#p2161197)

📊 ~~[Compatibility spreadsheet](https://docs.google.com/spreadsheets/d/1yCm2KWSFlV283SI35QpI86v68bFnK7MwxicKBEihIJM)~~ (Discontinued)

📃 [Test result form](https://docs.google.com/forms/d/e/1FAIpQLSeEGotx8ThaHQK8ywW_UPwTJysUZWnCPIDNBJjNpvXAYLPBEg/viewform)

🗃 [DLC database](https://scream-db.web.app/)

## ℹ Introduction

> Hint: Click on spoilers (`▶`) to expand their content

<details>
<summary>What is ScreamAPI?</summary>

ScreamAPI is a DLC unlocker for the games that are legitimately owned in your Epic Games account. It attempts to fool games that use Epic Online Services Software Development Kit (EOSSDK) into thinking that you own the desired DLCs. However, ScreamAPI does not modify the rest of the EOSSDK, hence features like multiplayer, achievements, etc. remain fully functional.

</details>

<details>
<summary>Which games are supported?</summary>

Only the games that use Epic Online Services Software Development Kit (EOS SDK) for the DLC ownership verification are supported. So if a game's installation directory does not contain any _EOSSDK-WinXX-Shipping.dll_ files, then it's definitely not supported. Even if the game uses EOS SDK DLL, it's not guaranteed to be supported because each game might implement its own additional verification checks. Therefore, you have to first research the game's topic, to see if it supports unlocking.

Additionally, there are several points to bear in mind when it comes to ScreamAPI and Epic Games Store:

* ScreamAPI most definitely will not work with games that use 3rd party DRM, such as games from Ubisoft, Rockstar, etc. Furthermore, ScreamAPI is also unlikely to unlock anything in Free-To-Play games since they typically store all player data on the corresponding game server and hence all the checks are server-side.
* ScreamAPI will not work with games that employ additional ownership protection or if the game is using alternative DLC verification mechanism (like Borderlands 3). However, it can unlock DLCs in Denuvo-protected games if there are no other integrity checks (like Death Stranding)
* ScreamAPI is unlikely to work with games that use Anti-Cheat, since they typically detect any DLL/EXE that has been tampered with. Sometimes it is possible to disable an anti-cheat, but that typically entails the loss of online capabilities. Search in the respective game topic for more information about how to disable anti-cheat.
* Some games include the DLC files in their base game, regardless of whether you own the DLC or not (like World War Z). This is the ideal scenario for ScreamAPI. However, some games download additional files only after a user has bought the corresponding DLC (like Civilization VI). In this case, not only you will need to install ScreamAPI, but you also have to get the additional DLC files elsewhere and put them into the game folder.
* Some games don't use any DRM at all, in which case ScreamAPI is useless. All you need to do is to get the DLC files elsewhere and put them into the game folder.

[Google Sheets document]: https://docs.google.com/spreadsheets/d/1yCm2KWSFlV283SI35QpI86v68bFnK7MwxicKBEihIJM

[Google Forms document]: https://docs.google.com/forms/d/e/1FAIpQLSeEGotx8ThaHQK8ywW_UPwTJysUZWnCPIDNBJjNpvXAYLPBEg/viewform

</details>

<details>
<summary>Will I get banned for it?</summary>

Epic Games or a game publisher will obviously have the right to do so, as modifying game components is a violation of Epic Games store [EULA](https://www.epicgames.com/store/en-US/eula). However, if you keep this to yourself, don't publicize your usage of it and don't try messing with games that have Anti-Cheats like BattleEye, Easy Anti-Cheat, etc. you should be fine. Either way, as mentioned in the [license](https://github.com/acidicoala/ScreamAPI/blob/master/LICENSE.txt) agreement of this project, I take no responsibility for the damages you might sustain. All I can say is that I have tried and used it in several games and have not been banned or punished in any shape or form.

</details>

## 🛠 Installation Instructions

> Note: These examples assumes a 64-bit game. If you are interested in a 32-bit game, then repeat the same steams with 32-bit versions of the binaries instead.

ScreamAPI supports 2 modes of operation: **Proxy** mode, and **Hook** mode.


<details><summary><b>Proxy mode</b></summary>

In **Proxy** mode ScreamAPI DLL assumes the role of a proxy DLL that gets loaded by the game which thinks it has loaded the original DLL. ScreamAPI DLL in this mode will redirect most of the function calls to the original DLL. Kind of like how web proxy redirects requests to the original server. This is a classical, time-proven installation method that is stable, but has a number of shortcomings. The primary of them is that the unlocker binaries may be overwritten after a game updated, requiring re-installation of DLL. Another one is that some users don't feel like reading instructions properly, and just go ahead replacing the original DLL, instead of renaming it. Or they rename it incorrectly, for example by adding number zero, instead of letter 0. Below are the instructions for the Proxy mode:

1. Find a file _EOSSDK-Win64-Shipping.dll_ in your game's installation folder.
1. Rename it by adding 2 characters: **_o** (notice that it is a letter **o** at the end, not a numeral **0**). In the end it should be named as _EOSSDK-Win64-Shipping_o.dll_.
1. Download the latest release zip from [GitHub Releases]. In case you have a hard time finding it, click on the **Assets** spoiler, then click on the link that looks like _ScreamAPI-vX.Y.Z.zip_
1. Open up the downloaded zip and unpack the _EOSSDK-Win64-Shipping.dll_ file alongside the original **EOSSDK-Win64-Shipping_o.dll** in the game's binary folder.

<details><summary>64-bit example</summary>

![64-bit example](https://i.ibb.co/60w1wtt/install.webp)
</details>
<details><summary>32-bit example</summary>

![32-bit example](https://i.ibb.co/1rdvN3B/install.webp)
</details>

</details>

---

<details><summary><b>Hook mode</b></summary>

In **Hook** mode ScreamAPI DLL is injected into the game process and dynamically patches the functions of interest in the original DLL, leaving the rest untouched. The unlocker DLL can be loaded automatically by exploiting DLL search order, which is what project Koaloader is used for. Koaloader DLL can be disguised as a system DLL, so that it can be automatically loaded by the game. In turn, Koaloader will load the ScreamAPI DLL. This has the advantage of not modifying any binary files on the disc, hence the unlocker will remain intact after updates. It should be also easier to install for some users. The only disadvantage is that Koaloader requires per-game configuration. Below are the instructions for the Hook mode (in combination with Koaloader):

1. Download the latest ScreamAPI release zip from [GitHub Releases].
2. Open up the downloaded zip, unpack the _EOSSDK-Win64(32)-Shipping.dll_ and rename it however you like, for example to *ScreamAPI.dll*
3. Place the ScreamAPI DLL anywhere you like, for example in game's root directory.
4. Download the latest Koaloader release zip from [GitHub Releases](https://github.com/acidicoala/Koaloader/releases/latest).
5. Pick a DLL that is supported by the target game. In this example, we will select *version-64/version.dll*. Place this DLL in the game directory where a game first attempts to find a DLL, such as the game's binary directory.
6. Place the `Koaloder.json` config file next to the Koaloader DLL and configure Koaloader as described in its [README](https://github.com/acidicoala/Koaloader/blob/master/README.md). Most importantly, set the `modules` option to point to the path of the ScreamAPI dll:
     ```json
    "modules": [
        {
          "path": "ScreamAPI.dll"
        }
    ]
    ```
   
You can also find an example setup [here](https://cs.rin.ru/forum/viewtopic.php?p=2548344#p2548344)
</details>

This completes the installation process. Now you can launch the game from Epic Game Store or using the shortcut created by EGS, and check if all DLCs have been unlocked. Sometimes you may have to accept a game's request to connect your EGS account in order to unlock DLCs, which some games require regardless of ScreamAPI.
**Keep in mind that you have to repeat this process again if the game gets updated by the Epic Games launcher.**

> **If the game is crashing...**<br>
If the game is crashing or not opening as expected after installing ScreamAPI, then try to download and install the latest [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017, 2019, and 2022](https://support.microsoft.com/en-gb/help/2977003/the-latest-supported-visual-c-downloads)
> <details><summary>Download page</summary>
>
> ![Download page](https://i.ibb.co/n6K0X27/redist.jpg)
> </details>

[GitHub Releases]: https://github.com/acidicoala/ScreamAPI/releases/latest

## ⚙ Configuration

ScreamAPI does not require any manual configuration. By default, it uses most reasonable options and tries to unlock all DLCs that it can. However, there might be circumstances in which you need more custom-tailored behaviour. In this case you can use a configuration file `ScreamAPI.json` that you can find in the release zip. To use it, just drop it alongside the ScreamAPI dll (EOSSDK-WinXX-Shipping.dll). It will be read upon each launch of a game. Configuration file itself is optional, but options within the file are not. In the absence of the config file, default value specified below will be used.

The description of each available option is presented below:

- `version`: This field may be used by ScreamAPI to identify how to parse the config file. This allows ScreamAPI to parse older config files in a backward compatible manner. Therefore, **this field must not be edited manually**! Default: `2`.
- `logging`: Enables or disables logging into a _ScreamAPI.log_ file. Default: `false`.
- `eos_logging`: Enables or disables interception of EOS SDK logs. Can cause issues when enabled in *hook* mode. Default: `false`.
- `block_metrics`: Blocks reporting of analytics and usage metrics by the game to Epic Online Services. Metrics sent by EOS SDK itself remain unaffected. Default: `false`.
- `catalog_items`: Options in this object specify behaviour when games use `EOS_Ecom_QueryOwnership` function.
  - `unlock_all`: Controls if all items should be automatically unlocked. Default: `true`.
  - `override`: Specifies items ids¹ that override the behaviour of `unlock_all`. If `unlock_all` is _true_, then items ids in this array will be locked. If `unlock_all` is _false_, then items ids in this array will be unlocked. Default: `[]` (empty array).
- `entitlements`: Options in this object specify behaviour when games use `EOS_Ecom_QueryEntitlements` function.
  - `unlock_all`: Controls if all entitlements that were requested by their IDs should be automatically unlocked. Default: `true`.
  - `auto_inject`: Controls if ScreamAPI should fetch from online database a list of all available entitlement IDs, when a game doesn't provide them itself. Default: `true`.
  - `inject`: Specifies which entitlement ids¹ to inject when a game asks for a list of entitlements that a user owns. Default: `[]` (empty array).

¹ Item/Entitlement IDs can be obtained from https://scream-db.web.app/. The log file will contain the link to the exact page which lists the game's DLCs

## 👋 Acknowledgements

ScreamAPI makes use of the following open source projects:

- [C++ Requests](https://github.com/libcpr/cpr)
- [JSON for Modern C++](https://github.com/nlohmann/json)
- [PolyHook 2](https://github.com/stevemk14ebr/PolyHook_2_0)
- [spdlog](https://github.com/gabime/spdlog)

## 📄 License

This software is licensed under [BSD Zero Clause  License], terms of which are available in [LICENSE.txt]

[BSD Zero Clause  License]: https://choosealicense.com/licenses/0bsd/
[LICENSE.txt]: LICENSE.txt
