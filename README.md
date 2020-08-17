# webserver
C - Final Project
****************************
Instructions 
  
->A request (which you will receive) will consist of the word “GET”, a space, a path to the file, a space and then the version of HTTP which is implemented (you can ignore this) all on the first line. There will be other lines to the request – you can ignore them.

->A response (which you will generate) will consist of: the string “HTTP/1.0 “, followed by code (200 for OK or 404 for file not found), a new line, then “Content-Length: “ + the number of bytes of the response followed by 2 new lines.

*Your program should run on port 8000 (port 80, which is the usual for web applications, will be blocked by your firewall)
*Your program should take a path as a parameter (and fail with a usage message if there is no parameter). That path will be the path that your web server will use to serve files. For example – let’s say that the path is /home/mphipps. If I try to access localhost:8000/secret/data.html, your web server will look at: /home/mphipps/secret and try to serve data.html. 
*If the file is there, it should return it with a “200 OK” message, as above.
* If the file does not exist, you should return a “404 Not Found” message with a pre-designed response (could be a file or could be a hard coded message).
*Your web server should launch a new thread for each request. You should use POSIX commands for files (no fopen/fclose/etc) and pthreads for threadedin. Your web server should be an infinite loop of waiting for and processing requests. 
