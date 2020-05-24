Tired of wrapping legacy C API functions? Want to reduce an amount of boilerplate code you have to write to make use of libraries written with different API design decisions?
return_adapters library to the rescue!

# Quick features overview

- [Transform functions returning values using out parameters](#Transform-functions-returning-values-using-out-parameters)
- [Transform functions returning error codes to functions throwing exceptions](#Transform-functions-returning-error-codes-to-functions-throwing-exceptions)
- [Transform functions to functions returning smart pointers](#Transform-functions-to-functions-returning-smart-pointers)
- [Transform functions throwing exceptions to functions returning bool or std optional](#Transform-functions-throwing-exceptions-to-functions-returning-bool-or-std-optional)
- [Transform functions to functions with mapped return value](#Transform-functions-to-functions-with-mapped-return-value)


## Transform functions returning values using out parameters

```c++
  // legacy function. stores 'a' / 'b' to 'result' out parameter. returns false if 'b' is 0, returns true otherwise
  bool divide(int a, int b, int& result); 
  
  // adapted function. takes just first two arguments and returns std::optional<int> with result. No more out parameters.
  constexpr auto adapted_divide = turn_outarg_to_optional_retval<&divide, check_retval_is_true>;
  
  assert(adapted_divide(6, 2) == std::optional<int>{3});
  assert(!adapted_divide(6, 0).has_value());  
  static_assert(std::is_same_v<decltype(adapted_divide), std::optional<int>(*const)(int, int)>);
```

Customization points: 
- specify an index of out parameter. By default a last parameter is considered to be an out parameter. Any index can be set.
- specify a return value check predicate determining whether the returned optional should have value
- specify a custom adapter in case you want to return not an std::optional but some other type.

## Transform functions returning error codes to functions throwing exceptions
```c++
  // legacy function. returns 0 if a file is closed, returns non-zero otherwise
  int fclose ( FILE * stream );
  
  // adapted function. Will throw an std::runtime_error with "::fclose failed; returned value: [RETURNED_VALUE]" message if a non-zero value will be returned. where [RETURNED_VALUE] will be the actual returned value.
  constexpr auto adapted_fclose  = RETURN_ADAPTERS_ADAPT_TO_THROWING( ::fclose, throwing::generic_adapter_handler<check_retval_is_zero> );
  
  static_assert(std::is_same_v<decltype(adapted_fclose), int(*const)(FILE*)>);
```
Customization points:  
- specify exception type
- specify exception message formatter
- specify a return value check predicate determining whether an exception should be thrown

## Transform functions to functions returning smart pointers

```c++
  // legacy functions
  resource* alloc_resource( size_t n );
  void free_resource( resource* );
  
  // adapted function. Returns std::unique_ptr<resource, Deleter>
  constexpr auto adapted_alloc_resource = adapt_to_unique_ptr<&alloc_resource, &free_resource>;
  auto resource = adapted_alloc_resource( 42 ); // resource will be freed at the scope exit

```
Customization points:  
- specify default deleter, deleter function or custom deleter type for unique_ptr if it's needed
- specify smart pointer type ( unique_ptr, shared_ptr )
- specify a custom adapter to support any smart pointer type

## Transform functions throwing exceptions to functions returning bool or std optional
```c++
  // function tries to parse integer and throws exception if string parse failed
  int parse_int( const std::string& str );
  
  // adapted function. Catches std::exception. If no exception was caught returns std::optional<int> with a value otherwise returns empty optional
  // if an adaptee function has a void return type then an adapted function will return bool instead of an std::optional
  constexpr auto adapted_parse_int = make_non_throwing<&parse_int>;
  
  static_assert(std::is_same_v<decltype(adapted_parse_int), std::optional<int>(*const)(const std::string&)>);
  ```
  Customization points:  
  - specify exception types to catch
  - specify a custom adapter in case you want to return not an std::optional/bool but some other type.

## Transform functions to functions with mapped return value

```c++
  // some api function
  some_library_error_code send_message( const char* msg );
  
  // adapted function. returns different return type.
  constexpr auto adapted_send_message = map_retval<&send_message, error_code_adapter>;
  
  static_assert(std::is_same_v<decltype(adapted_send_message), my_app_error_code(*const)( const char* )>);
```

Customization points:
- specify a custom adapter to map returned type
