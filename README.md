# reverie-reset

###Requirements

1. imagemagik

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