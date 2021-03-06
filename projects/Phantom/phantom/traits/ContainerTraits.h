#pragma once
#include "CopyTraits.h"
#include "ImplicitConv.h"

namespace phantom
{
template<class Container,
         class = decltype(
         std::declval<Container>().erase(std::declval<PHANTOM_TYPENAME Container::iterator>()))>
std::true_type HasContainerEraseH(Container&&);

template<class Container>
std::false_type HasContainerEraseH(ImplicitConv<Container&&>);

template<class Container>
struct HasContainerErase : decltype(HasContainerEraseH<Container>(std::declval<Container>()))
{
};

template<class Container,
         class = decltype(std::declval<Container>().push_back(
         std::declval<PHANTOM_TYPENAME Container::value_type>()))>
std::true_type HasContainerPushBackH(Container&&);

template<class Container>
std::false_type HasContainerPushBackH(ImplicitConv<Container>);

template<class Container>
struct HasContainerPushBack : decltype(HasContainerPushBackH<Container>(std::declval<Container>()))
{
};
} // namespace phantom
