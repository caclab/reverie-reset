var express = require('express');
var fileUpload = require('express-fileupload');
var watch = require('nodewatch');
var fs = require('fs');
// var exec = require('child_process').exec;
var exec = require('child_process');
var config = require('./config.json');

var readChunk = require('read-chunk');
const imageType = require('image-type');
var Jimp = require("jimp");
var app = express();
var randomstring = require("randomstring");

const WebSocket = require('ws');

var PORT = config.server_config.PORT; //http port
var SPORT = config.server_config.SPORT; //ws port
var uploadPath = __dirname + config.server_config.uploadPath;
var captionedPath = __dirname + config.server_config.captionedPath;
var conversionPath = __dirname + config.server_config.conversionPath;
var neuralCommand = config.server_config.neuralCommand;
var statusPath = __dirname + config.server_config.statusPath;
var systemInterval = config.server_config.systemInterval;
var clients = config.server_config.clients;
var screensPerLayer = config.server_config.screensPerLayer;

var numberOfClients = 0;
var toCompare = [];
for (var key in clients) {
    if (clients.hasOwnProperty(key)) {
        console.log(key + " -> " + clients[key]);
        if (clients[key]) {
            toCompare.push(key);
            numberOfClients++;
        }
    }
}

app.use(fileUpload());

watch.add(uploadPath);

var pathAutoRest = __dirname + "/auto_reset.js";
//AUTO-RESET in case there is leftover trash on the server folders
exec.exec("node " + pathAutoRest + " -t", function(error, stdout, stderr) {
    console.log('stdout: ' + stdout);
    console.log('stderr: ' + stderr);
    if (error !== null) {
        console.log('exec error: ' + error);
    } else {
        console.log("AUTO RESET DONE!");
    }
});

// route to index.html
app.use('/', express.static(__dirname + '/public'));

// Route to UPLOADS folder
app.post('/upload', function(req, res) {
    var img;
    if (!req.files) {
        res.send('No files were uploaded.');
        return;
    }
    img = req.files.image;
    var filetype = req.files.image.mimetype;
    filetype = filetype.split("/");
    console.log(filetype[1]);
    img.mv(uploadPath + img.name, function(err) {
        if (err) {
            res.status(500).send(err);
        } else {
            res.send('File uploaded!');
        }
    });
});

//FUNCTION CHECKS AND LIST ALL FILES IN ANY GIVEN FOLDER
function readFiles(dirname, callback) {
    fs.readdir(dirname, function(err, filenames) {
        if (err) {
            console.log(err);
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
                fileCounter++;
                if (fileCounter >= totalFiles) {
                    callback(fileCounter - 1);
                }

            });
        });

    });
}

//check all images ready to be displayed
function getAllReadyImages(readyImagesPath, callback) {
    readFiles(readyImagesPath, function(total) {
        callback(total);
    }, function(err) {
        throw err;
    }, function() {
        console.log("||||||||||||||||||||||||||||||||||||||");
        console.log("||||||||||Images In Database||||||||||");
        console.log("||||||||||||||||||||||||||||||||||||||");
        console.log("");
        console.log("PATH:", readyImagesPath);
        console.log("");
    });
}

//Execute NeuralNet
function runNeuralNet(imgInx) {
    //Please check the eval.lua for errors on saving the vis.json and the image file
    console.log("NEURAL NET COMMAND:", neuralCommand + imgInx);
    exec.exec(neuralCommand + imgInx, function(error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
            console.log('exec error: ' + error);
        } else {
            console.log("ORIGINAL FILE DELETED");
        }
    });
}

//Random data to display on screens
function prepareDataForClient(callback) {
    fs.readFile(captionedPath + "vis.json", 'utf-8', function(err, visData) {
        if (err) { console.log(err); }
        var income = JSON.parse(visData);
        var toSendTotal = {};
        for (var c = 0; c < numberOfClients; c++) {
            var toSend = {};
            for (var i = 0; i < screensPerLayer; i++) {
                toSend[i + 1] = income[Math.floor(Math.random() * income.length)];
                //console.log(toSend[i]);
                if (i == screensPerLayer - 1) {
                    toSendTotal[c + 1] = toSend;
                }
            }
            if (c == numberOfClients - 1) {
                callback(toSendTotal);
                //client_status = "";
                wwws.send(JSON.stringify(toSendTotal));
            }
        }
    });
}

//compare contents of 2 arrays
Array.prototype.compare = function(testArr) {
    if (this.length != testArr.length) return false;
    for (var i = 0; i < testArr.length; i++) {
        if (this[i].compare) { //To test values in nested arrays
            if (!this[i].compare(testArr[i])) return false;
        } else if (this[i] !== testArr[i]) return false;
    }
    return true;
}

//Function to recursivelly check for any changes in the system:
//It checks a status file which contains the previous index of captioned files
//If the number stored in the status.json file is greater than the total files in the captioned folder
//in the status file, then caption all images in conversion folder
function generalSystemInterval() {
    setInterval(function() {
        var image_index = getAllReadyImages(uploadPath, function(i) {
            var uploadIndex = parseInt(i); // -1 to avoid cointing the vis.json file
            if (i > 0) {
                exec.exec("rm -r " + uploadPath + "*", function(error, stdout, stderr) {
                    console.log('stdout: ' + stdout);
                    console.log('stderr: ' + stderr);
                    if (error !== null) {
                        console.log('exec error: ' + error);
                    }
                });
            }
            fs.readFile(statusPath, 'utf-8', function(err, content) {
                if (err) { console.log(err); }
                var obj = JSON.parse(content);
                console.log("IMAGE STATUS:", obj.image_status);
                var image_index = getAllReadyImages(captionedPath, function(indx) {
                    var imgInx = parseInt(indx - 1); // -1 to avoid cointing the vis.json file
                    console.log("IMAGE INDEX IN CAPTIONED FOLDER:", imgInx);

                    var image_index2 = getAllReadyImages(conversionPath, function(indx2) {
                        var imgInx2 = parseInt(indx2);
                        console.log("IMAGE INDEX IN CONVERSION FOLDER:", imgInx);

                        var totalIndex = imgInx + imgInx2;

                        //Check and validate all connected clients
                        //Then send random data to clients
                        if (client_status.sort().compare(toCompare.sort())) {
                            console.log("\nPreparing DATA to Send to clients!\n");
                            prepareDataForClient(function(toSend) {
                                console.log(toSend);
                            });
                        } else {
                            console.log("\nERROR with connected clients");
                            console.log("No data was sent!");
                            console.log("Make sure all clients are connected!");
                            console.log("Current connected clients:", client_status);
                            console.log("Expected connected clients:", toCompare, "\n");
                        }

                        if (totalIndex > imgInx) {
                            console.log("NEW IMAGE(S) FOUND!!");
                            runNeuralNet(imgInx + 1);
                            var ims = totalIndex;
                            var newData = {
                                "image_status": ims
                            }
                            fs.writeFile(__dirname + "/status.json", JSON.stringify(newData), function(err) {
                                if (err) {
                                    return console.error(err);
                                }

                            });
                        } else {
                            console.log("NO NEW IMAGES FOUND!!");
                            //AUTO-RESET in case there is leftover trash on the server folders
                            exec.exec("node " + pathAutoRest + " -p", function(error, stdout, stderr) {
                                console.log('stdout: ' + stdout);
                                console.log('stderr: ' + stderr);
                                if (error !== null) {
                                    console.log('exec error: ' + error);
                                } else {
                                    console.log("AUTO RESET DONE!");
                                }
                            });
                        }
                    });
                });
            });
            console.log("\nNEW INTERVAL CYCLE!");
        });


    }, systemInterval);
}

generalSystemInterval();

//check for changes in UPLOADED folder
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
        readFiles(uploadPath, function(totalFiles) {
            console.log("TOTAL FILES IN FOLDER", totalFiles);
            //Execute the NEURAL NET for all saved files
            if (fs.existsSync(file)) {
                var buffer = readChunk.sync(file, 0, 100);
                var imgtype = imageType(buffer);
                imgtype = imgtype.ext;
                console.log("IMG TYPE", imgtype);

                //Check if filetype is an image
                if (imgtype == 'jpg' || imgtype == 'JPG' ||
                    imgtype == 'png' || imgtype == 'PNG' ||
                    imgtype == 'bpm' || imgtype == 'BPM' ||
                    imgtype == 'jpeg' || imgtype == 'JPEG') {

                    Jimp.read(file, function(err, lenna) {

                        if (err) {
                            throw err;
                        } else {
                            var nameNoType = file.split('.' + imgtype);
                            console.log(nameNoType[0] + ".jpg");
                            lenna.write(conversionPath + randomstring.generate(10) + ".jpg", function() {
                                console.log("IMG CONVERTED TO .jpg");

                            });
                        }
                    });
                }
            }
        });
    }
});

/*
PROTOCOL

SYSTEM INITIATED SERVER ONLINE + 
CLIENTS CONNECT AND SEND:

    c1$ready
    c2$ready
    c3$ready
    c4$ready
    c5$ready

IF THERE IS NO NEW IMAGE UPLOAD, THE SERVER SENDS OUT
A JSON OBJECT WITH ALL RANDOM DATA:    
    
    {
        "c1": 
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
        "c2":
            {
                (same here)
            },
        .
        .
        .
        (All clients: c1, c2, c3, c4, c5)
    }



*/

var client_status = [];
var client_ips = [];
var wwws;
//Create websocket server
const wss = new WebSocket.Server({ port: SPORT });
wss.on('connection', function connection(ws) {
    wwws = ws;
    console.log('Client connected!');
    var tmpIp = ws._socket.remoteAddress;
    tmpIp = tmpIp.split("::");
    tmpIp = tmpIp[1].split(":");
    console.log(tmpIp[1]);
    client_ips.push(tmpIp[1]);
    console.log("CURRENT CONNECTED CLIENTS:", client_ips);

    ws.on('message', function incoming(message) {
        console.log('received: %s', message);
        var tmp = message.split('$');
        if (tmp[1] == 'ready') {
            client_status.push(tmp[0]);
            //console.log(client_status);
        }
    });
    ws.on('close', function close(data) {
        console.log('disconnected');
        var tmp_client_ips = [];
        wss.clients.forEach(function each(client) {
            if (client.readyState === WebSocket.OPEN) {
                console.log(client._socket.remoteAddress);
                var tmpIp = client._socket.remoteAddress;
                tmpIp = tmpIp.split("::");
                tmpIp = tmpIp[1].split(":");
                tmp_client_ips.push(tmpIp[1]);
            }
        });
        client_ips = tmp_client_ips;
        console.log("CURRENT CONNECTED CLIENTS:", client_ips);
    });
});

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
