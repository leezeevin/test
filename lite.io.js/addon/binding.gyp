{ "targets": [ { "target_name": "liteio_tcp_x64", "sources" : [ "./nxx/m.c", ], "include_dirs" : [ "./", "./nxx", "./coms", "./coms/x64", ], "link_settings" : { "libraries" : [ "<(module_root_dir)/coms/x64/liblite.n.x64.a", "<(module_root_dir)/coms/x64/liblite.io.x64.a", "<(module_root_dir)/coms/x64/libio.x64.a", ], }, "cflags" : [ "-O2", "-fPIC", "-std=c++11", "-fpermissive", "-D__DEBUG__=1", "-D__DRBG_CHECK__=0", "-D__HTTP__=1", "-D__MIME__=1", "-D__SHM__=1", "-D__NIC__=0", "-D__CGI__=0", "-D__XML__=0", "-D__SERIAL__=1", "-D__SOCKET__=1", "-D__PACKET__=1", "-D__DBC__=0", "-D__SHA1__=1", "-D__LIST__=0", "-D__QUEUE__=0", "-D__STACK__=0", "-D__JSON__=1", "-D__SYSQUEUE__=0", "-D__TRIGONOMETRIC__=1", "-D__SSL_TLS__=0", "-D__CUSTOM_X_SOCKET__=1", "-D__CUSTOM_X_HTTP__=1", "-D__CUSTOM_X_HTTPD__=1", "-D__CUSTOM_X_HTTPSD__=1", "-D__CUSTOM_X_WS__=1", "-D__CUSTOM_X_SERIAL__=1", "-D__CUSTOM_X_SIFR__=0", "-D__STATIC_LIB__=1", "-D__NODE__=1", "-D__LINUX__=1", "-D__WIN32__=0", "-D__NHTTP__=0","-D__NHTTPD__=0","-D__NHTTPSD__=0","-D__NSERIAL__=0","-D__NTCP__=1","-D__NTCPD__=0","-D__NTCPSD__=0","-D__NWSC__=0","-D__NWSD__=0","-D__NSHM__=0","-D__NMDNS__=0", ] } ] }