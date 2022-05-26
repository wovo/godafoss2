// =============================================================================
//
// gf-application.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2019-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// 
// not useful, cto can have a run too!
// concept for cto - has resources
// concept for application -> has also run
//
// =============================================================================


// ===========================================================================
//
// fixed-maximum-length structural string
//
// structural means usable as template parameter
//
// ===========================================================================

template< size_t N >
struct string_literal {
    
    char value[ N ];
    
    constexpr string_literal( const char * s ) {
        for( auto & v : value ){
           v = ( *s == '\0' ) ? *s : *s++;
        }
        value[ N - 1 ] = '\0';
    }
    
};


// ===========================================================================
//
// immutable list 
//
// for compile-time use
//
// ===========================================================================

template< typename T, int N >
struct immutable_list: std::array< T, N > {

   template< int A, int B > 
   constexpr immutable_list( 
      const immutable_list< T, A > & a, 
      const immutable_list< T, B > & b  
   ){
      static_assert( N == A + B );
      for( unsigned int i = 0; i < A; ++i ){
          this->operator[]( i ) = a[ i ];
      }
      for( unsigned int i = 0; i < B; ++i ){
          this->operator[]( A + i ) = b[ i ];
      }
   }
   
   constexpr immutable_list( 
      const T a[ N ]  
   ){
      for( unsigned int i = 0; i < N; ++i ){
          this->operator[]( i ) = a[ i ];
      }
   }   

};

template< typename T >
struct immutable_list< T, 1 >: std::array< T, 1 > {
    
    constexpr immutable_list( const T & x ){
        this->operator[]( 0 ) = x;
    }
    
};

template< typename T >
struct immutable_list< T, 0 >: std::array< T, 0 > { };

template< typename T, int A, int B >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, B > & b  
){
   return immutable_list< T, A + B >( a, b );
}   

template< typename T, int A  >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const immutable_list< T, 0 > & b  
){
   return a;
}

template< typename T, int B  >
constexpr auto operator+( 
   const immutable_list< T, 0 > & a, 
   const immutable_list< T, B > & b  
){
   return b;
}

template< typename T >
constexpr auto operator+( 
   const immutable_list< T, 0 > & a, 
   const immutable_list< T, 0 > & b  
){
   return immutable_list< T, 0 >();
}

template< typename T, int A  >
constexpr auto operator+( 
   const immutable_list< T, A > & a, 
   const T & x
){
   return a + immutable_list< T, 1 >( x );
}

template< typename T, int A  >
constexpr auto operator+( 
   const T & x,
   const immutable_list< T, A > & b
){
   return immutable_list< T, 1 >( x ) + b;
}

template< int N >
struct immutable_string : immutable_list< char, N > {
   constexpr immutable_string( 
      const char ( &s )[ N ]
   ): immutable_list< char, N >( s ){}
};

template<>
struct immutable_string< 0 > : immutable_list< char, 0 > {
   constexpr immutable_string( 
   ): immutable_list< char, 0 >(){}
};

template< int N >
constexpr auto prefix(){
   return immutable_list< char, 1 >( '@' ) + immutable_list< char, 1 >( '0' + N );
}



// =============================================================================

template< typename T >
concept resource = requires {
    true;
//   typename T::resources;
};

// fallback, required but never used
template< resource... _tail >
struct use_list {};

// recursion endpoint
template<>
struct use_list<> {
   using first = void;

   static void GODAFOSS_INLINE initialize(){}
};

// initialize() will call that same function in all arguments
template< resource _first, resource... _tail >
struct use_list< _first, _tail... > {
   using first      = _first;
   using resources  = use_list< _tail... >;

   static void initialize(){
      first::resources::initialize();
      resources::initialize();
   }
};


// =============================================================================

// background< f > can be used as an element of a use<> list,
// or instead of a use<> list
template< void f() >
struct execute {
   //using first      = execute< f >;
   //using resources  = use<>;

   // called when used as
   //    using resources = use< ..., excecute< f > >
   static void initialize(){
      f();
   };

   // called when used as
   //    using resources = excecute< f >
   struct resources {
      static void initialize(){
         f();
      };
   };
};


// =============================================================================

// background f > can be used as an element of a use<> list,
// or instead of a use<> list
template< void f() >
struct _background {
};


// =============================================================================

// concept for cto - has resources
// concept for application -> has also run

template< typename application >
class run {
public:   
   // determine the 
   
   // run the inits
   //application< timing >::
   
   
   
   // run the main    
   run(){
      application::resources::initialize();
      application::run();
   }
};

