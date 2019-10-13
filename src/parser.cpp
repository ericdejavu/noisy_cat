#include <rapidjson/document.h>
#include "include/mongoose.h"
#include "server.h"


std::vector<std::string> urls;
std::vector<std::string> post_datas;
rapidjson::Document doc, hash_doc;

#define MAX_CONTENT 4096
#define MAJOR_CONTENT 4096

#define STAND_LENGTH 4096

#define COMMON_RESPONSE "{ \"ok\": %s, \"data\": %s }"

#define SEND(respnose) 		mg_printf_http_chunk(np->nc, COMMON_RESPONSE, respnose->status, respnose->result); \
													mg_send_http_chunk(np->nc, "", 0);

bool BaseParser::parse_url_json(char *hash_path, char *file_path) {
	scan_dev_json(hash_path, file_path);
}

void BaseParser::scan_dev_json(char *hash_path, char *file_path) {
	int dev_fd = open(file_path, O_RDONLY, 0665);
	if (dev_fd == -1) {
		std::cout << "failed to load dev json file" << std::endl;
		return false;
	}

	int hash_fd = open(file_path, O_CREAT | O_RDWR, 0665);
	if (hash_fd == -1) {
		std::cout << "failed to load hash json file" << std::endl;
		return false;
	}

	char dev_content[MAX_CONTENT], hash_content[MAX_CONTENT];
	memset(dev_content, 0, sizeof(dev_content));
	memset(hash_content, 0, sizeof(hash_content));

	read(dev_fd, dev_content, MAX_CONTENT);
	read(hash_fd, hash_content, MAX_CONTENT);

	if (doc.Parse(dev_content).HasError()) {
		return false;
	}

	if (hash_doc.Parse(hash_content).HasError()) {
		return false;
	}

	urls.clear();

	for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		rapidjson::Document::AllocatorType allocator;
		rapidjson::Value key_value;
		key_value.CopyFrom(itr->name, allocator);
		if (key_value.IsString()) {
			urls.push_back(key_value.GetString());
		}




	}
	close(dev_fd);
	close(hash_fd);

	return true;
}

bool BaseParser::parse_major_json(char *file_path) {
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


Respnose *BaseParser::dy_service(std::vector<std::string> post_datas) {

}

void BaseParser::search_from_all_urls(char *uri) {
	for (int i=0;i<urls.size();i++) {
		if (mg_vcmp(uri, urls[i]) == 0) {
			post_datas.clear();
			if (doc.HasMember(uri) {
				rapidjson::Value &data = doc[uri];
				if (data.IsObject()) {
					for (rapidjson::Value::ConstMemberIterator itr = data.MemberBegin(); itr != data.MemberEnd(); ++itr) {
						Value &key = itr.GetName();
						if(key.IsString()) {
							char param[STAND_LENGTH];
							memset(param, 0, sizeof(param));
							mg_get_http_var(&np->hm->body, key.GetString(), param, sizeof(param));

							post_datas.push_back(param);

						}
					}
				}

				Respnose *respnose = dy_service(post_datas);
				SEND(respnose);
			}
			break;
		}
	}

}


static void BaseParser::ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
	struct http_message *hm = (struct http_message*) evdata;
	switch (ev) {
		case MG_EV_HTTP_REQUEST:
			search_from_all_urls(&hm->uri);
			break;
		default:
			break;
	}

}
