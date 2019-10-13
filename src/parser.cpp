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
#define DEFINE_NEW_FILE(file, path) 	file = new JsonFile(); \
										file->filepath = new char[DEFAULT_PATH_SIZE]; \
										memset(file->filepath, 0, sizeof(char)*DEFAULT_PATH_SIZE); \
										memcpy(file->filepath, path, strlen(path));

#define SEND(respnose) 		mg_printf_http_chunk(np->nc, COMMON_RESPONSE, respnose->status, respnose->result); \
							mg_send_http_chunk(np->nc, "", 0);



JsonFile::JsonFile() : filepath(NULL), fd(0) {}

JsonFile::~JsonFile() {
	if (filepath != NULL) {
		delete filepath;
	}
	if (fd != 0 && fd != -1) {
		close(fd);
	}
}

BaseParser::BaseParser() {
	DEFINE_NEW_FILE(dev_file, DEFAULT_DEV_PATH);
	DEFINE_NEW_FILE(major_file, DEFAULT_MAJOR_PATH);

	DEFINE_NEW_FILE(hash_file, DEFAULT_HASH_PATH);
	DEFINE_NEW_FILE(chain_file, DEFAULT_CHAIN_PATH);
	DEFINE_NEW_FILE(stable_file, DEFAULT_STABLE_PATH);
}

BaseParser::~BaseParser() {
	delete dev_file;
	delete major_file;

	delete hash_file;
	delete chain_file;
	delete stable_file;
}

bool BaseParser::parse_url_json() {
	scan_dev_json(true);
}


/**
write: i_arg1/u_arg2_arg1
dev normal key: arg1
hash_normal key: arg1_<index>
chain normal key: <index>:arg1/<index>:arg2->arg1

1. from dev file's url get post_datas
2. analyze post_datas and write to hash file
3. change dev file's post_datas to normal
4. update chain file
*/
void BaseParser::scan_dev_json(bool closed_when_done) {
	if (dev_file == NULL || hash_file == NULL) {
		std::cout << "dev file and hash_file is null" << std::endl;
		return;
	}
	dev_file->fd = open(dev_file->file_path, O_RDONLY, 0665);
	if (dev_file->fd == -1) {
		std::cout << "failed to load dev json file" << std::endl;
		return false;
	}

	hash_file->fd = open(hash_file->file_path, O_CREAT | O_RDWR, 0665);
	if (hash_file->fd == -1) {
		std::cout << "failed to load hash json file" << std::endl;
		return false;
	}

	char dev_content[MAX_CONTENT], hash_content[MAX_CONTENT];
	memset(dev_content, 0, sizeof(dev_content));
	memset(hash_content, 0, sizeof(hash_content));

	read(dev_file->fd, dev_content, MAX_CONTENT);
	read(hash_file->fd, hash_content, MAX_CONTENT);

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
	if (closed_when_done) {
		close(dev_file->fd);
		dev_file->fd = 0;
		close(hash_file->fd);
		hash_file->fd = 0;
	}

	return true;
}

void BaseParser::update_chain(std::vector<std::string> post_keys) {

}

bool BaseParser::parse_major_json() {
	if (major_file == NULL) {
		return false;
	}
	major_file->fd = open(major_file->file_path, O_RDONLY, 0665);
	if (major_file->fd == -1) {
		std::cout << "failed to load json file" << endl;
		return false;
	}

	char content[MAJOR_CONTENT];
	memset(content, 0, sizeof(content));

	read(major_file->fd, content, MAJOR_CONTENT);

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
	close(major_file->fd);
	major_file->fd = 0;

	return true;
}


ReturnEntity *BaseParser::dy_service(std::vector<std::string> post_datas) {

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

				ReturnEntity *returnEntity = dy_service(post_datas);
				if (returnEntity != NULL) {
					update_chain(returnEntity->post_keys);
					SEND(returnEntity->response);
					delete returnEntity;
				}
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
