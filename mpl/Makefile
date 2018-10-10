MakePage=mydef_page

TOPROOT=out/mpl_sockaddr.c 

all_targets: ${TOPROOT}

all: all_targets out

out/mpl_sockaddr.c: mpl_sockaddr.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc mpl_sockaddr.def out/mpl_sockaddr.c

