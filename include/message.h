#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//404 code, not found the page
const char *const err_404 = "HTTP/1.1 404 Not Found\n";
const char *const page_404 = "../www/_404.html";

//403 code, request forbidden
const char *const err_403 = "HTTP/1.1 403 Request Forbidden\n"
const char *const page_403 = "../www/_403.html";

//405 code, unknown method
const char *const err_405 = "HTTP/1.1 405 Unkown Method\n"
const char *const page_405 = "../www/_405.html";

//505 code \, Unsuport http protocol
const char *const err_505 = "HTTP/1.1 505 Unsuport Http Protocol\n"
const char *const page_505 = "../www/_505.html"

//fatal error
const char *const err_fatal = "<html><p>Fatel Error!</p><h>No re    ponse! please check your request.</h></html>"


#endif
