#include "conditionfactory.h"

namespace {

template<typename First>
std::tuple<First> listToTuple(VariantList& list)
{
    static_assert(!std::is_reference<First>::value, "function must not be given a reference type");
    auto first = list.front();
    list.erase(list.begin());
    auto arg = std::any_cast<typename std::decay<First>::type>(first);
    return std::tuple<First>(arg);
}

template<typename ... Args, typename = std::enable_if_t<sizeof...(Args) == 0>>
std::tuple<> listToTuple(VariantList&)
{
    return std::tuple<>();
}

template<typename First, typename Second, typename ... Args>
std::tuple<typename std::decay<First>::type, typename std::decay<Second>::type, typename std::decay<Args>::type...>
listToTuple(VariantList& list)
{
    auto first = listToTuple<typename std::decay<First>::type>(list);
    auto second = listToTuple<typename std::decay<Second>::type, typename std::decay<Args>::type...>(list);
    return std::tuple_cat(first, second);
}

template<typename T, typename... Args, size_t... IndexSeq>
Condition::Ptr __construct_condition_impl(std::tuple<Args...> args, std::index_sequence<IndexSeq...>)
{
    return T::create(std::get<IndexSeq>(args)...);
}

template<typename T, typename... Args>
Condition::Ptr __construct_condition(std::shared_ptr<T> (*)(Args...), const VariantList& argValues)
{
    auto constexpr N = sizeof...(Args);
    using IndexSeq = std::make_index_sequence<N>;

    auto argsCopy = argValues;
    auto argsTuple = listToTuple<Args...>(argsCopy);
    return __construct_condition_impl<T>(argsTuple, IndexSeq());
}

template<typename T>
Condition::Ptr construct_condition(const VariantList& argValues)
{
    return __construct_condition(&T::create, argValues);
}

template<int N, typename T, typename... Entries>
struct RepeatN {
    using Type = typename RepeatN<N-1, T, T, Entries...>::Type;
};

template<typename T, typename... Entries>
struct RepeatN<0, T, Entries...> {
    using Type = std::tuple<Entries...>;
};

template<typename T, typename... Args>
Condition::Ptr __construct_condition_nargs_tuple(const VariantList& argValues, std::tuple<Args...>) {
    return __construct_condition<T, Args...>(&T::create, argValues);
}

template<typename T, typename ArgType, int N = 20>
Condition::Ptr construct_condition_nargs(const VariantList& argValues)
{
    if constexpr (N < 0) {
        throw OverflowError{"Maximum number of arguments (20) exceeded"};
    }
    else {
        if (argValues.size() == N) {
            return __construct_condition_nargs_tuple<T>(argValues, typename RepeatN<N, ArgType>::Type());
        }
        return construct_condition_nargs<T, ArgType, N-1>(argValues);
    }
}

}

Condition::Ptr createCondition(std::string condition, const VariantList& args)
{
    if (condition == "Negate")
        return construct_condition<Negate>(args);
    if (condition == "HasMoney")
        return construct_condition<HasMoney>(args);
    if (condition == "AllOf")
        return construct_condition_nargs<AllOf, Condition::Ptr>(args);

    return Condition::None();
}

