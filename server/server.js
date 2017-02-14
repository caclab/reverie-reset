var express = require('express');
var fileUpload = require('express-fileupload');
var watch = require('nodewatch');
var fs = require('fs');
var ws = require("nodejs-websocket");
var exec = require('child_process').exec;
var config = require('./config.json');

var app = express();

var PORT = config.server_config.PORT; //http port
var SPORT = config.server_config.SPORT; //ws port
var uploadPath = __dirname + config.server_config.uploadPath;
var captionedPath = __dirname + config.server_config.captionedPath;
var neuralCommand = config.server_config.neuralCommand;

app.use(fileUpload());

watch.add(uploadPath);

// route to index.html
app.use('/', express.static(__dirname + '/public'));

app.post('/upload', function(req, res) {
    var img;
    if (!req.files) {
        res.send('No files were uploaded.');
        return;
    }
    img = req.files.image;
    img.mv(uploadPath + img.name, function(err) {
        if (err) {
            res.status(500).send(err);
        } else {
            res.send('File uploaded!');
        }
    });
});

//FUNCTION CHECKS AND LIST ALL FILES IN ANY GIVEN FOLDER
function readFiles(dirname, onFileContent, onError, callback) {
    fs.readdir(dirname, function(err, filenames) {
        if (err) {
            onError(err);
            return;
        }

        // get total files in folder. 
        var totalFiles = filenames.length;
        var fileCounter = 0;
        filenames.forEach(function(filename) {
            fs.readFile(dirname + filename, 'utf-8', function(err, content) {
                if (err) {
                    onError(err);
                    return;
                }

                var isFileAnImage = checkFilename(filename, ".jpg");

                if (isFileAnImage) {
                    onFileContent(filename, content);
                }

                fileCounter++;

                if (fileCounter >= totalFiles) {
                    callback(fileCounter);
                }
            });
        });

    });
}

//Make sure filenames are .jpg images
function checkFilename(str, suffix) {
    return str.indexOf(suffix, str.length - suffix.length) !== -1;
}

//check all images ready to be displayed
function getAllReadyImages(readyImagesPath, callback) {
    var imgList = [];
    readFiles(readyImagesPath, function(filename) {
        imgList.push(filename);
    }, function(err) {
        throw err;
    }, function(){
        console.log("||||||||||||||||||||||||||||||||||||||");
        console.log("||||||||||Images In Database||||||||||");
        console.log("||||||||||||||||||||||||||||||||||||||");
        console.log("");
        console.log("\nALL IMAGES ARE", imgList);
        console.log("PATH:", readyImagesPath);
        console.log("");
        callback(imgList.length);
    });
}

//check for changes in uploads folder
watch.onChange(function(file, prev, curr, action) {
    console.log("||||||||||||||||||||||||||||||||||||||");
    console.log("||||||||||||OnChange Event||||||||||||");
    console.log("||||||||||||||||||||||||||||||||||||||");
    console.log(file);
    console.log(prev.mtime.getTime());
    console.log(curr.mtime.getTime());
    console.log(action); // new, change, delete
    if (action !== 'delete') {
        console.log('FILE ADDED!!');
        //CHECK EXISTING FILES IN UPLOADS FOLDER
        // readFiles(uploadPath, function(filename, content) {
        //Execute the NEURAL NET for all saved files
        var image_index = getAllReadyImages(captionedPath, function(indx) {
            var imgInx = parseInt(indx);
            console.log("\nIMAGE INDEX " + imgInx);
            console.log(captionedPath);
            console.log(neuralCommand + imgInx);
            exec(neuralCommand + imgInx.toString(), function(error, stdout, stderr) {
                console.log('stdout: ' + stdout);
                console.log('stderr: ' + stderr);
                if (error !== null) {
                    console.log('exec error: ' + error);
                }
            });
        });

        // }, function(err) {
        //     throw err;
        // });
    }
});

//Socket server WS
var ws_server = ws.createServer(function(conn) {
    console.log("New connection");
    conn.sendText("hello world");
    // conn.on("message", function (str) {
    //     console.log("Received "+str);
    // });
    conn.on("close", function(code, reason) {
        console.log("Connection closed");
    });
}).listen(SPORT);

//Start MAIN app http Server
app.listen(PORT, function() {
    config.server_config.about.forEach(function(line) {
        if (line === "HTTP Server listening on port") {
            console.log(line + " " + PORT.toString());
        } else if (line === "WS Server listening on port") {
            console.log(line + " " + SPORT.toString());
        } else {
            console.log(line);
        }
    });
});
