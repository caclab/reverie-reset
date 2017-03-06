var express = require('express');
var fileUpload = require('express-fileupload');
var watch = require('nodewatch');
var fs = require('fs');
var ws = require("nodejs-websocket");
// var exec = require('child_process').exec;
var exec = require('child_process');
var config = require('./config.json');
var readChunk = require('read-chunk');
const imageType = require('image-type');

var Jimp = require("jimp");
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

function checkImageScaleAndOrientation(lenna, callback) {
    // console.log("\nIMG WIDTH", lenna.bitmap.width);
    // console.log("IMG HEIGTH", lenna.bitmap.height);
    // if (lenna.bitmap.width >= lenna.bitmap.height) {
    //     lenna.resize(Jimp.AUTO, 720, function() {
    //         if (lenna.bitmap.width < 1280) {
    //             lenna.resize(1280, Jimp.AUTO);
    //         }
    //     });
    //     // lenna.resize(1280, Jimp.AUTO);
    // } else if (lenna.bitmap.width < lenna.bitmap.height) {
    //     lenna.rotate(-90, function() {
    //             if (lenna.bitmap.width < 1280) {
    //                 lenna.resize(1280, Jimp.AUTO);
    //             }
    //         })
    //         //WITHOUT ROTATION TO VERTICAL IMAGE
    //         // lenna.resize(1280, Jimp.AUTO, function(){
    //         //     if(lenna.bitmap.height < 720){
    //         //         lenna.resize(Jimp.AUTO, 720);
    //         //     }
    //         // });
    // }

    callback(lenna);
}

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

        var image_index = getAllReadyImages(captionedPath, function(indx) {
            var imgInx = parseInt(indx);
            console.log("\nIMAGE INDEX " + imgInx);

            //CHECK EXISTING FILES IN UPLOADS FOLDER
            readFiles(uploadPath, function(totalFiles) {
                console.log("TOTAL FILES IN FOLDER", totalFiles);
                //Execute the NEURAL NET for all saved files
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
                        checkImageScaleAndOrientation(lenna, function(newLenna) {
                            if (err) {
                                throw err;
                            } else {
                                var nameNoType = file.split('.' + imgtype);
                                console.log(nameNoType[0] + ".jpg");
                                newLenna.write(nameNoType[0] + ".jpg", function() {
                                    console.log("IMG CONVERTED TO .jpg");

                                    // exec('sudo rm ' + file, function(error, stdout, stderr) {
                                    //     console.log('stdout: ' + stdout);
                                    //     console.log('stderr: ' + stderr);
                                    //     if (error !== null) {
                                    //         console.log('exec error: ' + error);
                                    //     } else {
                                    //         console.log("ORIGINAL FILE DELETED");
                                    //         exec(neuralCommand + imgInx, function(error, stdout, stderr) {
                                    //             console.log('stdout: ' + stdout);
                                    //             console.log('stderr: ' + stderr);
                                    //             if (error !== null) {
                                    //                 console.log('exec error: ' + error);
                                    //             } else {
                                    //                 console.log("ORIGINAL FILE DELETED");
                                    //             }
                                    //         });
                                    //     }
                                    // });

                                    // Works for one upload at a time, if 2 images are 
                                    // uploaded simultaneously or close to each other, 
                                    // The thread dismisses one and causes an error
                                    var proc1 = exec.spawn('rm', [file]);
                                    proc1.stdout.on('data', function(data) { console.log("stdout: " + data); });
                                    proc1.stderr.on('data', function(data) { console.log("stderr: " + data); });
                                    proc1.on('exit', function(code) {

                                        console.log("exit: " + code);
                                        console.log("ORIGINAL FILE DELETED");

                                        exec.exec(neuralCommand + imgInx, function(error, stdout, stderr) {
                                            console.log('stdout: ' + stdout);
                                            console.log('stderr: ' + stderr);
                                            if (error !== null) {
                                                console.log('exec error: ' + error);
                                            } else {
                                                console.log("ORIGINAL FILE DELETED");
                                            }
                                        });

                                    });
                                });

                            }
                        });
                    });

                }
            });
        });
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
