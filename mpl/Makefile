MakePage=mydef_page

TOPROOT=out/tcp.c out/test_hash.c out/runtests.pl out/mpl_sockaddr.c out/mpl_hash.c out/t.c out/test_if.pl 

all_targets: ${TOPROOT}

out/tcp.c: test_sockaddr.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc test_sockaddr.def out/tcp.c

out/test_hash.c: test_hash.def macros/hash.def /home/hzhou/lib/MyDef/c/hash.def /home/hzhou/lib/MyDef/c/strpool.def  
	${MakePage} -mc test_hash.def out/test_hash.c

out/runtests.pl: runtests.def   
	${MakePage} runtests.def out/runtests.pl

out/mpl_sockaddr.c: mpl_sockaddr.def ../mpich_frame.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc mpl_sockaddr.def out/mpl_sockaddr.c

out/mpl_hash.c: mpl_hash.def ../mpich_frame.def macros/hash.def /home/hzhou/lib/MyDef/c/hash.def /home/hzhou/lib/MyDef/c/strpool.def  
	${MakePage} -mc mpl_hash.def out/mpl_hash.c

out/t.c: test_sockaddr.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} -mc test_sockaddr.def out/t.c

out/test_if.pl: test_sockaddr.def macros/sockaddr.def macros/sockaddr_listen.def macros/sockaddr_if.def macros/sockaddr_debug.def  
	${MakePage} test_sockaddr.def out/test_if.pl

