
| Branch   | Workflow |
|-------------|----------|
| `master` | [![Build Project](https://github.com/acidicoala/ScreamAPI/actions/workflows/build-project.yml/badge.svg?branch=master)](https://github.com/acidicoala/ScreamAPI/actions/workflows/build-project.yml?query=branch%3Amaster) |
| `develop` | [![Build Project](https://github.com/acidicoala/ScreamAPI/actions/workflows/build-project.yml/badge.svg?branch=develop)](https://github.com/acidicoala/ScreamAPI/actions/workflows/build-project.yml?query=branch%3Adevelop) |

# 🐨 ScreamAPI

**Legit DLC Unlocker for Epic Online Services**

📥 [Download Latest Release](https://github.com/acidicoala/ScreamAPI/releases/latest)

💬 [Official Forum Topic](https://cs.rin.ru/forum/viewtopic.php?p=2161197#p2161197)

📊 [Compatibility Spreadsheet](https://docs.google.com/spreadsheets/d/1yCm2KWSFlV283SI35QpI86v68bFnK7MwxicKBEihIJM)

📃 [Test Result Form](https://docs.google.com/forms/d/e/1FAIpQLSeEGotx8ThaHQK8ywW_UPwTJysUZWnCPIDNBJjNpvXAYLPBEg/viewform)

🗃 [DLC Database](https://scream-db.web.app/)

## ℹ Introduction

> Hint: Click on spoilers (`▶`) to expand their content

<details>
<summary>What is ScreamAPI?</summary>

ScreamAPI is a DLC unlocker for the games that are legitimately owned in your Epic Games account. It attempts to fool games that uses Epic Online Services Software Development Kit (EOSSDK) into thinking that you own the desired DLCs. However, ScreamAPI does not modify the rest of the EOSSDK, hence features like multiplayer, achievements, etc. remain fully functional.

</details>

<details>
<summary>Which games are supported?</summary>

Only the games that use Epic Online Services Software Development Kit (EOS SDK) for the DLC ownership verification are supported. So if a game's installation directory does not contain any _EOSSDK-WinXX-Shipping.dll_ files, then it's definitely not supported. Even if the game uses EOS SDK DLL, it's not guaranteed to be supported because each game might implement its own additional verification checks. Therefore, you have to first research the game's topic, to see if it supports unlocking.

I have created a [Google Sheets document] which contains the information about which games are compatible with ScreamAPI. If a game is compatible then it means that it is possible to unlock all or some DLCs with ScreamAPI. The spreadsheet contains 2 main sheets: the _Official Sheet_ and the Community Sheet. The Official Sheet is hand-filled by me based on my test results and test results of other users of this forum. The _Community Sheet_ is auto-filled by a [Google Forms document], which is accessible to everybody, so feel free to submit your feedback via the form. You can submit forms anonymously and as many times as you want.

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

There are several simple steps required to install and use the ScreamAPI.

> Note: This example assumes a 64-bit game. If you are interested in a 32-bit game, then repeat the same steams with 32-bit versions of the binaries instead.

1. Find a file _EOSSDK-Win64-Shipping.dll_ in your game's installation folder.
1. Rename it by adding 2 characters: **_o** (notice that it is a letter **o** at the end, not a numeral **0**). In the end it should be named as _EOSSDK-Win64-Shipping_o.dll_.
1. Download the latest release zip from [GitHub Releases](https://github.com/acidicoala/ScreamAPI/releases/latest). In case you have a hard time finding it, click on the **Assets** spoiler, then click on the link that looks like _ScreamAPI-vX.Y.Z.zip_
1. Open up the downloaded zip and unpack the _EOSSDK-Win64-Shipping.dll_ file alongside the original **EOSSDK-Win64-Shipping_o.dll** in the game's binary folder.

<details><summary>64-bit example</summary>

![64-bit example](https://i.ibb.co/60w1wtt/install.webp)
</details>
<details><summary>32-bit example</summary>

![32-bit example](https://i.ibb.co/1rdvN3B/install.webp)
</details>

This completes the installation process. Now you can launch the game from Epic Game Store or using the shortcut created by EGS, and check if all DLCs have been unlocked. Sometimes you may have to accept a game's request to connect your EGS account in order to unlock DLCs, which some games require regardless of ScreamAPI.
**Keep in mind that you have to repeat this process again if the game gets updated by the Epic Games launcher.**

> **If the game is crashing...**<br>
If the game is crashing or not opening as expected after installing ScreamAPI, then try to download and install the latest [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-gb/help/2977003/the-latest-supported-visual-c-downloads)
> <details><summary>Download page</summary>
>
> ![Download page](https://i.ibb.co/7NqHycx/redist.webp)
> </details>

## ⚙ Configuration

ScreamAPI does not require any manual configuration. By default, it uses most reasonable options and tries to unlock all DLCs that it can. However, there might be circumstances in which you need more custom-tailored behaviour. In this case you can use a configuration file `ScreamAPI.json` that you can find in the release zip. To use it, just drop it alongside the ScreamAPI dll (EOSSDK-WinXX-Shipping.dll). It will be read upon each launch of a game. Configuration file itself is optional, but options within the file are not. In the absence of the config file, default value specified below will be used.

The description of each available option is presented below:

- `version`: This field is used by ScreamAPI to identify how to parse the config file. This allows ScreamAPI to parse older config files in a backward compatible manner. Therefore, **this field must not be edited manually**! Default: `1`.
- `logging`: Enables or disables logging into a _ScreamAPI.log_ file. Default: `false`.
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

- [spdlog](https://github.com/gabime/spdlog)
- [JSON for Modern C++](https://github.com/nlohmann/json)
- [C++ Requests](https://github.com/libcpr/cpr)

## 📄 License

This software is licensed under [BSD Zero Clause  License], terms of which are available in [LICENSE.txt]

[BSD Zero Clause  License]: https://choosealicense.com/licenses/0bsd/
[LICENSE.txt]: LICENSE.txt