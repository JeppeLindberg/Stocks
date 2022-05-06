#ifndef STOCKS_TYPES_H
#define STOCKS_TYPES_H

#include <type_traits>

//is_same_v: Returns value true if bool and T is the same, otherwise false
//remove_cv_t: Returns type of T where any const or volatile is removed
//remove_reference_t: Returns the dereferenced type of T if it is reference, otherwise T
template <typename T>
constexpr auto is_bool_v = std::is_same_v<bool, std::remove_cv_t<std::remove_reference_t<T>>>;

template <typename T>
constexpr auto is_number_v = std::is_arithmetic_v<std::remove_cv_t<std::remove_reference_t<T>>>;

template <typename T>
constexpr auto is_character_v = std::is_same_v<char, std::remove_cv_t<std::remove_reference_t<T>>>;

template <typename C>
using get_iterator = decltype(std::begin(std::declval<C&>()));

template <typename C, typename = void>
struct is_container : std::false_type
{};

template <typename C>  // specialization for the iterable types
struct is_container<C, std::void_t<get_iterator<C>>> : std::true_type
{};

template <typename C>
constexpr auto is_container_v = is_container<C>::value;

template <typename S, typename = void>
struct is_string : std::false_type
{};

template <typename S>  // container of characters
struct is_string<S, std::void_t<get_iterator<S>>>
: std::conditional_t<is_character_v<typename std::iterator_traits<get_iterator<S>>::value_type>, std::true_type,
std::false_type>
{};

template <>  // c-string
struct is_string<char*, void> : std::true_type
{};

template <>  // const c-string
struct is_string<const char*, void> : std::true_type
{};

template <typename S>
constexpr auto is_string_v = is_string<std::remove_cv_t<std::remove_reference_t<S>>>::value;

template <typename... Ts>
struct is_tuple : std::false_type
{};

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type
{};

template <typename T>
constexpr auto is_tuple_v = is_tuple<std::decay_t<T>>::value;

template <typename Data, typename Reader, typename = void>
struct accepts_writer : std::false_type
{};

template <typename Data, typename Writer>
struct accepts_writer<Data, Writer, std::void_t<decltype(std::declval<Data&>().accept_writer(std::declval<Writer&>()))>>
        : std::true_type
{};

template <typename Data, typename Writer>
constexpr auto accepts_writer_v = accepts_writer<Data, Writer>::value;




#endif //STOCKS_TYPES_H
