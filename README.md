sentry-cpp
==========

sentry-cpp is a c/c++ client for [Sentry](https://getsentry.com)

```
#include <Sentry.h>

int main(int argc, char** argv) 
{
    //Instantiate a new client with a compatible DSN
    SENTRY.init("http://public:secret@example.com/1");
    //only title
    SENTRY.error("title");
    //title and message
    SENTRY.info("title","message");
    //send extra data
    Json::Value extra;
    SENTRY.info("title","message",&extra);
}
```

Dependence
------------

before you use sentry-cpp, you need install the following libraries.

1. [Jsoncpp](http://jsoncpp.sourceforge.net/)

2. [libcurl](http://curl.haxx.se/libcurl/)

Install
------------

next, you need edit the Makefile, and change the path for json-cpp and libcurl, now you can

```
make
```

it will generate static library(.a) and share library(.so), you can use sentry-cpp in three ways:

1. use source code, only Sentry.cpp and Sentry.h

2. use shared object 

3. use static object

Enjoy!
