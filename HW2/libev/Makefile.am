AUTOMAKE_OPTIONS = foreign

VERSION_INFO = 4:0:0

EXTRA_DIST = LICENSE Changes libev.m4 autogen.sh \
	     ev_vars.h ev_wrap.h \
	     ev_epoll.c ev_select.c ev_poll.c ev_kqueue.c ev_port.c ev_linuxaio.c ev_iouring.c \
             ev_win32.c \
	     ev.3 ev.pod Symbols.ev Symbols.event

man_MANS = ev.3

include_HEADERS = ev.h ev++.h event.h

lib_LTLIBRARIES = libev.la

libev_la_SOURCES = ev.c event.c
libev_la_LDFLAGS = -version-info $(VERSION_INFO)

ev.3: ev.pod
	pod2man -n LIBEV -r "libev-$(VERSION)" -c "libev - high performance full featured event loop" -s3 <$< >$@
