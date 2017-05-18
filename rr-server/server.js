var express = require('express');
var fileUpload = require('express-fileupload');
var watch = require('nodewatch');
var fs = require('fs');
var exec = require('child_process');
var config = require('./config.json');
var readChunk = require('read-chunk');
const imageType = require('image-type');
var Jimp = require("jimp");
var app = express();
var randomstring = require("randomstring");
const WebSocket = require('ws');
var os = require('os');

//////////////////////////////////////
//GET ALL DATA FROM CONFIG.JSON FILE//
//////////////////////////////////////
var PORT = config.server_config.PORT; //http port
var SPORT = config.server_config.SPORT; //ws port
var uploadPath = __dirname + config.server_config.uploadPath;
var captionedPath = __dirname + config.server_config.captionedPath;
var conversionPath = __dirname + config.server_config.conversionPath;
var statusPath = __dirname + config.server_config.statusPath;
var systemInterval = config.server_config.systemInterval;
var clients = config.server_config.clients;
var screensPerLayer = config.server_config.screensPerLayer;
var neuralNetServerAddress = config.server_config.neuralNetServerAddress;
var totalAllowedImages = config.server_config.totalAllowedImages;

var stateSwitch = "r"; // r for random, n for new-image-upload
var newImageCounter = 0; // keeps track of how many new images are uploaded
var newImageQue = 0;
var uploadsCounter = 0;
var randomCycleCounter = 0;

////////////////
//GET LOCAL IP//
////////////////
var ifaces = os.networkInterfaces();
var localip = "";

Object.keys(ifaces).forEach(function(ifname) {
    var alias = 0;
    ifaces[ifname].forEach(function(iface) {
        if ('IPv4' !== iface.family || iface.internal !== false) {
            // skip over internal (i.e. 127.0.0.1) and non-ipv4 addresses
            return;
        }
        if (alias >= 1) {
            // this single interface has multiple ipv4 addresses
            //console.log(ifname + ':' + alias, iface.address);
        } else {
            // this interface has only one ipv4 adress
            //console.log(ifname, iface.address);
            if (ifname == 'enp0s25') {
                localip = iface.address;
            }
        }
        ++alias;
    });
});

/////////////////////////////
//LOAD CONTENTS OF VIS.JSON//
/////////////////////////////
var visDataTMP;

function loadVisFile() {
    fs.readFile(captionedPath + 'vis.json', 'utf-8', function(err, content) {
        if (err) { console.log(err); }
        visDataTMP = JSON.parse(content);
        console.log(visDataTMP.length, "images in database");
    });
}

////////////////////////////////////////////////////////////////////
//AUTO-RESET in case there is leftover trash on the server folders//
////////////////////////////////////////////////////////////////////
var status; //save data from status.json
var pathAutoRest = __dirname + "/auto_reset.js";
exec.exec("node " + pathAutoRest + " -t", function(error, stdout, stderr) {
    //console.log('stdout: ' + stdout);
    //console.log('stderr: ' + stderr);
    if (error !== null) {
        //console.log('exec error: ' + error);
    } else {
        console.log("AUTO RESET DONE!");
        //get data from status file from previous session
        fs.readFile(statusPath, 'utf-8', function(err, content) {
            if (err) { console.log(err); }
            var obj = JSON.parse(content);
            //console.log("IMAGE STATUS:", obj.image_status);
            status = obj.image_status + 1;
            console.log("STATUS:", status - 1);
            loadVisFile();
        });
    }
});

////////////////////////////////////////////////
//CLOSE DOCKER IF IT IS RUNNING and RESTART IT//
////////////////////////////////////////////////
var exec = require('child_process');
exec.exec('sudo docker run --name neuraltalk2-web -p 5000:5000 -v /home/reverie-reset/captiondata:/mounted jacopofar/neuraltalk2-web:latest', function(error, stdout, stderr) {
    //console.log('stdout: ' + stdout);
    //console.log('stderr: ' + stderr);
    if (error !== null) {
        //console.log('exec error: ' + error);
        exec.exec('sudo docker ps -aqf "name=neuraltalk2-web"', function(error, stdout, stderr) {
            //console.log('stdout: ' + stdout);
            //console.log('stderr: ' + stderr);
            if (error !== null) {
                console.log('exec error: ' + error);
            } else {
                exec.exec('sudo docker rm -fv ' + stdout, function(error, stdout, stderr) {
                    //console.log('stdout: ' + stdout);
                    //console.log('stderr: ' + stderr);
                    if (error !== null) {
                        console.log('exec error: ' + error);
                    } else {
                        exec.exec('sudo docker run --name neuraltalk2-web -p 5000:5000 -v /home/reverie-reset/captiondata:/mounted jacopofar/neuraltalk2-web:latest', function(error, stdout, stderr) {
                            //console.log('stdout: ' + stdout);
                            //console.log('stderr: ' + stderr);
                            if (error !== null) {
                                console.log('exec error: ' + error);
                            } else {
                                console.log("docker app running on port 5000");
                            }
                        });
                    }
                });
            }
        });
    } else {
        console.log("docker app running on port 5000");
    }
});


/////////////////////////////////////
//ON APP CLOSING OR CRASHING EVENTS//
/////////////////////////////////////
process.stdin.resume();

function exitHandler(options, err) {
    if (options.cleanup) {
        console.log('CLOSING SERVER');
    }
    if (err) {
        console.log(err.stack);
    }
    if (options.exit) {
        process.exit();
    }

}

//do something when app is closing
process.on('exit', exitHandler.bind(null, { cleanup: true }));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, { exit: true }));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, { exit: true }));

////////////////////////////////////////////////
//GET DATA OF ALLOWED CLIENTS FROM CONFIG.JSON//
////////////////////////////////////////////////
var numberOfClients = 0;
var toCompare = [];
for (var key in clients) {
    if (clients.hasOwnProperty(key)) {
        //console.log(key + " -> " + clients[key]);
        if (clients[key]) {
            toCompare.push(key);
            numberOfClients++;
        }
    }
}

////////////////////////////////////////////
//SETUP EXPRESS APP AND WATCH FOLDER EVENT//
////////////////////////////////////////////
var path = require('path');
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
    //console.log(filetype[1]);
    img.mv(uploadPath + img.name, function(err) {
        if (err) {
            res.status(500).send(err);
        } else {
            res.sendFile(path.join(__dirname + '/public/redirect.html'));
        }
    });
});

// Route to MONITOR
app.get('/monitor', function(req, res) {
    res.sendFile(path.join(__dirname + '/public/monitor.html'));
});

//////////////////////////////////////////////////////////
//FUNCTION CHECKS AND LIST ALL FILES IN ANY GIVEN FOLDER//
//////////////////////////////////////////////////////////
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

/////////////////////
//Execute NeuralNet//
/////////////////////
function runNeuralNet(iName, callback) {
    var img = iName;
    var curlCommand = "curl -X POST -H \"Content-Type: application/json\" -d \'{\"url\":\"http://" + localip + ":8080/" + img + "\"}\' \'http://localhost:5000/addURL\'";

    function getCaptionData(sha256sum, callback2) {
        exec.exec("curl \'" + neuralNetServerAddress + sha256sum + "\'", function(error, stdout, stderr) {
            //console.log('stdout: ' + stdout);
            //console.log('stderr: ' + stderr);
            if (status > totalAllowedImages) {
                status = 1;
                newImageCounter = 0;
                newImageQue = 0;

                console.log("UPDATED STATUS:", status);
            }
            console.log("CURRENT STATUS:", status);
            var capobj = {};
            if (error !== null) {
                //console.log('exec error: ' + error);
            } else {
                var capobj = JSON.parse(stdout);
                //console.log("CAPTION:", capobj.caption);
                callback2(capobj.caption);
            }
        });
    }

    function runAll() {
        exec.exec(curlCommand, function(error, stdout, stderr) {
            //console.log('stdout: ' + stdout);
            //console.log('stderr: ' + stderr);
            var totalData = { "image_id": "", "caption": "" };
            if (error !== null) {
                console.log('exec error: ' + error);
            } else {
                var obj = JSON.parse(stdout);
                console.log(obj.sha256sum);
                var tries = 0;
                var getDataInterval = setInterval(function() {
                    getCaptionData(obj.sha256sum, function(data) {
                        console.log(data, obj.sha256sum);
                        if (data !== undefined) {
                            clearInterval(getDataInterval);
                            totalData = { "image_id": status, "caption": data }
                            status++;
                            uploadsCounter++;
                            newImageQue++;
                            console.log("NEW IMAGE COUNTER:", newImageCounter);
                            console.log("NEW IMAGE WAITING-LIST:", newImageQue, "\n");
                            callback(totalData);
                        } else if (tries > 20) {
                            clearInterval(getDataInterval);
                            console.log("RETRYING TO GET CAPTIONS");
                            runAll();
                            tries = 0;
                        }
                        tries++;
                    });
                }, 2000);
            }
        });
    }
    runAll();
}

//////////////////////////////
//Data to display on screens//
//////////////////////////////
function prepareDataForClient(callback) {
    console.log("NewImageQue", newImageQue, "newImageCounter", newImageCounter);
    if (newImageQue > 0 && newImageCounter < 3) {
        stateSwitch = "n";
        newImageCounter++;
    } else {
        stateSwitch = "r";
        newImageCounter = 0;
        randomCycleCounter++;
    }

    console.log("ACTUAL SYSTEM'S STATE", stateSwitch, "\n");

    var income = visDataTMP;
    var toSendTotal = {};
    var totalImgs = income.length;

    for (var c = 0; c < numberOfClients; c++) {
        var toSend = {};
        for (var i = 0; i < screensPerLayer; i++) {
            if (stateSwitch == "r") {
                var rchoice = Math.floor(Math.random() * income.length);
                toSend[i + 1] = income[rchoice];
                toSend[i + 1]['image_id'] = (toSend[i + 1]['image_id']).toString();
                //console.log(toSend[i]);
            } else if (stateSwitch == "n" && i == 1) {
                toSend[i] = income[(status - 1) - newImageQue];
                toSend[i]['image_id'] = (toSend[i]['image_id']).toString();
            }

            if (i == screensPerLayer - 1) {
                toSendTotal[c + 1] = toSend;
            }
        }

        if (c == numberOfClients - 1) {
            callback(toSendTotal);
            //wwws.send(JSON.stringify(toSendTotal));
            wss.clients.forEach(function each(client) {
                if (client.readyState === WebSocket.OPEN) {
                    client.send(JSON.stringify(toSendTotal));
                }
            });
            console.log("NewImageQue", newImageQue, "newImageCounter", newImageCounter);
            if (stateSwitch == "n") {
                newImageQue--;
            }
        }
    }
}

////////////////////////////////
//compare contents of 2 arrays//
////////////////////////////////
Array.prototype.compare = function(testArr) {
    if (this.length != testArr.length) return false;
    for (var i = 0; i < testArr.length; i++) {
        if (this[i].compare) { //To test values in nested arrays
            if (!this[i].compare(testArr[i])) return false;
        } else if (this[i] !== testArr[i]) return false;
    }
    return true;
}

/////////////////////////
//Save Status.json file//
/////////////////////////
function saveStatusFile(indexToSave) {
    var newData = {
        "image_status": indexToSave
    }
    fs.writeFile(__dirname + "/status.json", JSON.stringify(newData), function(err) {
        if (err) {

            return console.error(err);
        }
        console.log("IMAGE STATUS:", indexToSave);
    });
}

////////////////////////////////////////////////////////////////
//Function to recursivelly check for any changes in the system//
////////////////////////////////////////////////////////////////
function generalSystemInterval() {
    setInterval(function() {
        console.log("\nNEW INTERVAL CYCLE!");
        saveVisJson(visDataTMP);
        saveStatusFile(status - 1);
    }, systemInterval);
}
generalSystemInterval();

////////////////////////////////////////
//check for changes in UPLOADED folder//
////////////////////////////////////////
watch.onChange(function(file, prev, curr, action) {
    if (action !== 'delete') {
        //console.log('FILE ADDED!!');

        //CHECK EXISTING FILES IN UPLOADS FOLDER
        readFiles(uploadPath, function(totalFiles) {
            if (fs.existsSync(file)) {
                var buffer = readChunk.sync(file, 0, 100);
                var imgtype = imageType(buffer);
                imgtype = imgtype.ext;
                //console.log("IMG TYPE", imgtype);

                Jimp.read(file, function(err, lenna) {

                    if (err) {
                        throw err;
                    } else {
                        var nameNoType = file.split('.' + imgtype);
                        //console.log(nameNoType[0] + ".jpg");
                        var imgName = randomstring.generate(10);
                        lenna.write(conversionPath + imgName + ".jpg", function() {
                            //console.log("IMG CONVERTED TO .jpg");

                            runNeuralNet("imgs/conversion/" + imgName + ".jpg", function(p) {
                                for (var i = 0; i < visDataTMP.length; i++) {
                                    if (visDataTMP[i].image_id == p.image_id) {
                                        visDataTMP[i] = p;
                                        // console.log(visDataTMP[i], p);
                                    }
                                }
                                exec.exec("sudo mv " + conversionPath + imgName + ".jpg " + captionedPath + p.image_id + ".jpg", function(error, stdout, stderr) {
                                    //console.log('stdout: ' + stdout);
                                    //console.log('stderr: ' + stderr);
                                    if (error !== null) {
                                        console.log('exec error: ' + error);
                                    } else {
                                        console.log(p.image_id, ".jpg SAVED");
                                        //visDataTMP.push(p);
                                        //console.log(visDataTMP);
                                        exec.exec("rm -r " + file, function(error, stdout, stderr) {
                                            //console.log('stdout: ' + stdout);
                                            //console.log('stderr: ' + stderr);
                                            if (error !== null) {
                                                //console.log('exec error: ' + error);
                                            }
                                        });
                                    }
                                });
                            });
                        });
                    }
                });
            }
        });
    }
});

/////////////////
//SAVE VIS.JSON//
/////////////////
function saveVisJson(vtd) {
    var toSave = JSON.stringify(vtd);
    fs.writeFile(captionedPath + "vis.json", toSave, function(err) {
        if (err) {
            return console.error(err);
        } else {
            console.log("vis.json SAVED!");
            //console.log(toSave, "\n");
        }
    });
}

//////////////////////////////
//Validate connected clients//
//////////////////////////////
function sortAndCompare(callback) {
    if (client_ips.sort().compare(toCompare.sort())) {
        console.log("\nPreparing DATA to Send to clients!\n");
        callback();
    } else {
        console.log("\nERROR with connected clients");
        console.log("No data was sent!");
        console.log("Make sure all clients are connected!");
        console.log("Current READY clients:", client_status);
        console.log("Connected clients:", client_ips);
        console.log("Expected connected clients:", toCompare, "\n");
    }
}

///////////////////////////
var client_status = [];
var client_standby = [];
var client_ips = [];
var monitor_ip;
var wwws;
///////////////////////////
//Create websocket server//
///////////////////////////
const wss = new WebSocket.Server({ port: SPORT });

wss.broadcast = function(data) {
    for (var i in this.clients)
        this.clients[i].send(data);
};

wss.on('connection', function connection(ws) {

    wwws = ws;
    console.log('Client connected!');
    var tmpIp = ws._socket.remoteAddress;
    tmpIp = tmpIp.split("::");
    tmpIp = tmpIp[1].split(":");
    //console.log(tmpIp[1]);

    if (include(toCompare, tmpIp[1])) {
        //console.log(tmpIp[1], "Almost Connected!!");
        if (include(client_ips, tmpIp[1]) === false) {
            client_ips.push(tmpIp[1]);
            //console.log(tmpIp[1], "Connected!!");
        }
    }

    console.log("CURRENT CONNECTED CLIENTS:", client_ips);

    ws.on('message', function incoming(message) {
        console.log('received: %s', message);
        var tmp = message.split('$');
        if (tmp[1] == 'ready') {
            if (include(client_status, tmp[0]) == false) {
                client_status.push(tmp[0]);
                console.log("Current READY clients:", client_status);
            } else {
                console.log("client already added to status list", client_status);
            }

            if (client_status.length === toCompare.length) {
                console.log("ALL CLIENTS READY");
                //Check and validate all connected clients
                sortAndCompare(function() {
                    //Then send random data to clients
                    prepareDataForClient(function(toSend) {
                        console.log(toSend);
                        client_status = [];
                    });
                });
            }
        } else if (tmp[1] == 'standby') {
            client_standby.push(tmp[0]);
            //Make sure all expected clients sent the STANDBY message
            if (client_standby.length === toCompare.length) {
                console.log("ALL CLIENTS STANDING BY");
                //Check and validate all connected clients
                sortAndCompare(function() {
                    //ws.send("BOOM");
                    wss.clients.forEach(function each(client) {
                        if (client.readyState === WebSocket.OPEN) {
                            client.send("BOOM");
                        }
                    });
                    client_standby = [];
                });
            }
        }
    });
    ws.on('close', function close(data) {
        console.log('disconnected');
        var tmp_client_ips = [];
        // var isMonitorConnected = false;
        var counter = 0;
        wss.clients.forEach(function each(client) {
            if (client.readyState === WebSocket.OPEN) {
                console.log(client._socket.remoteAddress);
                var tmpIp = client._socket.remoteAddress;
                tmpIp = tmpIp.split("::");
                tmpIp = tmpIp[1].split(":");
                tmp_client_ips.push(tmpIp[1]);
            }
        });

        if (isEmpty(wss.clients)) {
            console.log("NO CLIENTS CONNECTED!!");
        }

        client_ips = tmp_client_ips;
    });
});

var intervalID = null;

function intervalManager(flag, animate, time) {
    if (flag) {
        intervalID = setInterval(animate, time);
    } else {
        clearInterval(intervalID);
    }
}

function isEmpty(obj) {
    for (var prop in obj) {
        if (obj.hasOwnProperty(prop)) {
            return false;
        }
    }
    return JSON.stringify(obj) === JSON.stringify({});
}

function include(arr, obj) {
    return (arr.indexOf(obj) != -1);
}

//////////////////////////////
//Start MAIN app http Server//
//////////////////////////////
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
    console.log("LocalIP:", localip, "\n");
});
