#include <rapidjson/document.h>
#include <mongoose.h>
#include "server.h"


std::vector<std::string> urls;
std::vector<std::string> post_datas;
rapidjson::Document doc;

#define MAX_CONTENT 4096
#define MAJOR_CONTENT 4096

bool parse_url_json(char *file_path) {
	int fd = open(file_path, O_RDONLY, 0665);
	if (fd < 0) {
		std::cout << "failed to load json file" << endl;
		return false;
	}

	char content[MAX_CONTENT];
	memset(content, 0, sizeof(content));

	read(fd, content, MAX_CONTENT);

	if (doc.Parse(content).HasError()) {
		return false;
	}
	doc.Parse(content);

	urls.clear();
	for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		rapidjson::Document::AllocatorType allocator;
		rapidjson::Value key_value;
		key_value.CopyFrom(itr->name, allocator);
		if (key_value.IsString()) {
			urls.push_back(key_value.GetString());
		}
	}
	close(fd);

	return true;
}

bool parse_major_json(char *file_path) {
	int fd = open(file_path, O_RDONLY, 0665);
	if (fd < 0) {
		std::cout << "failed to load json file" << endl;
		return false;
	}

	char content[MAJOR_CONTENT];
	memset(content, 0, sizeof(content));

	read(fd, content, MAJOR_CONTENT);

	if (doc.Parse(content).HasError()) {
		return false;
	}
	doc.Parse(content);

	for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		rapidjson::Document::AllocatorType allocator;
		rapidjson::Value key_value;
		key_value.CopyFrom(itr->name, allocator);
		if (key_value.IsString()) {

		}
	}
	close(fd);

	return true;
}


void search_from_all_urls(char *uri, int (*fp)(string, string)) {
	for (int i=0;i<urls.size();i++) {
		if (mg_vcmp(uri, urls[i]) == 0) {
			post_datas.clear();
			if (doc.HasMember(uri) {
				rapidjson::Value &data = doc[uri];
				if (data.IsObject()) {
					for (rapidjson::Value::ConstMemberIterator itr = data.MemberBegin();  itr != data.MemberEnd(); ++itr) {

					}
				}
			}
			break;
		}
	}

}


static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
	struct http_message *hm = (struct http_message*) evdata;
	switch (ev) {
		case MG_EV_HTTP_REQUEST:
			search_from_all_urls(&hm->uri);
			break;
		default:
			break;
	}

}
