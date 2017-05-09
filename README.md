# reverie-reset

![alt tag](https://cloud.githubusercontent.com/assets/20104092/24031646/be609e5c-0ab1-11e7-806f-35d1220861fd.jpg)


This README file describes the parts and uses of the system Reverie-reset developed by CAC_LAB under the direction of fito_segrera. A total of 80 LCD displays connected to 10 computers (each controlling 8 screens) and a local server. The system allows people to upload images from their phones to the server. This images are processed to obtain the average color and then sent to a cognitive computation system to be analyzed and captioned as text.

### Files Included in the server folder

1. server.js: Server or main app of the system. Creates a route for uploading images at localhost:8080. Users can upload images to the system through this url. This app runs all subprocesses for cleaning and managing files + captioning images through the artificial neural network in realtime.

2. config.json: The main configuration file for images' paths, commands, server and wesocket ports, log messages, etc.

3. full_reset.js: A file to manually reset the system. It deletes all images in the system, captions (texts), status file, etc. WARNING: Please only use this file if you are sure you want to reset the entire system. All data will be lost.

4. auto_reset.js: This file is called internally by the server wen needed. It is designed to clean some trash images that sometimes remain in some of the folders. It also fixes the status file if for any reason its contents get messed up.

5. status.json: is a simple json object which keeps track of all captioned images vs images waiting to be captioned by the neuralnet.

6. package.json: regular nodejs json file containing all npm dependencies to be installed.

7. public: Folder with the client side of the webapp as well as the folders with the images. 3 main folders:

		/public/imgs/uploaded
		/public/imgs/captioned
		/public/imgs/conversion

All uploaded images go to the uploaded folder, the system copies them to the conversion folder and then removes them from the uploaded folder.

8. vis.json file: This last file will keep all the captioned data per image + each image's id.

### Server

User are invited to scan a QR code which takes them to a local url. Here they can press a button which allows them to upload an image to the server. The system will:

1. The clinet "index.html" checks and makes sure the file is either a .jpg, .bpm or .png, if none is detected, prompts a message for the user to try again.

2. If an image is successfully uploaded, save it on the server's path /public/imgs/uploaded/. Convert the image into a .jpg file, give it a random hash name and move it to the /public/imgs/conversion/ folder. The system will inmediately do a request to the docker app runing the neuralnetwork as a separate server on port 5000. The system PUTs the uploaded image and waits for the server response, which comes as a json object with image_id and caption data. Finally the captioned images get moved to the /public/imgs/captioned/ folder and the captioned data get added to the vis.json file.

The vis.json looks like this:

		[
			.
			.
			.
			{
				"image_id":"55",
				"caption":"a couple of people on a boat in the water"
			},
			{
				"image_id":"56",
				"caption":"a couple dogs playing"
			},
			{
				"image_id":"57",
				"caption":"a man is flying a kite on the beach"
			},
			.
			.
			.
		]

### Captioning

The captioning happens by means of the docker app [jacopofar/neuraltalk2-web](https://github.com/jacopofar/neuraltalk2-web) with a pretrained model. Please follow the instructions on the previous link in order to install the docker app. The neuraltalk2-web app runs on a separate server and it needs to be initiated by the server with the following command:

		sudo docker run --name neuraltalk2-web -p 5000:5000 -v /home/reverie-reset/captiondata:/mounted jacopofar/neuraltalk2-web:latest

This command has been added as a child process to the main server.js script. At boot, the server checks for any docker container running and if it finds one it kills it. Finally it executes the jacopofar/neuraltalk2-web.

### COM Protocol

The communication is done via websockets. The basic structure is: 

		Server  <-->  Client 1
		Server  <-->  Client 2
		Server  <-->  Client 3
		Server  <-->  Client 4
		Server  <-->  Client 5

Events Definition:

SYSTEM INITIATED SERVER ONLINE + 
CLIENTS CONNECTED AND READY:

	    c1$ready
	    c2$ready
	    c3$ready
	    c4$ready
	    c5$ready

RANDOM STATE -> IF THERE IS NO NEW IMAGE UPLOAD, THE SERVER SENDS OUT A JSON OBJECT WITH ALL RANDOM DATA:    
    
	    {
	        "1": 
	            {
	                "1":{
	                        "image_id":"259",
	                        "caption":"a close up of a clock on a wall"
	                    },
	                "2":{
	                        "image_id":"126",
	                        "caption":"a man on the beach"
	                    },
	                    .
	                    .
	                    .
	                (12 objects in total, all random)
	            },
	        "2":
	            {
	                (same here)
	            },
	        .
	        .
	        .
	        (All clients: 1, 2, 3, 4, 5)
	    }

NEW IMAGE STATE -> IF THERE IS A NEW IMAGE UPLOADED AND WAITING, THE SERVER SENDS OUT THE DATA FOR THE NEW IMAGE:

		{
	        "1": 
	            {
	                "1":{
	                        "image_id":"259",
	                        "caption":"a close up of a clock on a wall"
	                    }
	            },
	        "2":
	            {
	                "1":{
	                        "image_id":"259",
	                        "caption":"a close up of a clock on a wall"
	                    }
	            },
	        "3":
	            {
	                "1":{
	                        "image_id":"259",
	                        "caption":"a close up of a clock on a wall"
	                    }
	            },
	        .
	        .
	        .
	        (All clients: 1, 2, 3, 4, 5)
	    }


-------------------

####Some Extra and Usefull Docker Commands

- list containers:

		sudo docker ps

- Stop a running container:
	
		sudo docker rm -fv CONTAINERID

- running the APP:

		sudo docker run --name neuraltalk2-web -p 5000:5000 -v /home/reverie-reset/captiondata:/mounted jacopofar/neuraltalk2-web:latest

- running the BASH

		docker exec -it neuraltalk2-web bash

- Getting the sha256sum:

		curl -X POST -H "Content-Type: application/json" -d '{"url":"http://192.168.1.143:8080/1.jpg"}' 'http://localhost:5000/addURL'

This returns a JSON like this:

		{"extension":"jpg","sha256sum":"c79dd842f5343ec810b7a8a425195794a89e259b36bc7768323f5e50f888db7a"}

- Getting the caption:

		curl 'http://localhost:5000/caption/c79dd842f5343ec810b7a8a425195794a89e259b36bc7768323f5e50f888db7a'