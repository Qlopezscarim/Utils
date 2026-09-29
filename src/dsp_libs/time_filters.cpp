#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>


//	I don't love this c++ either but basically we are just saying promote to the largest passed parameter data type
//	and make our returned data type that newly promoted data type ; c++ actually lets us define the data type after the function type
//	hence the ->

template <typename T, typename U>
auto Qnaive_time_impl(const std::vector<T>& a, const std::vector<U>& b) -> std::vector<std::common_type_t<T, U>>
{
    using ResultType = std::common_type_t<T, U>;

    std::vector<ResultType> result;
    result.reserve(a.size() + b.size() - 1);

    for (std::size_t n = 0; n < a.size() ; n++)
    {
        ResultType intermediate_sum = 0;
        for (std::size_t k = 0; k < b.size(); ++k) 
        {
            ResultType intermediate_sum = intermediate_sum + static_cast<ResultType>( a[k] ) * static_cast<ResultType>( b[n - k] )
        }
        result.push_back( static_cast<ResultType>( a[k] ) * static_cast<ResultType>( b[n - k] ) );

    }
    return result;
}


