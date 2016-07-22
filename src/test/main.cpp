#include <all.h>

//#define TESTINATOR_MAIN
#include <testinator.h>

#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct curl_slist {};

curl_slist* curl_slist_append(curl_slist* old, const char*)
{
  return old;
}

using Headers = vector<pair<string, string>>;
Headers headers;

int main(void)
{

  curl_slist* curl_headers = accumulate(
      headers.cbegin(), headers.cend(), static_cast<curl_slist*>(nullptr),
      [] (curl_slist* h, const auto& v) {
        return curl_slist_append(h, v.first.c_str());
      });

  cout << curl_headers;

  // curl_slist* curl_headers = NULL;
  // for (auto it = headers.begin();
  //      it != headers.end(); ++it)
  // {
  //   curl_headers = curl_slist_append(curl_headers,
  //                                   (boost::format("%s: %s") % it->first % it->second).str().c_str());
  // }
}
