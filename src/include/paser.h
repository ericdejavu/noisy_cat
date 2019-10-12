#ifndef PARSER_H
#define PARSER_H

class Respnose {
public:
	char *status;
	char *result;

	Respnose();
	~Respnose();
};

class BaseParser {
private:
	bool parse_url_json(char *file_path);
	bool parse_major_json(char *file_path);

	void search_from_all_urls(char *uri);

public:
	virtual Respnose *dy_service(std::vector<std::string> post_datas);
	void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
};

class PythonParser : public BaseParser {
public:
	Respnose *dy_service(std::vector<std::string> post_datas);
};

class JavaParser : public BaseParser {
public:
	Respnose *dy_service(std::vector<std::string> post_datas);
};


#endif
