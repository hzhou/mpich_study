MakePage=mydef_page

TOPROOT=out/test_if.pl out/tcp.c out/mpl_sockaddr.c out/t.c 

all_targets: ${TOPROOT}

out/test_if.pl: tcp.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} tcp.def out/test_if.pl

out/tcp.c: tcp.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc tcp.def out/tcp.c

out/mpl_sockaddr.c: mpl_sockaddr.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc mpl_sockaddr.def out/mpl_sockaddr.c

out/t.c: t.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc t.def out/t.c

