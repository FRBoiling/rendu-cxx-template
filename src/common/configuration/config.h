/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RENDU_CONFIG_H_
#define RENDU_CONFIG_H_

#include "define.h"
#include <string>
#include <vector>

class RD_COMMON_API ConfigMgr
{
    ConfigMgr() = default;
    ConfigMgr(ConfigMgr const&) = delete;
    ConfigMgr& operator=(ConfigMgr const&) = delete;
    ~ConfigMgr() = default;

public:
    /// Method used only for loading main configuration files (bnetserver.conf and worldserver.conf)
    bool LoadInitial(std::string file, std::vector<std::string> args, std::string& error);
    bool LoadAdditionalFile(std::string file, bool keepOnReload, std::string& error);

    /// Overrides configuration with environment variables and returns overridden keys
    std::vector<std::string> OverrideWithEnvVariablesIfAny();

    static ConfigMgr* instance();

    bool Reload(std::vector<std::string>& errors);

    std::string GetStringDefault(std::string const& name, const std::string& def, bool quiet = false) const;
    bool GetBoolDefault(std::string const& name, bool def, bool quiet = false) const;
    int32 GetIntDefault(std::string const& name, int32 def, bool quiet = false) const;
    int64 GetInt64Default(std::string const& name, int64 def, bool quiet = false) const;
    float GetFloatDefault(std::string const& name, float def, bool quiet = false) const;

    std::string const& GetFilename();
    std::vector<std::string> const& GetArguments() const;
    std::vector<std::string> GetKeysByString(std::string const& name);

private:
    template<class T>
    T GetValueDefault(std::string const& name, T def, bool quiet) const;
};

#define sConfigMgr ConfigMgr::instance()

#endif //RENDU_CONFIG_H_
