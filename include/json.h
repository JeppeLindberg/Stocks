#ifndef STOCKS_JSON_H
#define STOCKS_JSON_H

#include <iostream>
#include <iomanip>
#include "types.h"

char peek(std::istream& is);
char read(std::istream& is);
void expect(std::istream& is, const char symbol);
void read_until(std::istream& is, const char symbol);

template <typename T>
struct json_t
{
    T&& data;
    int& amount;
    int& offset;
};

template <typename T>
json_t(T&&, int, int) -> json_t<T>;

template <typename T>
json_t(T&&) -> json_t<T>;

struct json_writer_t
{
    std::istream& is;
    int& amount;
    int& offset;
    bool first = true;

    template <typename Data>
    void operator()(const std::string& name, Data& value)
    {
        using namespace std::literals::string_literals;
        if (!first) {
            expect(is, ',');
        } else
            first = false;
        std::string name_read;
        auto c = peek(is);
        if (c != '"')
            throw std::logic_error("Expected '\"' but got '"s + c + "'");
        is >> std::quoted(name_read);
        if (name_read != name)
            throw std::logic_error("Expected field \""s + name + "\" but got " + name_read);
        expect(is, ':');
        is >> json_t{value, amount, offset};
    }
    /** handle tag-dispatch for tuples: */
    template <typename Tuple, std::size_t... Is>
    void fill_tuple(Tuple& tuple, std::index_sequence<Is...>)
    {
        (..., operator()(std::to_string(Is + 1), std::get<Is>(tuple)));
    }
    /** support for tuples: */
    template <typename... Ts>
    void operator()(std::tuple<Ts...>& data)
    {
        fill_tuple(data, std::index_sequence_for<Ts...>{});
    }
};

struct json_reader_t
{
    std::ostream& os;
    bool first = true;  // first field is being visited
    template <typename Data>
    void operator()(const std::string& name, const Data& value)
    {
        if (!first)
            os << ',';
        else
            first = false;
        os << std::quoted(name) << ':' << json_t{value};
    }
    /** handle tag-dispatch for tuples: */
    template <typename Tuple, std::size_t... Is>
    void print_tuple(const Tuple& tuple, std::index_sequence<Is...>)
    {
        // parameter pack fold over the comma operator:
        (..., operator()(std::to_string(Is + 1), std::get<Is>(tuple)));
    }
    /** support for tuples: */
    template <typename... Ts>
    void operator()(const std::tuple<Ts...>& data)
    {
        print_tuple(data, std::index_sequence_for<Ts...>{});
    }
};

// & = Bind to lvalue reference (resources cannot be reused)
// && = Bind to rvalue reference (resources can be reused)
template <typename T>
std::istream& operator>>(std::istream& is, json_t<T>&& json)
{
    using namespace std::literals::string_literals;
    auto flags = is.flags();

    if constexpr(is_bool_v<T>)
        // Sets the boolalpha flag of the stream
        is >> std::boolalpha >> json.data;
    else if constexpr(is_number_v<T>)
        is >> json.data;
    else if constexpr(is_string_v<T>)
        is >> quoted(json.data);
    else if constexpr(is_container_v<T>) {
        // Find the types of the elements that T contains
        using U = typename std::remove_reference_t<T>::value_type;
        expect(is, '[');
        auto first = true;
        while (is){
            if (json.amount == 0){
                read_until(is, ']');
                break;
            }

            auto c = peek(is);
            if (c == ']'){
                is.get();
                break;
            }
            if (!first){
                if (c != ',')
                    throw std::logic_error("Expected ',' but got "s + c);
                is.get();
            } else
                first = false;
            U value{};
            is >> json_t{value, json.amount, json.offset};

            if (json.offset <= 0) {
                json.data.push_back(std::move(value));
                if (json.amount > 0)
                    json.amount--;
            }
            else
                json.offset--;
        }
    }
    else if constexpr (is_tuple_v<T>) {
        expect(is, '{');
        json_writer_t{is, json.amount, json.offset}(json.data);
        expect(is, '}');
    } else if constexpr (accepts_writer_v<T, json_reader_t>) {
        expect(is, '{');
        json.data.accept_writer(json_writer_t{is, json.amount, json.offset});
        expect(is, '}');
    }

    is.setf(flags, flags);
    return is;
}

void expect(std::istream& is, const char symbol){
    using namespace std::literals::string_literals;
    const auto c = read(is);
    if (symbol != c)
        throw std::logic_error("Expected "s + symbol + " but got '" + c + "'");
}

char peek(std::istream& is){
    while (is && std::isspace(is.peek()))
        is.get();
    return static_cast<char>(is.peek());
}

char read(std::istream& is){
    char c;
    while (is.get(c) && std::isspace(c))
        ;
    return c;
}

void read_until(std::istream& is, const char symbol){
    char c;
    while (is.get(c) && c != symbol)
        ;
}

#endif //STOCKS_JSON_H
