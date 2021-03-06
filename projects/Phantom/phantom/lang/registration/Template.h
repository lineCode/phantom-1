#pragma once

#include <haunt>

HAUNT_STOP;

#include <phantom/detail/StaticGlobalRegistrer.h>
#include <phantom/detail/core.h>

namespace phantom
{
namespace lang
{
struct PHANTOM_EXPORT_PHANTOM TemplateRegistrer : public ::phantom::detail::_PHNTM_StaticGlobalRegistrer
{
    TemplateRegistrer(StringView (*func)(int), const char* a_strFile, int line, int tag);
    void _PHNTM_process(phantom::RegistrationStep PHANTOM_REGISTRATION_STEP) override;

private:
    StringView (*m_func)(int);
};
} // namespace lang
} // namespace phantom
