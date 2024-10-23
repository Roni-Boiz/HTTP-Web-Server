# HTTP-Web-Server 
Simple web server build in C progamming language from scratch.

![Home](https://github.com/user-attachments/assets/ea159e47-cdae-4c35-8f00-62515aac1084)

#### Compile: gcc main.c Client.c Response.c Routes.c Server.c Parameters.c -o Debug/HTTPWebServer

#### Run (Windows): cd Debug && HTTPWebServer.exe --> Open browser: http://localhost:8080/
#### Run (Linux): cd Debug && ./HTTPWebServer -v -p 8080 -b 20 -r true --> Open browser: http://localhost:8080/

### Routes
1. 	/ --> index.html
2. /home --> index.html
3. /about -->about.html
4. /service --> service.html
5. /blog --> blog.html
6. /contact --> contact.html
7. /File/txt --> sample-text.txt
8. /File/pdf --> sample-pdf.pdf
9. /File/audio --> sample-audio.mp3
10. /File/video --> sample-video.mp4

### Command Line Arguments
1. -v --> print current version
2. -p --> configure running port
3. -b --> configure maximum nunber of request wait in queue
4. -r --> show/hide routes
