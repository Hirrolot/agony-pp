/**
 * @file
 * Support for C language constructions.
 *
 * # Statement chaining
 *
 * This module exports several so-called _statement chaining macros_: they expect a statement right
 * after their invocation, and moreover, an invocation of such a macro with a statement afterwards
 * altogether form a single statement.
 *
 * How can this be helpful? Imagine you are writing a macro with the following syntax:
 *
 * @code
 * YOUR_MACRO(...) { bla bla bla }
 * @endcode
 *
 * Then `YOUR_MACRO` must expand to a _statement prefix_, i.e. something that expects a statement
 * after itself. One possible solution is to make `YOUR_MACRO` expand to a sequence of statement
 * chaining macros like this:
 *
 * @code
 * #define YOUR_MACRO(...) \
 *     ML99_INTRODUCE_VAR_TO_STMT(int x = 5) \
 *         ML99_CHAIN_EXPR_STMT(printf("%d\n", x)) \
 *             and so on...
 * @endcode
 *
 * Here `ML99_CHAIN_EXPR_STMT` accepts the statement formed by `ML99_CHAIN_EXPR_STMT`, which in turn
 * accepts the next statement and so on, until a caller of `YOUR_MACRO` specifies the final
 * statement, thus completing the chain.
 *
 * @see https://www.chiark.greenend.org.uk/~sgtatham/mp/ for analysis of statement prefixes as well
 * as the whole idea.
 */

#ifndef ML99_GEN_H
#define ML99_GEN_H

#include <metalang99/choice.h>
#include <metalang99/control.h>
#include <metalang99/lang.h>
#include <metalang99/list.h>
#include <metalang99/nat.h>
#include <metalang99/tuple.h>
#include <metalang99/util.h>
#include <metalang99/variadics.h>

/**
 * Pastes @p prefix, @p id, and the current line number together.
 *
 * It is used to imitate macro hygiene.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * #define MY_MACRO \
 *     int ML99_GEN_SYM(MY_MACRO_, x) = 5; \
 *     printf("%d\n", ML99_GEN_SYM(MY_MACRO_, x));
 * @endcode
 *
 * Within `MY_MACRO`, `ML99_GEN_SYM(MY_MACRO_, x)` will designate the same variable because any C
 * macro always results in a single source code line.
 *
 * @see https://en.wikipedia.org/wiki/Hygienic_macro
 */
#define ML99_GEN_SYM(prefix, id) ML99_CAT(prefix, ML99_CAT(id, ML99_CAT(_, __LINE__)))

/**
 * Puts provided arguments into braces.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // { int a, b, c; }
 * ML99_braced(v(int a, b, c;))
 * @endcode
 */
#define ML99_braced(...) ML99_call(ML99_braced, __VA_ARGS__)

/**
 * Generates a type definition.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // typedef struct { int x, y; } Point;
 * ML99_typedef(v(Point), v(struct { int x, y; }))
 * @endcode
 */
#define ML99_typedef(ident, ...) ML99_call(ML99_typedef, ident, __VA_ARGS__)

/**
 * Generates a C structure.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // struct Point { int x, y; }
 * ML99_struct(v(Point), v(int x, y;))
 * @endcode
 */
#define ML99_struct(ident, ...) ML99_call(ML99_struct, ident, __VA_ARGS__)

/**
 * Generates an anonymous C structure.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // struct { int x, y; }
 * ML99_struct(v(int x, y;))
 * @endcode
 */
#define ML99_anonStruct(...) ML99_call(ML99_anonStruct, __VA_ARGS__)

/**
 * The same as #ML99_struct but generates a union.
 */
#define ML99_union(ident, ...) ML99_call(ML99_union, ident, __VA_ARGS__)

/**
 * The same as #ML99_anonStruct but generates a union.
 */
#define ML99_anonUnion(...) ML99_call(ML99_anonUnion, __VA_ARGS__)

/**
 * The same as #ML99_struct but generates an enumeration.
 */
#define ML99_enum(ident, ...) ML99_call(ML99_enum, ident, __VA_ARGS__)

/**
 * The same as #ML99_anonStruct but generates an enumeration.
 */
#define ML99_anonEnum(...) ML99_call(ML99_anonEnum, __VA_ARGS__)

/**
 * Generates \f$(T_0 \ \_0, ..., T_n \ \_n)\f$.
 *
 * If @p type_list is empty, this macro results in `(void)`.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // (int _0, long long _1, const char * _2)
 * ML99_indexedParams(ML99_list(v(int, long long, const char *)))
 *
 * // (void)
 * ML99_indexedParams(ML99_nil())
 * @endcode
 */
#define ML99_indexedParams(type_list) ML99_call(ML99_indexedParams, type_list)

/**
 * Generates \f$T_0 \ \_0; ...; T_n \ \_n\f$.
 *
 * If @p type_list is empty, this macro results in emptiness.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // int _0; long long _1; const char * _2;
 * ML99_indexedFields(ML99_list(v(int, long long, const char *)))
 *
 * // ML99_empty()
 * ML99_indexedFields(ML99_nil())
 * @endcode
 */
#define ML99_indexedFields(type_list) ML99_call(ML99_indexedFields, type_list)

/**
 * Generates \f$\{ \_0, ..., \_{n - 1} \}\f$.
 *
 * If @p n is 0, this macro results in `{ 0 }`.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // { _0, _1, _2 }
 * ML99_indexedInitializerList(v(3))
 *
 * // { 0 }
 * ML99_indexedInitializerList(v(0))
 * @endcode
 */
#define ML99_indexedInitializerList(n) ML99_call(ML99_indexedInitializerList, n)

/**
 * Generates \f$\_0, ..., \_{n - 1}\f$.
 *
 * If @p n is 0, this macro results in emptiness.
 *
 * # Examples
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * // _0, _1, _2
 * ML99_indexedArgs(v(3))
 *
 * // ML99_empty()
 * ML99_indexedArgs(v(0))
 * @endcode
 */
#define ML99_indexedArgs(n) ML99_call(ML99_indexedArgs, n)

/**
 * A statement chaining macro which introduces several variable definitions to a statement right
 * after its invocation.
 *
 * Variable definitions must be specified as in the first clause of the for-loop.
 *
 * # Example
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * for (int i = 0; i < 10; i++)
 *     ML99_INTRODUCE_VAR_TO_STMT(double x = 5.0, y = 7.0)
 *         if (i % 2 == 0)
 *             printf("i = %d, x = %f, y = %f\n", i, x, y);
 * @endcode
 */
#define ML99_INTRODUCE_VAR_TO_STMT(...)                                                            \
    ML99_PRIV_SHADOWS(for (__VA_ARGS__, *ml99_priv_break = (void *)0;                              \
                           ml99_priv_break != (void *)1;                                           \
                           ml99_priv_break = (void *)1))

/**
 * The same as #ML99_INTRODUCE_VAR_TO_STMT but deals with a single non-`NULL` pointer.
 *
 * In comparison with #ML99_INTRODUCE_VAR_TO_STMT, this macro generates a little less code. It
 * introduces a pointer to @p ty identified by @p name and initialised to @p init.
 *
 * # Example
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * double x = 5.0, y = 7.0;
 *
 * for (int i = 0; i < 10; i++)
 *     ML99_INTRODUCE_NON_NULL_PTR_TO_STMT(double, x_ptr, &x)
 *         ML99_INTRODUCE_NON_NULL_PTR_TO_STMT(double, y_ptr, &y)
 *             printf("i = %d, x = %f, y = %f\n", i, *x_ptr, *y_ptr);
 * @endcode
 *
 * @note Unlike #ML99_INTRODUCE_VAR_TO_STMT, the generated pointer is guaranteed to be used at least
 * once, meaning that you do not need to suppress the unused variable warning.
 * @note @p init is guaranteed to be executed only once.
 */
#define ML99_INTRODUCE_NON_NULL_PTR_TO_STMT(ty, name, init)                                        \
    ML99_PRIV_SHADOWS(for (ty *name = (init); name != (void *)0; name = (void *)0))

/**
 * A statement chaining macro which executes an expression statement derived from @p expr right
 * before the next statement.
 *
 * # Example
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * int x;
 *
 * for(;;)
 *     ML99_CHAIN_EXPR_STMT(x = 5)
 *         ML99_CHAIN_EXPR_STMT(printf("%d\n", x))
 *             puts("abc");
 * @endcode
 */
#define ML99_CHAIN_EXPR_STMT(expr)                                                                 \
    ML99_PRIV_SHADOWS(for (int ml99_priv_expr_stmt_break = ((expr), 0);                            \
                           ml99_priv_expr_stmt_break != 1;                                         \
                           ml99_priv_expr_stmt_break = 1))

/**
 * A statement chaining macro which suppresses the "unused X" warning right before a statement after
 * its invocation.
 *
 * # Example
 *
 * @code
 * #include <metalang99/gen.h>
 *
 * int x, y;
 *
 * for(;;)
 *     ML99_SUPPRESS_UNUSED_BEFORE_STMT(x)
 *         ML99_SUPPRESS_UNUSED_BEFORE_STMT(y)
 *             puts("abc");
 * @endcode
 *
 * @deprecated Use `ML99_CHAIN_EXPR_STMT((void)expr)` instead.
 */
#define ML99_SUPPRESS_UNUSED_BEFORE_STMT(expr) ML99_CHAIN_EXPR_STMT((void)expr)

#ifndef DOXYGEN_IGNORE

#define ML99_PRIV_SHADOWS(...)                                                                     \
    ML99_CLANG_PRAGMA("clang diagnostic push")                                                     \
    ML99_CLANG_PRAGMA("clang diagnostic ignored \"-Wshadow\"")                                     \
    __VA_ARGS__                                                                                    \
    ML99_CLANG_PRAGMA("clang diagnostic pop")

#define ML99_braced_IMPL(...)         v({__VA_ARGS__})
#define ML99_typedef_IMPL(ident, ...) v(typedef __VA_ARGS__ ident;)
#define ML99_struct_IMPL(ident, ...)  v(struct ident{__VA_ARGS__})
#define ML99_anonStruct_IMPL(...)     v(struct {__VA_ARGS__})
#define ML99_union_IMPL(ident, ...)   v(union ident{__VA_ARGS__})
#define ML99_anonUnion_IMPL(...)      v(union {__VA_ARGS__})
#define ML99_enum_IMPL(ident, ...)    v(enum ident{__VA_ARGS__})
#define ML99_anonEnum_IMPL(...)       v(enum {__VA_ARGS__})

// ML99_indexedParams_IMPL {
#define ML99_indexedParams_IMPL(type_list)                                                         \
    ML99_tuple(ML99_PRIV_IF(                                                                       \
        ML99_IS_NIL(type_list),                                                                    \
        v(void),                                                                                   \
        ML99_variadicsTail(ML99_PRIV_indexedParamsAux_IMPL(type_list, 0))))

#define ML99_PRIV_indexedParamsAux_IMPL(type_list, i)                                              \
    ML99_matchWithArgs_IMPL(type_list, ML99_PRIV_indexedParamsAux_, i)
#define ML99_PRIV_indexedParamsAux_nil_IMPL(...) v(ML99_EMPTY())
#define ML99_PRIV_indexedParamsAux_cons_IMPL(x, xs, i)                                             \
    ML99_TERMS(v(, x _##i), ML99_PRIV_indexedParamsAux_IMPL(xs, ML99_INC(i)))
// }

// ML99_indexedFields_IMPL {
#define ML99_indexedFields_IMPL(type_list) ML99_PRIV_indexedFieldsAux_IMPL(type_list, 0)

#define ML99_PRIV_indexedFieldsAux_IMPL(type_list, i)                                              \
    ML99_matchWithArgs_IMPL(type_list, ML99_PRIV_indexedFields_, i)
#define ML99_PRIV_indexedFields_nil_IMPL(...) v(ML99_EMPTY())
#define ML99_PRIV_indexedFields_cons_IMPL(x, xs, i)                                                \
    ML99_TERMS(v(x _##i;), ML99_PRIV_indexedFieldsAux_IMPL(xs, ML99_INC(i)))
// }

#define ML99_indexedInitializerList_IMPL(n) ML99_braced(ML99_PRIV_INDEXED_ITEMS(n, v(0)))
#define ML99_indexedArgs_IMPL(n)            ML99_PRIV_INDEXED_ITEMS(n, v(ML99_EMPTY()))

#define ML99_PRIV_INDEXED_ITEMS(n, empty_case)                                                     \
    ML99_PRIV_IF(                                                                                  \
        ML99_NAT_EQ(n, 0),                                                                         \
        empty_case,                                                                                \
        ML99_variadicsTail(ML99_repeat_IMPL(n, ML99_PRIV_indexedItem)))

#define ML99_PRIV_indexedItem_IMPL(i) v(, _##i)

// Arity specifiers {
#define ML99_braced_ARITY                 1
#define ML99_typedef_ARITY                2
#define ML99_struct_ARITY                 2
#define ML99_anonStruct_ARITY             1
#define ML99_union_ARITY                  2
#define ML99_anonUnion_ARITY              1
#define ML99_enum_ARITY                   2
#define ML99_anonEnum_ARITY               1
#define ML99_indexedParams_ARITY          1
#define ML99_indexedFields_ARITY          1
#define ML99_indexedInitializerList_ARITY 1
#define ML99_indexedArgs_ARITY            1

#define ML99_PRIV_indexedItem_ARITY 1
// }

#endif // DOXYGEN_IGNORE

#endif // ML99_GEN_H
