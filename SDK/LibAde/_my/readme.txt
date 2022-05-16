sources\ade\include\ade\util\type_traits.hpp

template<class T, class... ArgTypes>
using result_of_t = typename std::invoke_result<T, ArgTypes...>::type;

sources\ade\include\ade\util\func_ref.hpp
 using actual_result_type = util::result_of_t<Callable,Args...>;