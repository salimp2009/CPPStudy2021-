#pragma once
#include "STLpch.h"

template<typename>
class function_ref;

template<typename Ret, typename... Args>
class function_ref<Ret(Args...)> final
{
private:
    void* _ptr;                                   // will be used to store the address of the object ; which will be type-erased
    Ret(*_erased_fn)(void*, Args...);            // this will be the address of of the erased operator() and store it as a function pointer

public:
    constexpr function_ref() noexcept = delete;

    /* Copy constructor and assignment*/
    constexpr function_ref(const function_ref&) noexcept = default;
    constexpr function_ref& operator = (const function_ref&) noexcept = default;

    template <typename T>
    requires std::is_invocable_r_v<Ret, T&&, Args...> && !std::is_same_v<std::remove_cvref<T>, function_ref>
    constexpr function_ref(T&& x) noexcept : _ptr{ reinterpret_cast<void*>(std::addressof(x)) }
    {
        /* Previous implementation revised according Sy Brands implementation; https://github.com/TartanLlama/function_ref/blob/master/include/tl/function_ref.hpp */
        //_erased_fn = [](void* ptr, Args...xs)->Ret {
        //    return (*reinterpret_cast<std::add_pointer_t<T>>(ptr))(
        //        std::forward<Args>(xs)...);
        //};

        _erased_fn = [](void* ptr, Args...xs)->Ret {
            return std::invoke(*reinterpret_cast<std::add_pointer_t<T>>(ptr), std::forward<Args>(x)...);
        };
    }

    /* Move assignment*/
    template <typename T>
    requires std::is_invocable_r_v<Ret, T&&, Args...> /* && !std::is_same_v<std::remove_cvref<T>, function_ref> */ // TODO ; check if this work basic tests works OK both way
    constexpr function_ref& operator=(T&& x) noexcept 
    {
        _ptr = reinterpret_cast<void*>(std::addressof(x));

        /* Previous implementation revised according Sy Brands implementation; https://github.com/TartanLlama/function_ref/blob/master/include/tl/function_ref.hpp */
        //_erased_fn = [](void* ptr, Args...xs)->Ret {
        //    return (*reinterpret_cast<std::add_pointer_t<T>>(ptr))(
        //        std::forward<Args>(xs)...);
        //};


        _erased_fn = [](void* ptr, Args...xs)->Ret {
            return std::invoke(*reinterpret_cast<std::add_pointer_t<T>>(ptr), std::forward<Args>(x)...);
        };

        return *this;
    }


   decltype(auto) operator()(Args...xs) const noexcept(noexcept(_erased_fn(_ptr, std::forward<Args>(xs)...)))
    {                                                           // this is function_ref operator()
        return _erased_fn(_ptr, std::forward<Args>(xs)...);     // it will invoke type erased function _erased_fn
                                                                // pointer _ptr and all the arguments
                                                                // decltype(auto) is used to preserve the references 
    }

    constexpr ~function_ref() = default;
};