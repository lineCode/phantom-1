// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/utils/Property.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallVector.h>
#include <phantom/utils/String.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Plugin
{
    friend class Application;
    friend class Module;
    friend class Package;
    friend class Source;

public:
    static bool           HasLoadingInProgress();
    static bool           HasLoadingInProgress(Module* a_pModule);
    static bool           HasUnloadingInProgress();
    static Plugins const& GetLoadingPluginStack();
    static Plugins const& GetUnloadingPluginStack();

public:
    Plugin(StringView a_strName, StringView a_strFilePath);
    Plugin(StringView a_strFilePath);

    StringView getFilePath() const
    {
        return m_strFilePath;
    }

    bool load();
    bool unload();
    bool loadAsDependency();
    bool unloadAsDependency();
    bool isLoaded() const
    {
        return m_pModule != nullptr;
    }
    void deleteOnDisk();

    Module* getModule() const
    {
        return m_pModule;
    }

    StringView getName() const
    {
        return m_strName;
    }
    void setName(StringView a_Name)
    {
        m_strName = a_Name;
    }

    StringView getDebugPath() const
    {
        return m_DebugPath;
    }
    void setDebugPath(StringView a_In)
    {
        m_DebugPath = a_In;
    }

    StringView getReleasePath() const
    {
        return m_ReleasePath;
    }
    void setReleasePath(StringView a_In)
    {
        m_ReleasePath = a_In;
    }

private:
    bool _isLoadingModule(Module* a_pModule) const;
    bool _refCountNative();
    bool _unrefCountNative();
    bool _load();
    bool _unload();
    bool _loadNative(StringView strPath);
    bool _unloadNative();
    bool _loadFile();

private:
    String  m_strFilePath;
    String  m_strName;
    String  m_DebugPath;
    String  m_ReleasePath;
    Module* m_pModule = nullptr;
    int     m_RefCount = 0;

private:
    friend class phantom::detail::DynamicCppInitializerH;
};
} // namespace lang

} // namespace phantom
