#include <Sentry.h>

std::string Sentry::url = "";
std::string Sentry::public_key = "";
std::string Sentry::secret_key = "";
std::string Sentry::project_id = "";
int Sentry::timeout = 2;

std::string& replace(std::string &str, char src) {
  int pos = str.find_first_of(src);
  if (pos == std::string::npos)
    return str;
  return replace(str.erase(pos, 1),src);
}

/*
 * 分解url,提取public key, secret key, project id, and url
 */

Sentry::Sentry(const char* file, int line, 
               const char* func, const char* func_header):_file(file),
                                                          _line(line),
                                                          _function(func),
                                                          _function_header(func_header),
                                                          headers(NULL){
}


int Sentry::init(std::string _url,int _timeout) {
  //1. 提取project id
  std::string tmp_str;
  unsigned found = _url.find_last_of("/");
  project_id = _url.substr(found+1);
  tmp_str = _url.substr(0,found);
  //2. 提取url
  unsigned found2 = _url.find_last_of("@");
  url = "http://";
  url = url.append(tmp_str.substr(found2+1));
  //3. 提取public key
  tmp_str= _url.substr(0,found2);
  unsigned found3 = tmp_str.find_first_of("//");
  std::string key = tmp_str.substr(found3+2);
  unsigned found4 = key.find_last_of(":");
  public_key = key.substr(0,found4);
  secret_key = key.substr(found4+1);
  url = url.append("/api/");
  url = url.append(project_id);
  url = url.append("/store/");
  //随机数初始化
  srand(time(NULL));

  timeout = _timeout;
}

Sentry::~Sentry() {
}

int Sentry::captureMessage(std::string title,
                           std::string message,
                           std::string level,
                           void* extra_data) {
  char auth_head[2018] = {0,};
  CURLcode res; //curl返回值
  snprintf(auth_head,1024,"X-Sentry-Auth: Sentry sentry_version=3, sentry_timestamp=%lu, \
                           sentry_key=%s, sentry_client=raven-python/1.0,\
                           sentry_secret=%s",time(NULL),public_key.c_str(),secret_key.c_str());
  headers = curl_slist_append(headers,auth_head);
  headers = curl_slist_append(headers,"User-Agent: raven-python/1.0");
  headers = curl_slist_append(headers,"Content-Type: application/octet-stream");
  time_t rawtime;
  struct tm *timeinfo;
  char timebuf[100] = {0,};
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (timebuf,100,"%Y-%m-%d %H:%M:%S",timeinfo);
  data["project_id"] = project_id;
  data["event_id"] = uuid4();
  data["culprit"] = title;
  data["timestamp"] = timebuf;
  data["message"] = message;
  //信息级别
  data["level"] = level;
  //平台
  data["platform"] = "c/c++";
  Json::Value extra_val;
  if (extra_data != NULL) {
    extra_val = *(static_cast<Json::Value *>(extra_data));
  }
  extra_val["current file"] = _file;
  extra_val["current line"] = _line;
  extra_val["current function"] = _function;
  extra_val["current function declare"] = _function_header;
  data["extra"] = extra_val;
  //获取机器名信息
  char sbuf[50] = {0,};
  int retval = gethostname(sbuf,50);
  if (0 == retval) {
    data["server_name"] = sbuf;
  }
  CURLM *multi_handle = curl_multi_init();
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_POST,1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    Json::FastWriter writer;
    std::string data_str = writer.write(data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_str.c_str());
    curl_easy_setopt(curl, CURLOPT_VERBOSE,false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,false);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT,timeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,timeout);
    curl_multi_add_handle(multi_handle, curl);
    int running_handle_count;
    //非阻塞
    while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count)){
    }
    while (running_handle_count) {
        timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int max_fd;
        fd_set fd_read;
        fd_set fd_write;
        fd_set fd_except;

        FD_ZERO(&fd_read);
        FD_ZERO(&fd_write);
        FD_ZERO(&fd_except);

        curl_multi_fdset(multi_handle, &fd_read, &fd_write, &fd_except, &max_fd);
        int return_code = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &tv);
        if (-1 == return_code) {
          std::cerr << "select error." << std::endl;
            break;
        } else {
            while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &running_handle_count)) {
	    }
        }
    }
    //同步方式
    //res = curl_easy_perform(curl);
    long http_code = 10;
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &http_code);
    std::cout << "curl return : " << http_code << std::endl;
  } else {
    std::cerr << "curl is null" << std::endl;
  }
  curl_multi_cleanup(multi_handle);
  curl_easy_cleanup(curl);
}

/*
 * uuid
 */
std::string Sentry::uuid4() {
  char uuid[1024] = {0,};
  snprintf(uuid,1024,"%04x%04x-%04x-%04x-%04x-%04x%04x%04x",rand() % 0xffff,\
                                                            rand() % 0xffff,\
                                                            rand() % 0xffff,\
                                                            rand() % 0x0fff | 0x4000,\
                                                            rand() % 0x3fff | 0x8000,\
                                                            rand() % 0xffff ,\
                                                            rand() % 0xffff,\
                                                            rand() % 0xffff);
  std::string tmp = uuid;
  replace(tmp,'-');
  return tmp;
}

bool Sentry::error(const char *title, const char *message, void *extra) {
  if (message == NULL) {
    captureMessage(title,title,"error",extra);
  } else {
    captureMessage(title,message,"error",extra);
  }
}

bool Sentry::warn(const char *title, const char *message, void *extra) {
  if (message == NULL) {
    captureMessage(title,title,"warn",extra);
  } else {
    captureMessage(title,message,"warn",extra);
  }
}

bool Sentry::debug(const char* title, const char *message, void *extra) {
  if (message == NULL) {
    captureMessage(title,title,"debug",extra);
  } else {
    captureMessage(title,message,"debug",extra);
  }
}

bool Sentry::info(const char* title, const char *message, void *extra) {
  if (message == NULL) {
    captureMessage(title,title,"info",extra);
  } else {
    captureMessage(title,message,"info",extra);
  }
}

