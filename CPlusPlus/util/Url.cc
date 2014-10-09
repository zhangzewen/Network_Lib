#include "Url.h"
#include "exception.h"

using namespace NetworkLib::Util;

namespace NetworkLib {
namespace Util {

Url::Url(http_parser* tmp) {
	parser_ = tmp;
}

Url::~Url() {
	// free parser_;
}

void Url::Parse(const std::string& urlStr) {
	http_parser_parse_url(urlStr.c_str(), urlStr.size(), 1, url_);
}

}
}
