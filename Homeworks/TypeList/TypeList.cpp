#include <iostream>
#include <type_traits>

namespace TypeSpace
{
    template <typename... Types> struct TypeList {};
    template<typename> struct Size;
    template<size_t ind, typename> struct TypeAt;
    template<typename, typename> struct inList;
    template<typename, typename, size_t index = 0> struct TypeIndex;
    template<typename, typename> struct pushFront;
    template<typename, typename> struct pushBack;

    template <size_t ind, typename Head, typename... Others>
    struct TypeAt<ind, TypeList<Head, Others...>>
    {
        using type = typename TypeAt<ind - 1, TypeList<Others...>>::type;
    };

    template <typename Head, typename... Others>
    struct TypeAt<0, TypeList<Head, Others...>>
    {
        using type = Head;
    };

    template <typename... Types>
    struct Size<TypeList<Types...>>
    {
        static constexpr size_t size = sizeof...(Types);
    };

    template <typename Type, typename Head, typename... Others>
    struct inList<Type, TypeList<Head, Others...>>
    {
        static constexpr bool is_inList = std::is_same_v<Type, Head> || inList<Type, TypeList<Others...>>::is_inList;
    };

    template <typename Type>
    struct inList<Type, TypeList<>>
    {
        static constexpr bool is_inList = false;
    };

    template <typename Type, size_t prev>
    struct TypeIndex<Type, TypeList<>, prev>
    {
        static_assert(sizeof(Type) == 0, "Type not found in TypeList");
    };

    template <typename Type, typename... Others, size_t prev>
    struct TypeIndex<Type, TypeList<Type, Others...>, prev>
    {
        static constexpr size_t index = prev;
    };

    template <typename Type, typename Head, typename... Others, size_t prev>
    struct TypeIndex<Type, TypeList<Head, Others...>, prev>
    {
        static constexpr size_t index = TypeIndex<Type, TypeList<Others...>, prev + 1>::index;
    };

    template <typename Type, typename... Types>
    struct pushFront<Type, TypeList<Types...>>
    {
        using list = TypeList<Type, Types...>;
    };

    template <typename... Types, typename Type>
    struct pushBack<TypeList<Types...>, Type>
    {
        using list = TypeList<Types..., Type>;
    };
};

int main()
{
    using namespace TypeSpace;

    using List = TypeList<int, char, double>;

    static_assert(Size<List>::size == 3, "Size failed");

    static_assert(std::is_same_v<TypeAt<0, List>::type, int>, "TypeAt 0 failed");
    static_assert(std::is_same_v<TypeAt<1, List>::type, char>, "TypeAt 1 failed");
    static_assert(std::is_same_v<TypeAt<2, List>::type, double>, "TypeAt 2 failed");
    //static_assert(std::is_same_v<TypeAt<3, List>::type, void>); //- ошибка

    static_assert(inList<int, List>::is_inList == true, "Contains int");
    static_assert(inList<char, List>::is_inList == true, "Contains char");
    static_assert(inList<double, List>::is_inList == true, "Contains double");
    static_assert(inList<float, List>::is_inList == false, "Not contains float");

    static_assert(TypeIndex<int, List>::index == 0, "Index of int");
    static_assert(TypeIndex<char, List>::index == 1, "Index of char");
    static_assert(TypeIndex<double, List>::index == 2, "Index of double");
    //TypeIndex<float, List>::index; //- ошибка 

    using ListWithFront = pushFront<float, List>::list;
    static_assert(std::is_same_v<ListWithFront, TypeList<float, int, char, double>>, "PushFront");

    using ListWithBack = pushBack<List, float>::list;
    static_assert(std::is_same_v<ListWithBack, TypeList<int, char, double, float>>, "PushBack");

    using Empty = TypeList<>;
    static_assert(Size<Empty>::size == 0, "Empty size");
    static_assert(inList<int, Empty>::is_inList == false, "Empty contains");
    //TypeAt<0, Empty> //- ошибка

}