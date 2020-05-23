#ifndef TEMPLATE_UTILS_
#define TEMPLATE_UTILS_
namespace alexei_prog_snob {

template <unsigned long N>
struct binary {
	static unsigned const value = binary<N/10>::value * 2 + N%10;
};

template <>
struct binary<0> {
	static unsigned const value = 0;
};

}  // end namespace alexei_prog_snob

#endif // TEMPLATE_UTILS_