####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
CC=gcc
CXX=g++
CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CPPFLAGS=-D_GNU_SOURCE \
  -D__STDC_LIMIT_MACROS \
  -DVERSION=\"1.0.0.1\"
INCPATH=-I. \
  -I./include \
  -I./output \
  -I./output/include \
  -I./src
DEP_INCPATH=-I../third-64/json-cpp \
  -I../third-64/json-cpp/include \
  -I../third-64/json-cpp/output \
  -I../third-64/json-cpp/output/include \
  -I../third-64/libcurl \
  -I../third-64/libcurl/include \
  -I../third-64/libcurl/output \
  -I../third-64/libcurl/output/include

#============ CCP vars ============
CCHECK=@ccheck.py
CCHECK_FLAGS=
PCLINT=@pclint
PCLINT_FLAGS=
CCP=@ccp.py
CCP_FLAGS=


#COMAKE UUID
COMAKE_MD5=d8add8f9a6313dff6aa753d86f42c0e0  COMAKE


.PHONY:all
all:comake2_makefile_check sentry libsentry.a libsentry.so 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	#in case of error, update 'Makefile' by 'comake2'
	@echo "$(COMAKE_MD5)">comake2.md5
	@md5sum -c --status comake2.md5
	@rm -f comake2.md5

.PHONY:ccpclean
ccpclean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mccpclean[0m']"
	@echo "make ccpclean done"

.PHONY:clean
clean:ccpclean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf sentry
	rm -rf ./output/bin/sentry
	rm -rf libsentry.a
	rm -rf ./output/lib/libsentry.a
	rm -rf libsentry.so
	rm -rf ./output/so/libsentry.so
	rm -rf src/sentry_Sentry.o

.PHONY:dist
dist:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	tar czvf output.tar.gz output
	@echo "make dist done"

.PHONY:distclean
distclean:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdistclean[0m']"
	rm -f output.tar.gz
	@echo "make distclean done"

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

sentry:src/sentry_Sentry.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msentry[0m']"
	$(CXX) src/sentry_Sentry.o -Xlinker "-("  ../third-64/json-cpp/lib/linux-gcc-3.4.5/libjson_linux-gcc-3.4.5_libmt.a \
  ../third-64/libcurl/lib/libcurl.a -lpthread \
  -lcrypto \
  -lrt \
  -lssl \
  -lcurl \
  -lldap -Xlinker "-)" -o sentry
	mkdir -p ./output/bin
	cp -f --link sentry ./output/bin

libsentry.a:src/sentry_Sentry.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlibsentry.a[0m']"
	ar crs libsentry.a src/sentry_Sentry.o
	mkdir -p ./output/lib
	cp -f --link libsentry.a ./output/lib

libsentry.so:src/sentry_Sentry.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlibsentry.so[0m']"
	$(CXX) -shared src/sentry_Sentry.o -Xlinker "-("  -lpthread \
  -lcrypto \
  -lrt \
  -lssl \
  -lcurl \
  -lldap -Xlinker "-)" -o libsentry.so
	mkdir -p ./output/so
	cp -f --link libsentry.so ./output/so

src/sentry_Sentry.o:src/Sentry.cpp \
  src/Sentry.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40msrc/sentry_Sentry.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CPPFLAGS) $(CXXFLAGS)  -o src/sentry_Sentry.o src/Sentry.cpp

endif #ifeq ($(shell uname -m),x86_64)


