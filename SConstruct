

_CCFLAGS = '-c -Wall -Wno-return-type -D_REENTRANT -DHAVE_NETINET_IN_H -I/opt/hypertable/current/include \
       -I/opt/hypertable/current/include/thrift \
		-I/opt/hypertable/current/include/ThriftBroker/gen-cpp \
		-I/usr/local/include'

_LINKFLAGS = '-g -v -rdynamic -L/opt/hypertable/current/lib -lHyperThrift \
        -lHyperCommon -lHypertable -lthrift -levent -llog4cpp -lHyperSpace \
		-lboost_system-mt -lpthread -lcurl -lbson -lpcrecpp \
		-ljson_linux-gcc-4.2.1_libmt -lswcs -lswallow -lprotobuf -lcld2 \
		-lboost_locale-mt -licudata -licuio -licui18n -licule -liculx \
		-licutu -licuuc'

_CXX = ' clang++'# -Wc++11-extensions'


cpp_files = []

cpp_files = cpp_files + Glob('../thirdparty/libstemmer_c/libstemmer/*utf8.c')
cpp_files = cpp_files + Glob('../thirdparty/libstemmer_c/src_c/*UTF*.c')
cpp_files = cpp_files + Glob('../thirdparty/libstemmer_c/runtime/*.c')
cpp_files = cpp_files + Glob('*.cpp')
cpp_files = cpp_files + Glob('../htdba/*.cpp')
cpp_files = cpp_files + Glob('../tetramorph/threadpool/*.cpp')
cpp_files = cpp_files + Glob('../tetramorph/common/string_utils.cpp')

env = Environment(CCFLAGS='-g')
Program('hdict_test', 
[ cpp_files
], LIBPATH='.', CCFLAGS=_CCFLAGS, LINKFLAGS=_LINKFLAGS, CXX = _CXX)
