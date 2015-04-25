#include "other/function_traits.h"

template <class _FUNC>
void test(const _FUNC &func) {
    typedef function_traits<_FUNC> traits;
    typedef typename traits::template argv<0>::type arg0type;
    static_assert(std::is_same<arg0type, int>::value, "error!");
    typedef typename traits::template argv<1>::type arg1type;
    static_assert(std::is_same<arg1type, double>::value, "error!");
}

int f(int, double) { return 0; }
struct s { void a(int, double){ } static void b(int, double){} };
struct g{ void operator()(int, double){} };
void f2(double, int) {}

int main() {
    test([](int, double) { return 0.0f; });
    std::function<float(int, double)> fff;
    test(fff);
    //test(std::bind(f2, std::placeholders::_2, std::placeholders::_1));
    test(&s::b);
    test(&s::a);
    test(&f);
    test(f);
    test(*f);
    int(*p)(int, double) = f;
    test(p);
    test(*p);
    int(&r)(int, double) = f;
    test(r);
    test(g());

    return 0;
}
