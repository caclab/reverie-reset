# reverie-reset

This README file describes the parts and uses of the system Reverie-reset developed by CAC_LAB under the direction of fito_segrera. A total of 80 LCD displays connected to 10 computers (each controlling 8 screens) and a local server. The system allows people to upload images from their phones to the server. This images are processed to obtain the average color and then sent to a cognitive computation system to be analyzed and captioned as text.

### Server

User are invited to scan a QR code which takes them to a local url. Here they can press a button which allows them to upload an image to the server. The system will:

1. Check and make sure the file is either a .jpg or .png, if none is detected, print a message for the user and then redirect to the upload link and try again.

2. If an image is successfully uploaded, then make sure the name doesn't have spaces or uppper case letters. If everything is ok, save it on the server's path public/imgs/uploaded

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

The communication is done via websockets. The basic structure is as follows:

	Server  <-->  Client 1
			<-->  Client 2
			<-->  Client 3
			<-->  Client 4
			<-->  Client 5

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
