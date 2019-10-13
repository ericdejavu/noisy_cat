#ifndef PARSER_H
#define PARSER_H

#define DEFAULT_PATH_SIZE 1024

#define DEFAULT_DEV_PATH "./router/dev.json"
#define DEFAULT_MAJOR_PATH "./router/dev.json"

#define DEFAULT_HASH_PATH "./router/auto/hash.json"
#define DEFAULT_CHAIN_PATH "./router/auto/chain.json"
#define DEFAULT_STABLE_PATH "./router/auto/stable.json"

class Respnose {
public:
	char *status;
	char *result;

	Respnose();
	~Respnose();
};

class ReturnEntity {
public:
	Respnose *respnose;
	std::vector<char*> post_keys;
};

class JsonFile {
public:
	char *filepath;
	int fd;

	JsonFile();
	~JsonFile();
};

class BaseParser {
private:

	JsonFile *dev_file, *hash_file, *chain_file, *major_file, *stable_file;

	bool parse_url_json();
	bool parse_major_json();

	void update_chain(std::vector<std::string> post_keys);
	void search_from_all_urls(char *uri);

public:
	BaseParser();
	~BaseParser();
	void setFilePath(char *dev_path, char *hash_path, char *chain_path, char *major_path, char *stable_path);
	virtual ReturnEntity *dy_service(std::vector<std::string> post_datas);
	void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
};

class PythonParser : public BaseParser {
private:
	Python *pPython;
public:
	ReturnEntity *dy_service(std::vector<std::string> post_datas);
};

class JavaParser : public BaseParser {
public:
	ReturnEntity *dy_service(std::vector<std::string> post_datas);
};


#endif
