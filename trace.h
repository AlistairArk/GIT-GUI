
#undef untested

#ifdef DO_TRACE
#define trace0(s) ( std::cerr << "@#@" << (s) << "\n")
#define trace1(s,x) ( \
	   std::cerr <<  "@#@" << (s) << "  " << #x << "=" << (x)  \
		     << "\n" )
#define trace2(s,x,y) ( \
	   std::cerr <<  "@#@" << (s) << "  " << #x << "=" << (x)  \
		     << "  " << #y << "=" << (y)  \
		     << "\n" )

#else
#define USE(x) (1)?(void)(0):(void)(x)
#define trace0(s) USE(s)
#define trace1(s,x) (USE(s),USE(x))
#define trace2(s,x,y) USE(s);USE(x);USE(y)
#endif

#ifdef TRACE_UNTESTED

#ifdef __cplusplus

#define untested() ( std::cerr <<  "@@#\n@@@:"<< __FILE__ << ":"<< __LINE__ \
          <<":" << __func__ << "\n" )
#define untested1(s,x) ( std::cerr <<  "@@#\n@@@:"<< __FILE__ << ":"<< __LINE__ \
          <<":" << __func__ << ": "  << s << "  " << #x << "=" << (x) << "\n" )
#define untested2(s,x,y) ( std::cerr <<  "@@#\n@@@:"<< __FILE__ << ":"<< __LINE__ \
            <<":" << __func__ << ": "  << s << "  " << #x << "=" << (x) << ", "\
                                                    << #y << "=" << (y) << "\n" )
#define untested3(s,x,y,z) ( std::cerr <<  "@@#\n@@@:"<< __FILE__ << ":"<< __LINE__ \
              <<":" << __func__ << ": "  << s << "  " << #x << "=" << (x) << ", "\
                                                      << #y << "=" << (y) << ", " \
                                                      << #z << "=" << (z) << "\n" )

#else // no __cplusplus

#define untested() (fprintf(stderr,"@@#\n@@@:%s:%u:%s\n", \
			   __FILE__, __LINE__, __func__))

#endif // __cplusplus

#else
#define untested()
#define untested1(s,x)
#define untested2(s,x,y)
#define untested3(s,x,y,z)
#endif

#define incomplete() ( \
    std::cerr << "@@#\n@@@\nincomplete:" \
              << __FILE__ << ":" << __LINE__ << ":" << __func__ << "\n" )
#define unreachable() ( \
    std::cerr << "@@#\n@@@\nunreachable:" \
              << __FILE__ << ":" << __LINE__ << ":" << __func__ << "\n" )

