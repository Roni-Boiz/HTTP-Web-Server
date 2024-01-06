# HTTP-Web-Server 

### Run: Debug/HTTPWebServer.exe --> Open browser: http://localhost:8080/

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

##### eg:- HTTPWebServer -v -p 8081 -b 20 -r true
