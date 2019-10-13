#ifndef JAVA_MODULE_H
#define JAVA_MODULE_H

class JavaModule {

public:
	JavaModule();
	~JavaModule();

	int java_load(string url, string data);

};


#endif
