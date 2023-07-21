<div align="center">

<img src="https://github.com/The-AniLab/AniLab-Discord-Bot/assets/87577447/4022aa44-6c54-4f29-a7ce-4a541c11eca3" alt="Anilab" width="150" />

# Anilab - Discord Bot

The ultimate anime bot for all servers! <br />
(Under-construction)

[![CMake](https://github.com/The-AniLab/AniLab-Discord-Bot/actions/workflows/cmake.yaml/badge.svg?branch=master)](https://github.com/The-AniLab/AniLab-Discord-Bot/actions/workflows/cmake.yaml)
[![MSBuild](https://github.com/The-AniLab/AniLab-Discord-Bot/actions/workflows/msbuild.yaml/badge.svg?branch=master)](https://github.com/The-AniLab/AniLab-Discord-Bot/actions/workflows/msbuild.yaml) 
[![GitHub issues](https://img.shields.io/github/issues/harshfeudal/Tsukasa-Tsukuyomi)](https://github.com/The-AniLab/AniLab-Discord-Bot/issues) <br />
[![Discord](https://img.shields.io/discord/1103255588051357696?style=flat&logo=discord)](https://discord.gg/pAZZfqC5ST)
[![GitHub license](https://img.shields.io/github/license/harshfeudal/Tsukasa-Tsukuyomi?color=brightgreen)](https://github.com/https://github.com/The-AniLab/AniLab-Discord-Bot/blob/main/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/The-AniLab/AniLab-Discord-Bot?color=brightgreen)](https://github.com/The-AniLab/AniLab-Discord-Bot/stargazers)

</div>

### 📚 Libraries and versions

These are the libraries that we're using in this project:

- [Dpp](https://github.com/brainboxdotcc/DPP) - version `10.0.24` (latest)
- [Spdlog](https://github.com/gabime/spdlog) - version `1.11.0` (latest)
- [Base64 (extracted)](https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594)
- [Sqlite3](https://www.sqlite.org/) - version `3.42.0` (latest)

### 👷 Support platform
- Microsoft Windows 10 `x64/x86`
- Visual Studio platform `x64/x86`
- C++ 17 `ISO/IEC 14882`

### ✨ Support us!

#### 1. Translation

We need your help! We are currently using Google Translate to translate our [language data](https://github.com/The-AniLab/AniLab-Discord-Bot/tree/master/languages). If you found that the our translation is incorrect, compare with the original `en-us.json`, please help us to fix by creating a pull request. 

Also, if you want to add more language, please **copy** exactly what we wrote in `en-us.json`, and translate. You can see our demonstration below:

```json
"LANGUAGE":
{
    "title": "please_translate_here",
    "description": "please_translate_here"
},
```

`please_translate_here` is the place where you can translate.

#### 2. Add new languages

If you want to add more languages, we would glad to see your translation. For adding new language, please follow these steps below:

**a. Open [language.cpp](https://github.com/The-AniLab/AniLab-Discord-Bot/blob/master/src/cmd_common/language.cpp) and copy and paste like this:**

```cpp
else if (language == "japanese")
{
    if (check_user)
        database.deleteRecord("configuration", "id=" + target_user);

    change_language = "ja-jp";
    database.insert("configuration", target_user + ", '" + change_language + "'");
}
// Copy from here ...
else if (language == "{change_here}")
{
    if (check_user)
        database.deleteRecord("configuration", "id=" + target_user);

    change_language = "{change_here}";
    database.insert("configuration", target_user + ", '" + change_language + "'");
}
// ... to here, and paste like a "chain" else-if statement
```

`{change_here}` variable is the variable you **should** change when you added.

**b. Create your `.json` file in our [language data](https://github.com/The-AniLab/AniLab-Discord-Bot/tree/master/languages)**

Please make sure to name the file like this:

```
{Language}-{regional}.json
```

For example: `en-uk.json`: Language is English, regional is United Kingdom

**c. Go to [cores/cmd_layout.h](https://github.com/The-AniLab/AniLab-Discord-Bot/blob/master/cores/cmd_layout.h) and do like this:**

```cpp
inline std::map<std::string, Command> commands
{ 
    // Common commands
    {
        "ping",
        {
            "Check bot latency", ping 
        }
    },
    {
        "language",
        {
            "Configure bot language to show to you", language,
            {
                dpp::command_option(dpp::co_string, "option", "Your language option", true)
                    .add_choice(dpp::command_option_choice("English", std::string("english")))
                    // Copy this line below and change where you found {change_here} ...
                    .add_choice(dpp::command_option_choice("{change_here}", std::string("{change_here}")))
                    // ... end of your copy
            }
        }
    },
};
```

`{change_here}` is the variable you should change like what we did above.

Thank you so much for collaborating ^^

### 🤝 Contribution and Pull Request

I'd probably be glad to see if you have anything new to help and support me. To do that, please make one and before you do that, just make sure you've tested the code. I won't bite you if you do it wrong, but just make sure that you have to test it clearly before I merge it.

Please follow [our Code of Conduct](https://github.com/The-AniLab/AniLab-Discord-Bot/blob/master/CODE_OF_CONDUCT.md)!

### 💎 Any interested?
Your star is our inspiration. Leave us a star ⭐️ can let us have more inspiration to make this project more better for everyone;
Or, if you want to contact us faster, you can join our Discord server!

[![Discord](https://img.shields.io/discord/1103255588051357696?style=flat&logo=discord)](https://discord.gg/pAZZfqC5ST)

### 📝 License

- This project license is based on both [MIT](https://github.com/The-AniLab/AniLab-Discord-Bot/blob/master/MIT-LICENSE.md) and [GPL-3.0](https://github.com/The-AniLab/AniLab-Discord-Bot/blob/master/GNU-LICENSE.md) licenses.
- [Dotenv](https://github.com/The-AniLab/AniLab-Discord-Bot/blob/master/cores/client_dotenv.h) is written by [Harshfeudal](https://github.com/harshfeudal) and the license based on MIT license.

<div align="center">
    <span>
            - © 2023 The Anilab Development Team. All rights reserved. -
    </span>
</div>
