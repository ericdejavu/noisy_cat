#ifndef PARSER_H
#define PARSER_H

class BaseParser {
private:
	bool parse_url_json(char *file_path);
	bool parse_major_json(char *file_path);

	void search_from_all_urls(char *uri);

public:
	virtual void dy_service();
	void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
};

class PythonParser : public BaseParser {
public:
	void dy_service();
};

class JavaParser : public BaseParser {
public:
	void dy_service();
};


#endif
