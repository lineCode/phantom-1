// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

namespace phantom
{
namespace detail
{
template<typename T, typename V = void>
struct IsEnumClassH : std::false_type
{
};

template<typename T>
struct IsEnumClassH<T, decltype((void)+T{})> : std::true_type
{
};
} // namespace detail

template<typename T>
using IsEnumClass =
std::integral_constant<bool, !detail::IsEnumClassH<T>::value && std::is_enum<T>::value>;
} // namespace phantom
