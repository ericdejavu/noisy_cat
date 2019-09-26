#include <iostream>
#include <mongoose.h>




int run_server(const char *address, MG_CB(mg_event_handler_t handler, void *user_data)) {
	struct mg_mgr mgr;
	struct mg_connection *nc = NULL;

	struct mg_bind_opts bind_opts;
	char *err_str = NULL;


	mg_mgr_init(&mgr, NULL);
	
	memset(&bind_opts, 0, sizeof(bind_opts));
	bind_opts.error_string = &err_str;

	nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
	if (nc == NULL) {
		exit(1);
	}

	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.enable_directory_listing = "yes";
	cout << "start RESTful server on port " << s_http_port << "serving " << s_http_server_opts.document_root << endl;

	while(true) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
	return 0;

}
