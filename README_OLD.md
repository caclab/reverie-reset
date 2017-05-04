# reverie-reset

![alt tag](https://cloud.githubusercontent.com/assets/20104092/24031646/be609e5c-0ab1-11e7-806f-35d1220861fd.jpg)


This README file describes the parts and uses of the system Reverie-reset developed by CAC_LAB under the direction of fito_segrera. A total of 80 LCD displays connected to 10 computers (each controlling 8 screens) and a local server. The system allows people to upload images from their phones to the server. This images are processed to obtain the average color and then sent to a cognitive computation system to be analyzed and captioned as text.

### Files Included in the server folder

1. server.js: Server or main app of the system. Creates a rout for uploading images at localhost:5000. Users can upload images to the system through this url. This app runs all subprocesses for cleaning and managing files + captioning images through the artificial neural network in realtime.

2. config.json: The main configuration file for images' paths, commands, server and wesocket ports, log messages, etc.

3. full_reset.js: A file to manually reset the system. It deletes all images in the system, captions (texts), status file, etc. WARNING: Please only use this file if you are sure you want to reset the entire system. All data will be lost.

4. auto_reset.js: This file is called internally by the server wen needed. It is designed to clean some trash images that sometimes remain in some of the folders. It also fixes the status file if for any reason its contents get messed up.

5. status.json: is a simple json object which keeps track of all captioned images vs images waiting to be captioned by the neuralnet.

6. package.json: regular nodejs json file containing all npm dependencies to be installed.

7. neuraltalk2: Folder with the entire neural network app.

8. public: Folder with the client side of the webapp as well as the folders with the images. 3 main folders:

		/public/imgs/uploaded
		/public/imgs/captioned
		/public/imgs/conversion

All uploaded images go to the uploaded file, the system copies them to the conversion folder and then removes them from the uploaded folder. Once there is a cycle check, the neuralnet will run and caption all images on the conversion folder and move them to the captioned folder. This last folder keeps all images which have been processed + a vis.json file.

9. vis.json file: This last file will keep all the captioned data per image + each image's id.

### Server

User are invited to scan a QR code which takes them to a local url. Here they can press a button which allows them to upload an image to the server. The system will:

1. The clinet "index.html" checks and makes sure the file is either a .jpg, .bpm or .png, if none is detected, prompts a message for the user to try again.

2. If an image is successfully uploaded, save it on the server's path /public/imgs/uploaded/. Convert the image into a .jpg file, give it a random hash name and move it to the /public/imgs/conversion/ folder. The system has a cycle with a period of 30 secs by default (which can be modified at any time in the config.json file). When a cycle finishes the system checks for images in the /public/imgs/conversion/ folder. If there are images there, the neuralnet will be executed and it will caption all these images. The images get moved to the /public/imgs/captioned/ folder.

The way it keeps order of the images is as follows:

a. It checks for the total number of images that are already saved in the /public/imgs/captioned/ folder. If there are, lets say, 54 images already saved and captioned, it starts the neural network with the image_index 55. The new images in the conversion folder will be saved as 55.jpg, 56.jpg, 57.jpg and so on. The same goes to the vis.json file where the captions are saved, it will add them:

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

The system monitors for any change on the folder public/imgs/uploaded. Every time there is a new upload, the main computer will process this using the cognitive computation system in order to generate the image's caption. The new image gets renamed and saved into public/imgs/captioned with the .json file containing the text. The .json file will have the same name as the image, ie:

	img-1.png 
	img-1.json

Once the image is captioned, it gets deleted.

### IMPORTANT NOTES

1. The path for the vis folder has to be changed a made ABSOLUTE based on the machine where it is running. GO TO LINES 145 AND 189 on the eval.lua and make appropiate changes.

Example for line 145: 

	/home/myusername/someFolder/images/captioned/

Example for line 189:

	/home/myusername/someFolder/output/vis.json

### COM Protocol

The communication is done via websockets. The basic structure is: 

		Server  <-->  Client 1
		Server  <-->  Client 2
		Server  <-->  Client 3
		Server  <-->  Client 4
		Server  <-->  Client 5

Events Definition:

1. Clients connect to server and is online:

	$READY

2. No new image is uploaded currently, server sends the state message:
	
	$STATE-RANDOM

The client is ready to show random images from database:

	$R-STANBY

Server responds with a vis.json file will all images in database and their corresponding captions.

3. Client received the data and randomly choses the images and their respective captions. The client then loads the image, scales and rotates if necesary and averages the color of the pixels, once this subprocesses are ready, the client sends back a message to the server:

	$R-READY 

4. When all clients are ready with the preprocessed images and texts, meaning when the server received all 5 $R-READY messages, the server replies back with the trigger to display:

	$R-SHOW

This is a broadcast to all clients, they inmediately show their images on their displays.

The same protocol and process applies to the new image state of the system. This means if the system receive a new uploaded image from a user in the gallery space. The system will allways give priority to the new images submitted. The difference is on the message but the type of messages are the same. If there is a new image the server will inmediately send the following message:

	$STATE-UPLOAD

The rest are the same as the random state but replaced with:
	
	$U-STANDBY
	$U-READY
	$U-SHOW