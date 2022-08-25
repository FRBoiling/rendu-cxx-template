/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef __GITREVISION_H__
#define __GITREVISION_H__

#include "define.h"

namespace GitRevision
{
    RD_COMMON_API char const* GetHash();
    RD_COMMON_API char const* GetDate();
    RD_COMMON_API char const* GetBranch();
    RD_COMMON_API char const* GetCMakeCommand();
    RD_COMMON_API char const* GetCMakeVersion();
    RD_COMMON_API char const* GetHostOSVersion();
    RD_COMMON_API char const* GetBuildDirectory();
    RD_COMMON_API char const* GetSourceDirectory();
    RD_COMMON_API char const* GetMySQLExecutable();
    RD_COMMON_API char const* GetFullDatabase();
    RD_COMMON_API char const* GetHotfixesDatabase();
    RD_COMMON_API char const* GetFullVersion();
    RD_COMMON_API char const* GetCompanyNameStr();
    RD_COMMON_API char const* GetLegalCopyrightStr();
    RD_COMMON_API char const* GetFileVersionStr();
    RD_COMMON_API char const* GetProductVersionStr();
}

#endif
