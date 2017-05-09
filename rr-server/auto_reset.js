var fs = require("fs");
var exec = require('child_process').exec;

var captionedPath = __dirname + "/public/imgs/captioned/";
var uploadedPath = __dirname + "/public/imgs/uploaded/";
var conversionPath = __dirname + "/public/imgs/conversion/";

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

function resetStatusFile() {
    fs.readFile(__dirname + "/status.json", 'utf-8', function(err, content) {
        if (err) { console.log(err); }
        var obj = JSON.parse(content);
        var image_index = getAllReadyImages(captionedPath, function(indx) {
            var imgInx = parseInt(indx - 1); // -1 to avoid cointing the vis.json file
            var newData = {
                "image_status": imgInx
            }
            fs.writeFile(__dirname + "/status.json", JSON.stringify(newData), function(err) {
                if (err) {
                    return console.error(err);
                }
                console.log("IMAGE STATUS:", obj.image_status);
            });
        });
    });
}

function main() {
    var arguments = process.argv[2];
    console.log(arguments);
    if (arguments == "-t") {
        // resetStatusFile();
        // console.log("STATUS FILE UPDATED!");
        // console.log("");
        var image_index_upload = getAllReadyImages(uploadedPath, function(indx_upload) {
            var imgInx_upload = parseInt(indx_upload);
            if (imgInx_upload > 0) {
                exec("sudo rm -r " + uploadedPath + "*", function(error, stdout, stderr) {
                    if (error !== null) {
                        console.log('exec error: ' + error);
                    }
                    console.log("All IMAGES have been deleted from the folder:", uploadedPath);
                });
            }
        });

        var image_index_conversion = getAllReadyImages(conversionPath, function(indx_conversion) {
            var imgInx_conversion = parseInt(indx_conversion);
            if (imgInx_conversion > 0) {
                exec("sudo rm " + conversionPath + "*.jpg", function(error, stdout, stderr) {
                    if (error !== null) {
                        console.log('exec error: ' + error);
                    }
                    console.log("All IMAGES have been deleted from the folder:", captionedPath);
                });
            }
        });
    } else if (arguments == "-p") {
        var image_index = getAllReadyImages(captionedPath, function(indx) {
            var imgInx = parseInt(indx - 1); // -1 to avoid cointing the vis.json file
            fs.readFile(__dirname + "/status.json", 'utf-8', function(err, content) {
                if (err) { console.log(err); }
                var obj = JSON.parse(content);
                if (obj.image_status !== imgInx) {
                    var newData = {
                        "image_status": imgInx
                    }
                    fs.writeFile(__dirname + "/status.json", JSON.stringify(newData), function(err) {
                        if (err) {
                            return console.error(err);
                        }
                    });
                }
            });
        });
        var image_index2 = getAllReadyImages(conversionPath, function(indx) {
            var imgInx2 = parseInt(indx);
            if (imgInx2 > 0) {
                exec("sudo rm " + conversionPath + "*.jpg", function(error, stdout, stderr) {
                    if (error !== null) {
                        console.log('exec error: ' + error);
                    }
                    console.log("All IMAGES have been deleted from the folder:", captionedPath);
                });
            }
        });
    }
}

main()
