/*
* Created by boil on 2022/8/20.
*/

#include "config.h"
#include "log.h"
#include "string_convert.h"
#include <algorithm>
#include <boost/property_tree/ini_parser.hpp>
#include <cstdlib>
#include <memory>
#include <mutex>

namespace bpt = boost::property_tree;

namespace
{
    std::string _filename;
    std::vector<std::string> _additonalFiles;
    std::vector<std::string> _args;
    bpt::ptree _config;
    std::mutex _configLock;

    bool LoadFile(std::string const& file, bpt::ptree& fullTree, std::string& error)
    {
        try
        {
            bpt::ini_parser::read_ini(file, fullTree);

            if (fullTree.empty())
            {
                error = "empty file (" + file + ")";
                return false;
            }
        }
        catch (bpt::ini_parser::ini_parser_error const& e)
        {
            if (e.line() == 0)
                error = e.message() + " (" + e.filename() + ")";
            else
                error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
            return false;
        }

        return true;
    }

    // Converts ini keys to the environment variable key (upper snake case).
    // Example of conversions:
    //   SomeConfig => SOME_CONFIG
    //   myNestedConfig.opt1 => MY_NESTED_CONFIG_OPT_1
    //   LogDB.Opt.ClearTime => LOG_DB_OPT_CLEAR_TIME
    std::string IniKeyToEnvVarKey(std::string const& key)
    {
        std::string result;

        const char *str = key.c_str();
        size_t n = key.length();

        char curr;
        bool isEnd;
        bool nextIsUpper;
        bool currIsNumeric;
        bool nextIsNumeric;

        for (size_t i = 0; i < n; ++i)
        {
            curr = str[i];
            if (curr == ' ' || curr == '.' || curr == '-')
            {
                result += '_';
                continue;
            }

            isEnd = i == n - 1;
            if (!isEnd)
            {
                nextIsUpper = isupper(str[i + 1]);

                // handle "aB" to "A_B"
                if (!isupper(curr) && nextIsUpper)
                {
                    result += static_cast<char>(std::toupper(curr));
                    result += '_';
                    continue;
                }

                currIsNumeric = isNumeric(curr);
                nextIsNumeric = isNumeric(str[i + 1]);

                // handle "a1" to "a_1"
                if (!currIsNumeric && nextIsNumeric)
                {
                    result += static_cast<char>(std::toupper(curr));
                    result += '_';
                    continue;
                }

                // handle "1a" to "1_a"
                if (currIsNumeric && !nextIsNumeric)
                {
                    result += static_cast<char>(std::toupper(curr));
                    result += '_';
                    continue;
                }
            }

            result += static_cast<char>(std::toupper(curr));
        }
        return result;
    }

    Optional<std::string> EnvVarForIniKey(std::string const& key)
    {
        std::string envKey = "RD_" + IniKeyToEnvVarKey(key);
        char* val = std::getenv(envKey.c_str());
        if (!val)
            return std::nullopt;

        return std::string(val);
    }
}

bool ConfigMgr::LoadInitial(std::string file, std::vector<std::string> args,
                            std::string& error)
{
    std::lock_guard<std::mutex> lock(_configLock);

    _filename = std::move(file);
    _args = std::move(args);

    bpt::ptree fullTree;
    if (!LoadFile(_filename, fullTree, error))
        return false;

    // Since we're using only one section per config file, we skip the section and have direct property access
    _config = fullTree.begin()->second;

    return true;
}

bool ConfigMgr::LoadAdditionalFile(std::string file, bool keepOnReload, std::string& error)
{
    bpt::ptree fullTree;
    if (!LoadFile(file, fullTree, error))
        return false;

    for (bpt::ptree::value_type const& child : fullTree.begin()->second)
        _config.put_child(bpt::ptree::path_type(child.first, '/'), child.second);

    if (keepOnReload)
        _additonalFiles.emplace_back(std::move(file));

    return true;
}

std::vector<std::string> ConfigMgr::OverrideWithEnvVariablesIfAny()
{
    std::lock_guard<std::mutex> lock(_configLock);

    std::vector<std::string> overriddenKeys;

    for (bpt::ptree::value_type& itr: _config)
    {
        if (!itr.second.empty() || itr.first.empty())
            continue;

        Optional<std::string> envVar = EnvVarForIniKey(itr.first);
        if (!envVar)
            continue;

        itr.second = bpt::ptree(*envVar);

        overriddenKeys.push_back(itr.first);
    }

    return overriddenKeys;
}

ConfigMgr* ConfigMgr::instance()
{
    static ConfigMgr instance;
    return &instance;
}

bool ConfigMgr::Reload(std::vector<std::string>& errors)
{
    std::string error;
    if (!LoadInitial(_filename, std::move(_args), error))
        errors.push_back(std::move(error));

    for (std::string const& additionalFile : _additonalFiles)
        if (!LoadAdditionalFile(additionalFile, false, error))
            errors.push_back(std::move(error));

    OverrideWithEnvVariablesIfAny();

    return errors.empty();
}

template<class T>
T ConfigMgr::GetValueDefault(std::string const& name, T def, bool quiet) const
{
    try
    {
        return _config.get<T>(bpt::ptree::path_type(name, '/'));
    }
    catch (bpt::ptree_bad_path const&)
    {
        Optional<std::string> envVar = EnvVarForIniKey(name);
        if (envVar)
        {
            Optional<T> castedVar = rendu::StringTo<T>(*envVar);
            if (!castedVar)
            {
                RD_LOG_ERROR("servers.loading", "Bad value defined for name %s in environment variables, going to use default instead", name.c_str());
                return def;
            }

            if (!quiet)
                RD_LOG_WARN("servers.loading", "Missing name %s in config file %s, recovered with environment '%s' value.", name.c_str(), _filename.c_str(), envVar->c_str());

            return *castedVar;
        }
        else if (!quiet)
        {
            RD_LOG_WARN("servers.loading", "Missing name %s in config file %s, add \"%s = %s\" to this file",
                name.c_str(), _filename.c_str(), name.c_str(), std::to_string(def).c_str());
        }
    }
    catch (bpt::ptree_bad_data const&)
    {
        RD_LOG_ERROR("servers.loading", "Bad value defined for name %s in config file %s, going to use %s instead",
            name.c_str(), _filename.c_str(), std::to_string(def).c_str());
    }

    return def;
}

template<>
std::string ConfigMgr::GetValueDefault<std::string>(std::string const& name, std::string def, bool quiet) const
{
    try
    {
        return _config.get<std::string>(bpt::ptree::path_type(name, '/'));
    }
    catch (bpt::ptree_bad_path const&)
    {
        Optional<std::string> envVar = EnvVarForIniKey(name);
        if (envVar)
        {
            if (!quiet)
                RD_LOG_WARN("servers.loading", "Missing name %s in config file %s, recovered with environment '%s' value.", name.c_str(), _filename.c_str(), envVar->c_str());

            return *envVar;
        }
        else if (!quiet)
        {
            RD_LOG_WARN("servers.loading", "Missing name %s in config file %s, add \"%s = %s\" to this file",
                name.c_str(), _filename.c_str(), name.c_str(), def.c_str());
        }
    }
    catch (bpt::ptree_bad_data const&)
    {
        RD_LOG_ERROR("servers.loading", "Bad value defined for name %s in config file %s, going to use %s instead",
            name.c_str(), _filename.c_str(), def.c_str());
    }

    return def;
}

std::string ConfigMgr::GetStringDefault(std::string const& name, const std::string& def, bool quiet) const
{
    std::string val = GetValueDefault(name, def, quiet);
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    return val;
}

bool ConfigMgr::GetBoolDefault(std::string const& name, bool def, bool quiet) const
{
    std::string val = GetValueDefault(name, std::string(def ? "1" : "0"), quiet);
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    Optional<bool> boolVal = rendu::StringTo<bool>(val);
    if (boolVal)
        return *boolVal;
    else
    {
        RD_LOG_ERROR("servers.loading", "Bad value defined for name %s in config file %s, going to use '%s' instead",
            name.c_str(), _filename.c_str(), def ? "true" : "false");
        return def;
    }
}

int32 ConfigMgr::GetIntDefault(std::string const& name, int32 def, bool quiet) const
{
    return GetValueDefault(name, def, quiet);
}

int64 ConfigMgr::GetInt64Default(std::string const& name, int64 def, bool quiet) const
{
    return GetValueDefault(name, def, quiet);
}

float ConfigMgr::GetFloatDefault(std::string const& name, float def, bool quiet) const
{
    return GetValueDefault(name, def, quiet);
}

std::string const& ConfigMgr::GetFilename()
{
    std::lock_guard<std::mutex> lock(_configLock);
    return _filename;
}

std::vector<std::string> const& ConfigMgr::GetArguments() const
{
    return _args;
}

std::vector<std::string> ConfigMgr::GetKeysByString(std::string const& name)
{
    std::lock_guard<std::mutex> lock(_configLock);

    std::vector<std::string> keys;

    for (bpt::ptree::value_type const& child : _config)
        if (child.first.compare(0, name.length(), name) == 0)
            keys.push_back(child.first);

    return keys;
}
