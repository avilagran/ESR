MAINTAINERCLEANFILES    = Makefile.in
  
if HAVE_SNDFILE
JACKREC = jackrec
dist-check-sndfile:
else
JACKREC =
dist-check-sndfile:
	@echo
	@echo ' ******' You need sndfile installed to make dist.' ******'
	@echo
	@false
endif
 
if HAVE_READLINE
JACK_TRANSPORT = jack_transport
dist-check-readline:
else
JACK_TRANSPORT =
dist-check-readline:
	@echo
	@echo ' ******' You need readline installed to make dist.' ******'
	@echo
	@false
	endif
 
bin_PROGRAMS = jack_load \
	jack_unload \
	jack_simple_client \
 	jack_monitor_client \
	jack_impulse_grabber \
	jack_connect \
	jack_disconnect \
	jack_metro \
	jack_showtime \
	jack_bufsize \
	jack_lsp \
	jack_freewheel \
	$(JACKREC) \
	$(JACK_TRANSPORT)
 
if HAVE_SNDFILE
# note! jackrec_CFLAGS syntax not supported by automake-1.4
sndfile_cflags = @SNDFILE_CFLAGS@
endif

AM_CFLAGS = -I.. $(JACK_CFLAGS) $(sndfile_cflags)
AM_CXXFLAGS = -I.. $(JACK_CFLAGS) $(sndfile_cflags)

jack_simple_client_SOURCES = simple_client.c
jack_simple_client_LDFLAGS = -lrt -ldl -lpthread
jack_simple_client_LDADD = ../libjack/libjack.la

jack_connect_SOURCES = connect.c
jack_connect_LDFLAGS = -lrt -ldl -lpthread
jack_connect_LDADD = ../libjack/libjack.la

jack_disconnect_SOURCES = connect.c
jack_disconnect_LDFLAGS = -lrt -ldl -lpthread
jack_disconnect_LDADD = ../libjack/libjack.la

jack_monitor_client_SOURCES = monitor_client.c
jack_monitor_client_LDFLAGS = -lrt -ldl -lpthread
jack_monitor_client_LDADD = ../libjack/libjack.la

jack_metro_SOURCES = metro.c
jack_metro_LDFLAGS = -lrt -ldl -lpthread
jack_metro_LDADD = ../libjack/libjack.la

jack_lsp_SOURCES = lsp.c
jack_lsp_LDFLAGS = -lrt -ldl -lpthread
jack_lsp_LDADD = ../libjack/libjack.la

jack_showtime_SOURCES = showtime.c
jack_showtime_LDFLAGS = -lrt -ldl -lpthread
jack_showtime_LDADD = ../libjack/libjack.la

jack_bufsize_SOURCES = bufsize.c
jack_bufsize_LDFLAGS =
jack_bufsize_LDADD = ../libjack/libjack.la

jack_freewheel_SOURCES = freewheel.c
jack_freewheel_LDFLAGS =
jack_freewheel_LDADD = ../libjack/libjack.la

if HAVE_SNDFILE
jackrec_SOURCES = capture_client.c
jackrec_LDFLAGS = @SNDFILE_LIBS@ -lrt -ldl -lpthread
jackrec_LDADD = ../libjack/libjack.la
endif

if HAVE_READLINE
jack_transport_SOURCES = transport.c
jack_transport_LDFLAGS = -lhistory -lreadline @READLINE_DEPS@ -lrt -ldl -lpthread
jack_transport_LDADD = ../libjack/libjack.la
endif

jack_impulse_grabber_SOURCES = impulse_grabber.c
jack_impulse_grabber_LDFLAGS = -lrt -ldl -lpthread -lm
jack_impulse_grabber_LDADD = ../libjack/libjack.la

#
# general purpose in-process loader
#

jack_load_SOURCES = ipload.c
jack_load_LDFLAGS = -lrt -ldl -lpthread -lm
jack_load_LDADD = ../libjack/libjack.la

jack_unload_SOURCES = ipunload.c
jack_unload_LDFLAGS = -lrt -ldl -lpthread -lm
jack_unload_LDADD = ../libjack/libjack.la

#
# sample in-process client(s)
#

ip_clientdir = $(ADDON_DIR)

ip_client_LTLIBRARIES = inprocess.la intime.la

inprocess_la_LDFLAGS = -module -avoid-version
inprocess_la_SOURCES = inprocess.c

intime_la_LDFLAGS = -module -avoid-version
intime_la_SOURCES = intime.c

dist-hook: dist-check-sndfile





#http://jackaudio.org/files/docs/html/index.html
#SETUP JACK
#http://blog.desdelinux.net/introduccion-a-jack-audio-connection-kit/
#Download JACK2
#Follow README 
#####Install libsamplerate: Donwload from http://www.linuxfromscratch.org/blfs/view/svn/multimedia/libsamplerate.html
#./configure --prefix=/usr --disable-static && make
#sudo make htmldocdir=/usr/share/doc/libsamplerate-0.1.8 install
#####Install alsa
#Synaptics: add alsaplayer-jack and multimedia-jack



#####Compile jack2
#./waf configure --prefix /usr
#./waf
#./waf install



You have to start a jackd server first, usually with the alsa backend.

jackd -d alsa

now jack_simple_client on another terminal should work

jack_simple_client

INFO: http://dis-dot-dat.net/index.cgi?item=jacktuts/starting/invsimpleclient
