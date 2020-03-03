// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/MemberAnonymousSection.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a member anonymous struct.
/// \code{.cpp}
///     struct
///     {
///         int a;
///         int b;
///     };
/// \endcode
class PHANTOM_EXPORT_PHANTOM MemberAnonymousStruct : public MemberAnonymousSection
{
    PHANTOM_DECLARE_META_CLASS(MemberAnonymousStruct);

public:
    MemberAnonymousStruct(Modifiers modifiers = 0, uint a_uiFlags = 0);

    MemberAnonymousStruct* asMemberAnonymousStruct() const override
    {
        return (MemberAnonymousStruct*)this;
    }

protected:
    void computeSizeAndAlignment(size_t& a_uiSize, size_t& a_uiAlignment) const override;
};

} // namespace lang
} // namespace phantom
