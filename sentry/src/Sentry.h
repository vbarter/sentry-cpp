#ifndef  __SENTRY_H_
#define  __SENTRY_H_


#include <iostream>
#include <string>
#include <curl/curl.h>
#include <time.h>
#include <stdlib.h>
#include <json/json.h>


class Sentry
{
private:
  static std::string url;
  
  static std::string public_key;
  
  static std::string secret_key;
  
  static std::string project_id;
  
  static int timeout; //curl超时时间
  
  CURL *curl;
  
  Json::Value data; //发送数据的json
  
  struct curl_slist *headers;
  
  std::string uuid4(); //产生一个32位的uuid
  
  int captureMessage(std::string,
                     std::string,
                     std::string,
                     void* extra_data=NULL); //发送消息

  //文件名
  const char *_file;
  //文件行数
  int _line;
  //当前函数名
  const char *_function;
  //完整的函数头信息
  const char *_function_header;

public:
  Sentry(const char *file, int line, const char *func, const char *func_header);
  
  ~Sentry();

  bool error(const char *title, const char *message=NULL, void *extra=NULL);

  bool warn (const char *title, const char *message=NULL, void *extra=NULL);

  bool info (const char *title, const char *message=NULL, void *extra=NULL);
  
  bool debug(const char *title, const char *message=NULL, void *extra=NULL);

  static int init(std::string,int _timeout=2);
};

/// Helper macro to get file name and line number.
#define SENTRY (Sentry(__FILE__, __LINE__,__func__,__PRETTY_FUNCTION__))

#endif //__SENTRY_H_
